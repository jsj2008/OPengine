#include "./Scripting/include/Node/OPscriptNodeHelper.h"

#ifdef OPIFEX_OPTION_NODEJS

#include "./Scripting/include/Node/OPscriptNodeWrapperData.h"

#include "./Data/Data.h"
#include "./Core/include/OPlog.h"


NODE_RETURN_VAL _OPcmanInit(const NODE_ARGS& args) {
    SCOPE_AND_ISOLATE

 	const char* assetDir = NULL;

 	if (args.Length() > 0) {
 		v8::String::Utf8Value utf8(args[0]);
        assetDir = *utf8;
        OPlog("Assets %s", assetDir);
    }

    OPcmanInit(assetDir);

    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPcmanLoad(const NODE_ARGS& args) {
 	SCOPE_AND_ISOLATE

    if(args.Length() > 0) {
        v8::String::Utf8Value utf8(args[0]);
        const char* file = *utf8;
        OPint result = OPcmanLoad(file);

        NODE_RETURN(NODE_NEW_NUMBER(result));
 	}
    NODE_RETURN_NULL
 }

NODE_RETURN_VAL _OPcmanGet(const NODE_ARGS& args) {
 	SCOPE_AND_ISOLATE

    if(args.Length() > 0) {
        v8::String::Utf8Value utf8(args[0]);
        const char* file = *utf8;

        void* asset = OPcmanGet(file);

        Handle<Object> result = NODE_NEW_OBJECT();
        NODE_SET_PTR(result, asset);

        NODE_RETURN(result);
 	}
    NODE_RETURN_NULL
}

NODE_RETURN_VAL _OPcmanLoadGet(const NODE_ARGS& args) {
 	SCOPE_AND_ISOLATE

    if(args.Length() > 0) {
        v8::String::Utf8Value utf8(args[0]);
        const char* file = *utf8;
        void* asset = OPcmanLoadGet(file);

        Handle<Object> result = NODE_NEW_OBJECT();
        NODE_SET_PTR(result, asset);

        NODE_RETURN(result);
 	}
    NODE_RETURN_NULL
}

void OPscriptNodeWrapperData(Handle<Object> exports) {
    SCOPE_AND_ISOLATE

    {
        // OP.cman
        Handle<Object> cman = NODE_NEW_OBJECT();
        NODE_SET_METHOD(cman, "Init", _OPcmanInit);
        NODE_SET_METHOD(cman, "Load", _OPcmanLoad);
        NODE_SET_METHOD(cman, "Get", _OPcmanGet);
        NODE_SET_METHOD(cman, "LoadGet", _OPcmanLoadGet);
        NODE_SET_OBJECT(exports, "cman", cman);
    }
}

#endif