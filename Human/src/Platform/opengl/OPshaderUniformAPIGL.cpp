#include "./Human/include/Platform/opengl/OPshaderUniformAPIGL.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Platform/opengl/OPeffectAPIGL.h"
#include "./Human/include/Rendering/OPeffect.h"
#include "./Human/include/Rendering/OPshaderUniform.h"
#include "./Core/include/Assert.h"

OPshaderUniform* OPshaderUniformGLInit(OPshaderUniform* shaderUniform, OPeffect* effect, const OPchar* name) {
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)OPalloc(sizeof(OPshaderUniformGL));;
	shaderUniform->internalPtr = shaderUniformGL;

	OPeffectGL* effectGL = (OPeffectGL*)effect->internalPtr;


	GLint result = glGetUniformLocation(effectGL->Handle, name);
	if (result > -1) {
		shaderUniformGL->Handle = result;
	}

	shaderUniform->Found = result > -1;

	return shaderUniform;
}

OPshaderUniform* OPshaderUniformGLCreate(OPeffect* effect, const OPchar* name) {
	OPshaderUniform* shaderUniform = (OPshaderUniform*)OPalloc(sizeof(OPshaderUniform));
	return OPshaderUniformGLInit(shaderUniform, effect, name);
}

void OPshaderUniformGLDestroy(OPshaderUniform* shaderUniform) {
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	OPfree(shaderUniformGL);
	shaderUniform->internalPtr = NULL;
}

void OPshaderUniformSetBoolGL(OPshaderUniform* shaderUniform, bool val) {
#ifdef _DEBUG
	if (shaderUniform == NULL) return;
#endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform1i(shaderUniformGL->Handle, val);
}
void OPshaderUniformSetFGL(OPshaderUniform* shaderUniform, f32 val) {
#ifdef _DEBUG
	if (shaderUniform == NULL) return;
#endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform1f(shaderUniformGL->Handle, val);
}
void OPshaderUniformSetFvGL(OPshaderUniform* shaderUniform, ui32 count, f32* val) {
#ifdef _DEBUG
	if (shaderUniform == NULL) return;
#endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform1fv(shaderUniformGL->Handle, count, val);
}
void OPshaderUniformSetIGL(OPshaderUniform* shaderUniform, i32 val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform1i(shaderUniformGL->Handle, val);
}
void OPshaderUniformSetIvGL(OPshaderUniform* shaderUniform, ui32 count, i32* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform1iv(shaderUniformGL->Handle, count, val);
}
void OPshaderUniformSetVec2GL(OPshaderUniform* shaderUniform, OPvec2* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform2fv(shaderUniformGL->Handle, 1, (OPfloat*)val);
}
void OPshaderUniformSetVec2vGL(OPshaderUniform* shaderUniform, ui32 count, OPvec2* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform2fv(shaderUniformGL->Handle, count, (OPfloat*)val);
}
void OPshaderUniformSetVec3GL(OPshaderUniform* shaderUniform, OPvec3* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform3fv(shaderUniformGL->Handle, 1, (OPfloat*)val);
}
void OPshaderUniformSetVec3vGL(OPshaderUniform* shaderUniform, ui32 count, OPvec3* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform3fv(shaderUniformGL->Handle, count, (OPfloat*)val);
}
void OPshaderUniformSetVec4GL(OPshaderUniform* shaderUniform, OPvec4* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform4fv(shaderUniformGL->Handle, 1, (OPfloat*)val);
}
void OPshaderUniformSetVec4vGL(OPshaderUniform* shaderUniform, ui32 count, OPvec4* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniform4fv(shaderUniformGL->Handle, count, (OPfloat*)val);
}
void OPshaderUniformSetMat3GL(OPshaderUniform* shaderUniform, OPmat3* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniformMatrix3fv(shaderUniformGL->Handle, 1, GL_FALSE, (f32*)val);
}
void OPshaderUniformSetMat3vGL(OPshaderUniform* shaderUniform, ui32 count, OPmat3* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniformMatrix3fv(shaderUniformGL->Handle, count, GL_FALSE, (f32*)val);
}
void OPshaderUniformSetMat4GL(OPshaderUniform* shaderUniform, OPmat4* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniformMatrix4fv(shaderUniformGL->Handle, 1, GL_FALSE, (f32*)val);
}
void OPshaderUniformSetMat4vGL(OPshaderUniform* shaderUniform, ui32 count, OPmat4* val) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	glUniformMatrix4fv(shaderUniformGL->Handle, count, GL_FALSE, (f32*)val);
}
void OPshaderUniformSetTextureGL(OPshaderUniform* shaderUniform, OPtexture* val, ui32 slot) {
    #ifdef _DEBUG
        if(shaderUniform == NULL) return;
	#else
		if (shaderUniform == NULL) {
			OPlogErr("Shader Uniform was NULL");
			return;
		}
    #endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	val->Bind(slot);
	glUniform1i(shaderUniformGL->Handle, slot);
}
void OPshaderUniformSetTexturevGL(OPshaderUniform* shaderUniform, ui32 count, OPtexture* val, ui32 slot) {
	ASSERT(true, "NOT SUPPORTED YET");
}
void OPshaderUniformSetTextureCubeGL(OPshaderUniform* shaderUniform, OPtextureCube* val, ui32 slot) {
	#ifdef _DEBUG
		if (shaderUniform == NULL) return;
	#endif
	OPshaderUniformGL* shaderUniformGL = (OPshaderUniformGL*)shaderUniform->internalPtr;
	val->Bind(slot);
	glUniform1i(shaderUniformGL->Handle, slot);

}
void OPshaderUniformSetTextureCubevGL(OPshaderUniform* shaderUniform, ui32 count, OPtextureCube* val, ui32 slot) {
	ASSERT(true, "NOT SUPPORTED YET");
}

void OPshaderUniformAPIGLInit(OPshaderUniformAPI* shaderUniform) {
	shaderUniform->Create = OPshaderUniformGLCreate;
	shaderUniform->Init = OPshaderUniformGLInit;
	shaderUniform->Destroy = OPshaderUniformGLDestroy;

	shaderUniform->SetBool = OPshaderUniformSetBoolGL;
	shaderUniform->SetF = OPshaderUniformSetFGL;
	shaderUniform->SetFv = OPshaderUniformSetFvGL;
	shaderUniform->SetI = OPshaderUniformSetIGL;
	shaderUniform->SetIv = OPshaderUniformSetIvGL;
	shaderUniform->SetVec2 = OPshaderUniformSetVec2GL;
	shaderUniform->SetVec2v = OPshaderUniformSetVec2vGL;
	shaderUniform->SetVec3 = OPshaderUniformSetVec3GL;
	shaderUniform->SetVec3v = OPshaderUniformSetVec3vGL;
	shaderUniform->SetVec4 = OPshaderUniformSetVec4GL;
	shaderUniform->SetVec4v = OPshaderUniformSetVec4vGL;
	shaderUniform->SetMat3 = OPshaderUniformSetMat3GL;
	shaderUniform->SetMat3v = OPshaderUniformSetMat3vGL;
	shaderUniform->SetMat4 = OPshaderUniformSetMat4GL;
	shaderUniform->SetMat4v = OPshaderUniformSetMat4vGL;
	shaderUniform->SetTexture = OPshaderUniformSetTextureGL;
	shaderUniform->SetTexturev = OPshaderUniformSetTexturevGL;
	shaderUniform->SetTextureCube = OPshaderUniformSetTextureCubeGL;
	shaderUniform->SetTextureCubev = OPshaderUniformSetTextureCubevGL;

}
