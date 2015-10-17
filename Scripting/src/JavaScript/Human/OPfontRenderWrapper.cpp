// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"
#include "./Data/include/OPcman.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"

JS_RETURN_VAL _OPfontRenderBegin(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontManager* fontManager = JS_GET_ARG_PTR(args, 0, OPfontManager);
    OPfontRenderBegin(fontManager);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontRenderColor(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontColor(OPvec4Create(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue(), 1.0));

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontRender(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    String::Utf8Value str(args[0]->ToString());
    OPvec2 pos = OPvec2Create(args[1]->NumberValue(), args[2]->NumberValue());
    OPfontRender(*str, pos);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPfontRenderEnd(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPfontRenderEnd();

    JS_RETURN_NULL;
}

void OPfontRenderWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Local<FunctionTemplate> tpl = JS_NEW_FUNCTION_TEMPLATE(_OPfontRender);

    Handle<Object> result = tpl->GetFunction();
    JS_SET_METHOD(result, "Begin", _OPfontRenderBegin);
    JS_SET_METHOD(result, "Color", _OPfontRenderColor);
    JS_SET_METHOD(result, "End", _OPfontRenderEnd);
    JS_SET_OBJECT(exports, "fontRender", result);

}

#endif