#include "./Scripting/include/JavaScript/OPjavaScriptV8.h"
#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#ifdef OPIFEX_OPTION_V8

#include "./Core/include/Assert.h"
#include "./include/libplatform/libplatform.h"
#include "./Data/include/OPstring.h"
#include "./Data/include/OPcman.h"
#include "./v8-debug.h"

#define CACHED_FILE_COUNT 256

Isolate* isolate = NULL;
OPint(*OPJAVASCRIPTV8_REQUIRE)(FunctionCallbackInfo<Value>) = NULL;
void(*OPJAVASCRIPTV8_CUSTOMWRAPPER)(Handle<Object>) = NULL;

class ArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
public:
	virtual void* Allocate(size_t length) {
		void* data = AllocateUninitialized(length);
		return data == NULL ? data : memset(data, 0, length);
	}
	virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
	virtual void Free(void* data, size_t) { free(data); }
};

ArrayBufferAllocator allocator;

Persistent<Context, CopyablePersistentTraits<Context>> V8CONTEXT;
Persistent<ObjectTemplate, CopyablePersistentTraits<ObjectTemplate>> V8GLOBALTEMPLATE;


void _OPscriptV8Log(const v8::FunctionCallbackInfo<v8::Value>& args) {
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope scope(args.GetIsolate());
		v8::String::Utf8Value str(args[i]);
		if (*str == NULL) return;
		OPlogChannel(30, "SCRIPT", *str);
	}
}

void _OPscriptV8Error(const v8::FunctionCallbackInfo<v8::Value>& args) {
	for (int i = 0; i < args.Length(); i++) {
		v8::HandleScope scope(args.GetIsolate());
		v8::String::Utf8Value str(args[i]);
		if (*str == NULL) return;
		OPlogErr(*str);
	}
}

void _OPscriptV8SetConsole(Handle<ObjectTemplate> objTemplate) {
    Handle<ObjectTemplate> console = JS_NEW_OBJECT_TEMPLATE();
	console->Set(JS_NEW_STRING("log"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8Log));
	console->Set(JS_NEW_STRING("error"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8Error));
    objTemplate->Set(JS_NEW_STRING("console"), console);
}

OPchar* _OPscriptV8GetDir(const OPchar* path) {
    OPchar* result = OPstringCopy("");
    i32 pos = -1;
    for(i32 i = 0; i < strlen(path); i++) {
        if(path[i] == '/') {
            pos = i;
        }
    }
    if(pos > -1) {
        return OPstringSub(path, 0, pos);
    } else {
        return OPstringCopy("");
    }
}


 OPint _OPjavaScriptRequireScript(OPscript* script, OPchar* dir, Handle<Value>* result) {
    OPjavaScriptV8Compiled compiled;
    if(!OPjavaScriptV8Compile(&compiled, script, dir)) {
		return 0;
	}

    OPjavaScriptV8Run(&compiled);
    Handle<Context> localContext = Local<Context>::New(isolate, compiled.Context);

    localContext->DetachGlobal();
    Local<Object> global = localContext->Global();
    Local<Value> moduleVal = global->Get(JS_NEW_STRING("module"));
    Local<Object> module = moduleVal->ToObject();
    *result = module->Get(JS_NEW_STRING("exports"));
    return 1;
}

OPint loadCount = 0;
Persistent<Value, CopyablePersistentTraits<Value> > loaded[CACHED_FILE_COUNT];
#ifdef _DEBUG
OPscript* loadedScripts[CACHED_FILE_COUNT];
#endif
OPchar* stored[CACHED_FILE_COUNT];

