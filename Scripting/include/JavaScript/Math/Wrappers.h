#ifndef OPENGINE_SCRIPTING_NODE_MATH_WRAPPERS
#define OPENGINE_SCRIPTING_NODE_MATH_WRAPPERS

#include "./Scripting/include/JavaScript/OPjavaScriptHelper.h"

#if defined(OPIFEX_OPTION_NODEJS) || defined(OPIFEX_OPTION_V8)

void OPscriptNodeWrapperMath(Handle<Object> target);

void OPvec2Wrapper(Handle<Object> exports);
void OPvec3Wrapper(Handle<Object> exports);
void OPvec4Wrapper(Handle<Object> exports);
void OPmat4Wrapper(Handle<Object> exports);

#include "./Math/include/OPvec3.h"

Handle<Object> OPvec3WrapperSetup(Handle<Object> result, OPvec3* ptr);

#define OPVEC3WRAPPERSETUP(target, name, ptr) JS_SET_OBJECT(target, name, OPvec3WrapperSetup(JS_NEW_OBJECT(), ptr));

#endif

#endif