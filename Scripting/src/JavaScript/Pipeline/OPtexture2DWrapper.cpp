// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Pipeline/Pipeline.h"

JS_RETURN_VAL _OPtexture2DCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

    OPtexture* texture = JS_GET_ARG_PTR(args, 0, OPtexture);
    OPeffect* effect = NULL;

    if(args.Length() > 1) {
      effect = JS_GET_ARG_PTR(args, 1, OPeffect);
    }

	OPtexture2DOLD* tex2d;

    if(args.Length() > 2) {
        tex2d = OPtexture2DCreate(texture, effect, OPvec2((f32)args[2]->NumberValue(), (f32)args[3]->NumberValue()), OPvec2((f32)args[4]->NumberValue(), (f32)args[5]->NumberValue()));
    } else {
        tex2d = OPtexture2DCreate(texture, effect);
    }

    Handle<Object> result = JS_NEW_OBJECT();
    JS_SET_PTR(result, tex2d);

    Handle<Object> Scale = JS_NEW_OBJECT();
    OPvec2WrapperCreate(Scale, &tex2d->Scale);
    JS_SET_OBJECT(result, "Scale", Scale);

    Handle<Object> Position = JS_NEW_OBJECT();
    OPvec2WrapperCreate(Position, &tex2d->Position);
    JS_SET_OBJECT(result, "Position", Position);


    JS_RETURN(result);
}

JS_RETURN_VAL _OPtexture2DDestroy(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

		OPtexture2DOLD* tex2d = JS_GET_ARG_PTR(args, 0, OPtexture2DOLD);
    OPtexture2DDestroy(tex2d);

    JS_RETURN_NULL
}

JS_RETURN_VAL _OPtexture2DPrepRender(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

		OPtexture2DOLD* tex2d = JS_GET_ARG_PTR(args, 0, OPtexture2DOLD);
    OPtexture2DPrepRender(tex2d);

    JS_RETURN_NULL
}

JS_RETURN_VAL _OPtexture2DRender(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE

		OPtexture2DOLD* tex2d = JS_GET_ARG_PTR(args, 0, OPtexture2DOLD);
    OPtexture2DRender(tex2d);

    JS_RETURN_NULL
}

void OPtexture2DWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> texture2D = JS_NEW_OBJECT();
    JS_SET_METHOD(texture2D, "Create", _OPtexture2DCreate);
    JS_SET_METHOD(texture2D, "Destroy", _OPtexture2DDestroy);
    JS_SET_METHOD(texture2D, "PrepRender", _OPtexture2DPrepRender);
    JS_SET_METHOD(texture2D, "Render", _OPtexture2DRender);
    JS_SET_NUMBER(texture2D, "size", sizeof(OPtexture2DOLD));
    JS_SET_OBJECT(exports, "texture2D", texture2D);
}

#endif