OPint _OPjavaScriptRequire(OPchar* pathToLoad, Handle<Value>* result) {
    OPlog("Loading '%s' for require()", pathToLoad);

    for(OPint i = 0; i < loadCount; i++) {
        if(OPstringEquals(pathToLoad, stored[i])) {
            OPlog("Reloaded %s", pathToLoad);

			#ifdef _DEBUG
			if(loadedScripts[i]->changed) {
				// The script was changed, releoad the cache
			    OPchar* tmpDir = _OPscriptV8GetDir(pathToLoad);
				Handle<Value> tmpResult;
				if(!_OPjavaScriptRequireScript(loadedScripts[i], tmpDir, &tmpResult)) {
					return 0;
				}
				loaded[loadCount] = Persistent<Value, CopyablePersistentTraits<Value> >(isolate, tmpResult);
				*result = tmpResult;
				return 1;
			}
			#endif
			*result = Local<Value>::New(isolate, loaded[i]);
            return 1;
        }
    }

    ASSERT(loadCount < CACHED_FILE_COUNT, "Too many cached files. Increase the CACHED_FILE_COUNT define in OPjavaScriptV8.cpp");

    OPscript* scriptSource = (OPscript *)OPCMAN.LoadGet(pathToLoad);

    OPchar *dir = _OPscriptV8GetDir(pathToLoad);

    Handle<Value> resultTemp;
	if(!_OPjavaScriptRequireScript(scriptSource, dir, &resultTemp)) {
		return 0;
	}


    loaded[loadCount] = Persistent<Value, CopyablePersistentTraits<Value> >(isolate, resultTemp);
    stored[loadCount] = OPstringCopy(pathToLoad);
	#ifdef _DEBUG
	loadedScripts[loadCount] = scriptSource;
	#endif
    loadCount++;

    OPfree(dir);

	*result = resultTemp;

    return 1;
}


#include "./Scripting/include/JavaScript/Core/Wrappers.h"
#include "./Scripting/include/JavaScript/Data/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"
#include "./Scripting/include/JavaScript/Performance/Wrappers.h"
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Communication/Wrappers.h"
#include "./Scripting/include/JavaScript/Pipeline/Wrappers.h"

OPint setup = 0;
Persistent<Object, CopyablePersistentTraits<Object> > exports;

Handle<Object> _OPscriptV8WrapEngine() {

    if (!setup) {

		Handle<Object> Wrapper = JS_NEW_OBJECT();

        Handle<Object> OP = JS_NEW_OBJECT();

        OPscriptNodeWrapperCore(OP);
        OPscriptNodeWrapperData(OP);
        OPscriptNodeWrapperMath(OP);
        OPscriptNodeWrapperPerformance(OP);
        OPscriptNodeWrapperHuman(OP);
        OPscriptNodeWrapperCommunication(OP);
        OPscriptNodeWrapperPipeline(OP);

        //OPlog("Wrapped the engine.");

        //OPscript *result = NULL;
        //OPscriptLoad("Scripts/node_modules/OPengine/OPgameState.js", &result);
        //OPlog("Loaded OPgameState.js Script.");
        //JS_SET_OBJECT(tmp, "gameState", _OPjavaScriptRequireScript(result, NULL));

        //OPlog("Wrapped OPgameState.");

		// If the application specified a Custom Wrapper
		// Then we call it and it sets anyrthing it needs
		// onto the Wrapper obj
		if(OPJAVASCRIPTV8_CUSTOMWRAPPER != NULL) {
			OPJAVASCRIPTV8_CUSTOMWRAPPER(Wrapper);
		}

		JS_SET_OBJECT(Wrapper, "engine", OP);

        exports = Persistent<Object, CopyablePersistentTraits<Object> >(isolate, Wrapper);
        setup = 1;
    }

    return Local<Object>::New(isolate, exports);
}

JS_RETURN_VAL _platform(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    #if defined(OPIFEX_WINDOWS)
        JS_RETURN(JS_NEW_STRING("win32"));
    #elif defined(OPIFEX_OSX)
        JS_RETURN(JS_NEW_STRING("darwin"));
    #else
        JS_RETURN(JS_NEW_STRING("unix"));
    #endif

    JS_RETURN_NULL;
}

JS_RETURN_VAL _arch(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    #if defined(OPIFEX_OS64)
        JS_RETURN(JS_NEW_STRING("x64"));
    #else
        JS_RETURN(JS_NEW_STRING("x86"));
    #endif

    JS_RETURN_NULL;
}

