// myobject.cc
#include "./Scripting/include/JavaScript/Human/Wrappers.h"
#include "./Scripting/include/JavaScript/Math/Wrappers.h"
#include "./Data/include/OPstring.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

#include "./Human/Human.h"


JS_RETURN_VAL _OPmaterialClearParams(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);
	material->ClearParams();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmaterialClearParamsSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_PTR(args.This(), OPmaterial);
	material->ClearParams();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmaterialInit(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);
    OPeffect* effect = JS_GET_ARG_PTR(args, 1, OPeffect);
	material->Init(effect);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmaterialAddParam(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);

    OPmaterialParamType matParamType = (OPmaterialParamType)(OPint)args[1]->NumberValue();

    String::Utf8Value str(args[2]->ToString());

    void* data = (void*)(OPint)args[3]->NumberValue();

    ui8 count = (ui8)args[4]->NumberValue();

    OPchar* c = OPstringCopy(*str);

    material->AddParam(matParamType, c, data, count);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmaterialAddParamSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_PTR(args.This(), OPmaterial);

    OPmaterialParamType matParamType = (OPmaterialParamType)(OPint)args[0]->NumberValue();

    String::Utf8Value str(args[1]->ToString());

    void* data = JS_GET_ARG_PTR(args, 2, void);

    ui8 count = (ui8)args[3]->NumberValue();

    OPchar* c = OPstringCopy(*str);

    OPlog("DATA %p", data);

	material->AddParam(matParamType, c, data, count);

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmaterialBind(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);
	material->Bind();

    JS_RETURN_NULL;
}

JS_RETURN_VAL _OPmaterialBindSelf(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = JS_GET_ARG_PTR(args, 0, OPmaterial);
	material->Bind();

    JS_RETURN_NULL;
}

void OPmaterialWrapperCreate(Handle<Object> result, OPmaterial* material) {
    SCOPE_AND_ISOLATE

	
    JS_SET_PTR(result, material);
    JS_SET_METHOD(result, "ClearParams", _OPmaterialClearParamsSelf);
    JS_SET_METHOD(result, "AddParam", _OPmaterialAddParamSelf);
    JS_SET_METHOD(result, "Bind", _OPmaterialBindSelf);
    JS_SET_NUMBER(result, "size", sizeof(OPmaterial));
}

JS_RETURN_VAL _OPmaterialCreate(const JS_ARGS& args) {
    SCOPE_AND_ISOLATE;

    OPmaterial* material = (OPmaterial*)OPalloc(sizeof(OPmaterial));
    OPeffect* effect = JS_GET_ARG_PTR(args, 0, OPeffect);
    material->Init(effect);

    Handle<Object> result = JS_NEW_OBJECT();
    OPmaterialWrapperCreate(result, material);

    JS_RETURN(result);
}

void OPmaterialWrapper(Handle<Object> exports) {
    SCOPE_AND_ISOLATE;

    Handle<Object> material = JS_NEW_OBJECT();
    JS_SET_METHOD(material, "ClearParams", _OPmaterialClearParams);
    JS_SET_METHOD(material, "Init", _OPmaterialInit);
    JS_SET_METHOD(material, "Create", _OPmaterialCreate);
    JS_SET_METHOD(material, "AddParam", _OPmaterialAddParam);
    JS_SET_METHOD(material, "Bind", _OPmaterialBind);
    JS_SET_NUMBER(material, "size", sizeof(OPmaterial));

    Handle<Object> ParamType = JS_NEW_OBJECT();
    JS_SET_NUMBER(ParamType, "MATRIX4", OPmaterialParamType::MATRIX4);
    JS_SET_NUMBER(ParamType, "MATRIX4V", OPmaterialParamType::MATRIX4V);
    JS_SET_NUMBER(ParamType, "TEXTURE", OPmaterialParamType::TEXTURE);
    JS_SET_NUMBER(ParamType, "VECTOR3", OPmaterialParamType::VECTOR3);
    JS_SET_NUMBER(ParamType, "VECTOR4", OPmaterialParamType::VECTOR4);
    JS_SET_OBJECT(material, "ParamType", ParamType);

    JS_SET_OBJECT(exports, "material", material);

}

#endif
