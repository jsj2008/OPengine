#pragma once

#include "./Core/include/OPtypes.h"
#include "./Scripting/include/OPloaderOPS.h"
#include "./Data/include/OPcman.h"

#ifdef OPIFEX_OPTION_V8

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

extern OPint(*OPJAVASCRIPTV8_REQUIRE)(FunctionCallbackInfo<Value>);
extern void(*OPJAVASCRIPTV8_CUSTOMWRAPPER)(Handle<Object>);
extern Persistent<Context, CopyablePersistentTraits<Context>> V8CONTEXT;
extern Persistent<ObjectTemplate, CopyablePersistentTraits<ObjectTemplate>> V8GLOBALTEMPLATE;

struct OPjavaScriptV8Compiled {
	OPscript* Source;
	OPjavaScriptPersistentScript Script;
	OPjavaScriptPersistentContext Context;
	OPjavaScriptPersistentValue ScriptResult;

	OPjavaScriptV8Compiled() {

	}
	OPjavaScriptV8Compiled(const OPchar* file) {
		CompileAndExecute(file);
	}

	OPint Compile(const OPchar* file);
	OPint Execute();
	OPjavaScriptPersistentValue Function(const OPchar* name, ui32 count, void** args);
	OPjavaScriptPersistentValue Function(const OPchar* name, ui32 count, Handle<Value>* args);

	inline OPjavaScriptPersistentValue Function(const OPchar* name) {
		Function(name, 0, (void**)NULL);
	}
	inline OPjavaScriptPersistentValue Function(const OPchar* name, void* arg) {
		Function(name, 1, (void**)arg);
	}

	inline OPint CompileAndExecute(const OPchar* file) {
		if (Compile(file)) {
			return Execute();
		}
		return 0;
	}
};

void OPjavaScriptV8Init();

OPint OPjavaScriptV8Compile(OPjavaScriptV8Compiled* compiled, OPscript* script, const OPchar* dir);
void OPjavaScriptV8Update(OPjavaScriptV8Compiled* scriptCompiled);
OPjavaScriptPersistentValue OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled);
OPjavaScriptPersistentValue OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled, const OPchar* name);
OPjavaScriptPersistentValue OPjavaScriptV8Run(OPjavaScriptV8Compiled* scriptCompiled, const OPchar* name, ui32 count, void** args);

void OPjavaScriptV8SetupRun(const OPchar* script);

inline OPint OPjavaScriptV8Compile(OPjavaScriptV8Compiled* compiled, OPscript* script) {
	return OPjavaScriptV8Compile(compiled, script, NULL);
}
inline OPint OPjavaScriptV8Compile(OPjavaScriptV8Compiled* compiled, const OPchar* path) {
	OPscript* script = (OPscript*)OPCMAN.LoadGet(path);
	return OPjavaScriptV8Compile(compiled, script);
}
#ifdef _DEBUG
	#define OPJAVASCRIPTV8_UPDATE(compiled) OPjavaScriptV8Update(compiled);
#else
	#define OPJAVASCRIPTV8_UPDATE(compiled)
#endif


#endif