OPint setupOS = 0;
Persistent<Object, CopyablePersistentTraits<Object> > exportsOS;
Handle<Object> _OPscriptV8WrapOS() {

    if (!setupOS) {

        Handle<Object> tmp = JS_NEW_OBJECT();

        JS_SET_METHOD(tmp, "platform", _platform);
        JS_SET_METHOD(tmp, "arch", _arch);

        exportsOS = Persistent<Object, CopyablePersistentTraits<Object> >(isolate, tmp);
        setupOS = 1;
    }

    return Local<Object>::New(isolate, exportsOS);
}

OPchar* _OPscriptV8NormalizePath(const OPchar* path, bool assumeModule) {

    // Get a copy of the current argument path that we can use to alter it
    OPchar* pathCopy = OPstringCopy(path);
    OPchar* pathToLoad = pathCopy;

    const OPchar* begin = "./";
    const OPchar* jsEnd = ".js";
    const OPchar* opsEnd = ".ops";

    // If the require starts with a ./ then remove it
    if(OPstringStartsWith(pathToLoad, "./")) {
        OPchar* temp = OPstringCopy(&pathToLoad[2]);
        OPfree(pathToLoad);
        pathToLoad = temp;
    }

    // If the require doesn't end with a .js or a .ops then load from node_modules

    if (assumeModule && !OPstringEndsWith(pathToLoad, jsEnd) && !OPstringEndsWith(pathToLoad, opsEnd)) {
        OPchar* start = OPstringCreateMerged("node_modules/", pathToLoad);
        OPchar* result = OPstringCreateMerged(start, "/index.js");
        OPfree(start);
        OPfree(pathToLoad);
        pathToLoad = result;
    }


    return pathToLoad;
}


void _OPscriptV8Require(const v8::FunctionCallbackInfo<v8::Value>& args) {
	SCOPE_AND_ISOLATE;

	if (args.Length() == 0 || !args[0]->IsString()) return;

	v8::String::Utf8Value utf8(args[0]);
	const char *arg0 = *utf8;

	//OPlog("Requiring: %s", arg0);

	if (OPstringEquals(arg0, "OP")) {
		args.GetReturnValue().Set(_OPscriptV8WrapEngine());
		return;
	}

	if (OPstringEquals(arg0, "os")) {
		args.GetReturnValue().Set(_OPscriptV8WrapOS());
		return;
	}

	Handle<Object> currGlobal = args.Callee()->CreationContext()->Global();
	Handle<Value> __dirname = currGlobal->Get(JS_NEW_STRING("__dirname"));

	String::Utf8Value str(__dirname->ToString());
	//OPlog("__dirname %s", *str);

	if (OPJAVASCRIPTV8_REQUIRE == NULL || !OPJAVASCRIPTV8_REQUIRE(args)) {
		OPchar *pathToLoad = _OPscriptV8NormalizePath(arg0, true);
		Handle<Value> result;
		if (_OPjavaScriptRequire(pathToLoad, &result)) {
			args.GetReturnValue().Set(result);
		}
		OPfree(pathToLoad);
	}

}


void _OPscriptV8Load(const v8::FunctionCallbackInfo<v8::Value>& args) {
	SCOPE_AND_ISOLATE;

	if (args.Length() == 0 || !args[0]->IsString()) return;

	v8::String::Utf8Value utf8(args[0]);
	const char *arg0 = *utf8;

	//OPlog("Load: %s", arg0);

	Handle<Object> currGlobal = args.Callee()->CreationContext()->Global();
	Handle<Value> __dirname = currGlobal->Get(JS_NEW_STRING("__dirname"));

	String::Utf8Value str(__dirname->ToString());
	//OPlog("__dirname %s", *str);

	if (OPJAVASCRIPTV8_REQUIRE == NULL || !OPJAVASCRIPTV8_REQUIRE(args)) {
		OPchar *pathToLoad = _OPscriptV8NormalizePath(arg0, false);
		Handle<Value> result;
		if (_OPjavaScriptRequire(pathToLoad, &result)) {
			args.GetReturnValue().Set(result);
		}
		OPfree(pathToLoad);
	}

}

void _OPscriptV8SetImmediate(const v8::FunctionCallbackInfo<v8::Value>& args) {
    SCOPE_AND_ISOLATE;

    if (args.Length() == 0) return;

    Handle<Object> obj = JS_NEW_OBJECT();
    Handle<Value> argv[1] = { JS_NEW_BOOL(true) };
    Local<Function> callback = Local<Function>::Cast(args[0]);
    callback->Call(obj, 1, argv);
}

OPint OPjavaScriptV8CompileOLD(OPjavaScriptV8Compiled* compiled, OPscript* script, const OPchar* dir) {
	TIMED_BLOCK;

	ASSERT(isolate != NULL, "V8 Engine must be initialized first.");
	//OPlog("compiling");

	SCOPE_AND_ISOLATE;

	Handle<ObjectTemplate> global = JS_NEW_OBJECT_TEMPLATE();
	global->Set(JS_NEW_STRING("module"), JS_NEW_OBJECT_TEMPLATE());
	global->Set(JS_NEW_STRING("global"), JS_NEW_OBJECT_TEMPLATE());
	global->Set(JS_NEW_STRING("require"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8Require));
	global->Set(JS_NEW_STRING("load"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8Load));
	global->Set(JS_NEW_STRING("setImmediate"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8SetImmediate));
	_OPscriptV8SetConsole(global);

	//OPlog("Wrapped the things");

	Handle<Context> localContext = Context::New(isolate, NULL, global);

	v8::Context::Scope context_scope(localContext);

	//OPlog("Created the context");


	Local<Object> globalSelf = localContext->Global();
	if (dir == NULL) {
		JS_SET_STRING(globalSelf, "__dirname", "");
	}
	else {
		JS_SET_STRING(globalSelf, "__dirname", dir);
	}

	//OPlog("Now for the source");
	//OPlog("%s", script->filename);
	//OPlog("%s", script->data);

	Handle<String> source = JS_NEW_STRING(script->data);

	TryCatch trycatch;
	v8::ScriptOrigin origin(JS_NEW_STRING(script->filename));
	Local<Script> compiledV8 = v8::Script::Compile(source, &origin);
	//OPlog("Compile was attempted");
	if (compiledV8.IsEmpty()) {
		ReportException(isolate, &trycatch);
		return 0;
	}

	OPjavaScriptV8Compiled result;
	result.Source = script;
	result.Script = Persistent<Script, CopyablePersistentTraits<Script> >(isolate, compiledV8),
		result.Context = Persistent<Context, CopyablePersistentTraits<Context> >(isolate, localContext);

	*compiled = result;
	return 1;
}

OPint OPjavaScriptV8Compile(OPjavaScriptV8Compiled* compiled, OPscript* script, const OPchar* dir) {
	TIMED_BLOCK;

	ASSERT(isolate != NULL, "V8 Engine must be initialized first.");
	//OPlog("compiling");

	SCOPE_AND_ISOLATE;

	//Handle<ObjectTemplate> global = JS_NEW_OBJECT_TEMPLATE();
	//global->Set(JS_NEW_STRING("module"), JS_NEW_OBJECT_TEMPLATE());
	//global->Set(JS_NEW_STRING("global"), JS_NEW_OBJECT_TEMPLATE());
	//global->Set(JS_NEW_STRING("require"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8Require));
	//global->Set(JS_NEW_STRING("load"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8Load));
 //   global->Set(JS_NEW_STRING("setImmediate"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8SetImmediate));
	//_OPscriptV8SetConsole(global);

	////OPlog("Wrapped the things");

	//Handle<Context> localContext = Context::New(isolate, NULL, global);

	Handle<Context> localContext = Local<Context>::New(isolate, V8CONTEXT);
	v8::Context::Scope context_scope(localContext);

	//OPlog("Created the context");


    Local<Object> globalSelf = localContext->Global();
    if(dir == NULL) {
        JS_SET_STRING(globalSelf, "__dirname", "");
    } else {
        JS_SET_STRING(globalSelf, "__dirname", dir);
    }

	//OPlog("Now for the source");
	//OPlog("%s", script->filename);
	//OPlog("%s", script->data);

	Handle<String> source = JS_NEW_STRING(script->data);

	TryCatch trycatch;
	v8::ScriptOrigin origin(JS_NEW_STRING(script->filename));
	Local<Script> compiledV8 = v8::Script::Compile(source, &origin);
	//OPlog("Compile was attempted");
	if (compiledV8.IsEmpty()) {
		ReportException(isolate, &trycatch);
		return 0;
	}

	OPjavaScriptV8Compiled result;
	result.Source = script;
	result.Script = Persistent<Script, CopyablePersistentTraits<Script> >(isolate, compiledV8),
	result.Context = Persistent<Context, CopyablePersistentTraits<Context> >(isolate, localContext);

	*compiled = result;
	return 1;
}

void OPjavaScriptV8Update(OPjavaScriptV8Compiled* scriptCompiled) {
//    if (scriptCompiled->Source->changed) {
//        OPjavaScriptV8Compiled temp;
//        if (OPjavaScriptV8Compiled(&temp, scriptCompiled->Source, &scriptCompiled->Context)) {
//            *scriptCompiled = temp;
//            _runCompiled(scriptCompiled);
//        }
//        scriptCompiled->Source->changed = 0;
//    }
}

OPjavaScriptPersistentValue OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled) {
    SCOPE_AND_ISOLATE;

    Handle<Context> context = Local<Context>::New(isolate, scriptCompiled->Context);
    v8::Context::Scope context_scope(context);

    Handle<Script> compiled = Local<Script>::New(isolate, scriptCompiled->Script);

    TryCatch trycatch;
    Local<Value> result = compiled->Run();
    if (result.IsEmpty()) {
		ReportException(isolate, &trycatch);
        return Persistent<Value>(isolate, JS_NEW_NULL());
    }

    return Persistent<Value>(isolate, result);
}

OPjavaScriptPersistentValue OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled, const OPchar* name) {
    SCOPE_AND_ISOLATE;

    Handle<Context> context = Local<Context>::New(isolate, scriptCompiled->Context);
    v8::Context::Scope context_scope(context);
    Handle<v8::Object> global = context->Global();
    Handle<v8::Value> value = global->Get(JS_NEW_STRING(name));
    Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);

	TryCatch trycatch;
	Handle<Value> result = func->Call(global, 0, NULL);
    if (result.IsEmpty()) {
		ReportException(isolate, &trycatch);
        return Persistent<Value>(isolate, JS_NEW_NULL());
    }

    return Persistent<Value>(isolate, result);

//    Handle<Value> values[10];
//    for (OPint i = 0; i < count; i++) {
//        values[i] = Local<Value>::New(OPSCRIPTV8_ISOLATE, args[i]);
//    }

}

OPjavaScriptPersistentValue OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled, const OPchar* name, ui32 count, void** args) {
	SCOPE_AND_ISOLATE;

	Handle<Context> context = Local<Context>::New(isolate, scriptCompiled->Context);
	v8::Context::Scope context_scope(context);
	Handle<v8::Object> global = context->Global();
	Handle<v8::Value> value = global->Get(JS_NEW_STRING(name));
	Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);

	Handle<Value> values[10];
	for (ui32 i = 0; i < count; i++) {
		Local<Object> obj = JS_NEW_OBJECT();
		JS_SET_PTR(obj, args[i]);
		values[i] = obj;
		// Local<Object>::New(isolate, args[i]);
	}

	TryCatch trycatch;
	Handle<Value> result = func->Call(global, count, values);
	if (result.IsEmpty()) {
		ReportException(isolate, &trycatch);
		return Persistent<Value>(isolate, JS_NEW_NULL());
	}

	return Persistent<Value>(isolate, result);
}

OPjavaScriptPersistentValue OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled, const OPchar* name, ui32 count, Handle<Value>* args) {
	SCOPE_AND_ISOLATE;

	Handle<Context> context = Local<Context>::New(isolate, scriptCompiled->Context);
	v8::Context::Scope context_scope(context);
	Handle<v8::Object> global = context->Global();
	Handle<v8::Value> value = global->Get(JS_NEW_STRING(name));
	Handle<v8::Function> func = v8::Handle<v8::Function>::Cast(value);

	TryCatch trycatch;
	Handle<Value> result = func->Call(global, count, args);
	if (result.IsEmpty()) {
		ReportException(isolate, &trycatch);
		return Persistent<Value>(isolate, JS_NEW_NULL());
	}

	return Persistent<Value>(isolate, result);
}

void OPjavaScriptV8SetupRun(const OPchar* script) {
    OPjavaScriptV8Init();
    OPscript *result = NULL;
	OPstream* str = OPfile::ReadFromFile(script);
	ASSERT(str != NULL, "File couldn't be found");
	ASSERT(str->Source != NULL, "Filename wasn't set.");
	//OPlog("Script read");
	OPscriptLoad(str, &result);
	//OPlog("Script loaded");
    OPjavaScriptV8Compiled compiled;
    OPjavaScriptV8Compile(&compiled, result);
	//OPlog("Script compiled");
    OPjavaScriptV8Run(&compiled);
	//OPlog("Script run");
}

OPint OPjavaScriptV8Compiled::Compile(const OPchar* path) {
	return OPjavaScriptV8Compile(this, path);
}

OPint OPjavaScriptV8Compiled::Execute() {
	ScriptResult = OPjavaScriptV8Run(this);
	return 1;
}

OPjavaScriptPersistentValue OPjavaScriptV8Compiled::Function(const OPchar* name, ui32 count, void** args) {
	return OPjavaScriptV8Run(this, name, count, args);
}

OPjavaScriptPersistentValue OPjavaScriptV8Compiled::Function(const OPchar* name, ui32 count, Handle<Value>* args) {
	return OPjavaScriptV8Run(this, name, count, args);
}

void OPjavaScriptV8Init() {
	if (isolate == NULL) {
		bool result = v8::V8::InitializeICU();

        // Not sure why, but the latest update of V8 is returning
        // false every time.
		//ASSERT(result, "Make sure icudt.dll is available");
		v8::Platform* platform = v8::platform::CreateDefaultPlatform();
		v8::V8::InitializePlatform(platform);
		result = v8::V8::Initialize();
		ASSERT(result, "FAILED to initialize the V8 engine");

		Isolate::CreateParams create_params;
		create_params.array_buffer_allocator = &allocator;
		isolate = Isolate::New(create_params);

		HandleScope scope(isolate);

		Handle<ObjectTemplate> global = JS_NEW_OBJECT_TEMPLATE();
		global->Set(JS_NEW_STRING("module"), JS_NEW_OBJECT_TEMPLATE());
		global->Set(JS_NEW_STRING("global"), JS_NEW_OBJECT_TEMPLATE());
		global->Set(JS_NEW_STRING("require"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8Require));
		global->Set(JS_NEW_STRING("load"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8Load));
		global->Set(JS_NEW_STRING("setImmediate"), JS_NEW_FUNCTION_TEMPLATE(_OPscriptV8SetImmediate));
		_OPscriptV8SetConsole(global);

		V8GLOBALTEMPLATE = Persistent<ObjectTemplate, CopyablePersistentTraits<ObjectTemplate>>(isolate, global);

		//OPlog("Wrapped the things");

		Handle<Context> localContext = Context::New(isolate, NULL, global);
		V8CONTEXT = Persistent<Context, CopyablePersistentTraits<Context>>(isolate, localContext);

		OPlogInfo("Javascript V8 engine Initialized");
	}
}


#endif
