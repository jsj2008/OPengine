#include "./Human/include/Rendering/API/OPrenderer.h"
#include "./Human/include/Platform/opengl/OPcommonGL.h"
#include "./Human/include/Rendering/OPmonitor.h"
#include "./Human/include/Rendering/OPglew.h"
#include "./Human/include/Rendering/OPwindow.h"
#include "./Core/include/Assert.h"

// TODO: (garrett) CMake should be generating these variables and we'll check for just OPIFEX_OPENGL and use the OPIFEX_OPENGL_MAJOR and OPIFEX_OPENGL_MINOR
#if defined(OPIFEX_OPENGL_3_3)
#define OPIFEX_OPENGL 1
#define OPIFEX_OPENGL_MAJOR 3
#define OPIFEX_OPENGL_MINOR 3
#endif

void glfwErrorCallback(int error, const char* desc) {
	OPlogErr("GLFW ERROR: %s", desc);
}

i8 OPrendererInitGL(OPwindow* window) {
	OPlogInfo("Initializing OpenGL Renderer");

	if (OPglewInit() != 0) {
		OPlogErr("Failed to initialize GLEW");
		return -2;
	}

	glfwSetErrorCallback(glfwErrorCallback);

	OPint result = glfwInit();
	if (!result) {
		OPlogErr("Failed to initialize GLFW");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPIFEX_OPENGL_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPIFEX_OPENGL_MINOR);

#ifdef OPIFEX_OSX
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif


	return 0;
}

void OPrendererClearGL(OPvec4 color){
	glClearColor(color.x, color.y, color.z, color.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OPrendererPresentGL() {
	ASSERT(OPWINDOW_ACTIVE != NULL, "There must be an active window");
	glfwSwapBuffers(OPWINDOW_ACTIVE->Window);
}

void OPrendererSetDepthTestingGL(bool state) {
	if (state) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
}

void OPrendererSetDepthWriteGL(bool state){
	glDepthMask(state ? GL_TRUE : GL_FALSE);
}

void OPrendererSetCullGL(bool state) {
	if (state) {
		glEnable(GL_CULL_FACE);
	}
	else {
		glDisable(GL_CULL_FACE);
	}
}

void OPrendererSetCullModeGL(i8 state) {
	if (state) {
		glCullFace(GL_FRONT);
	}
	else {
		glCullFace(GL_BACK);
	}
}

void OPrendererSetBlendGL(bool state){
	if (state) {
		glEnable(GL_BLEND);
	}
	else {
		glDisable(GL_BLEND);
	}
}

void OPrendererSwapBufferGL() {
	ASSERT(OPWINDOW_ACTIVE != NULL, "There must be an active window");
	glfwSwapBuffers(OPWINDOW_ACTIVE->Window);
}

void OPrendererShutdownGL() {
	glfwTerminate();
}


#include "./Human/include/Platform/opengl/OPcontextGL.h"
#include "./Human/include/Platform/opengl/OPeffectAPIGL.h"
#include "./Human/include/Platform/opengl/OPframeBuffer2DGL.h"
#include "./Human/include/Platform/opengl/OPframeBufferDepthGL.h"
#include "./Human/include/Platform/opengl/OPindexBufferGL.h"
#include "./Human/include/Platform/opengl/OPshaderAPIGL.h"
#include "./Human/include/Platform/opengl/OPshaderResourceGL.h"
#include "./Human/include/Platform/opengl/OPshaderUniformAPIGL.h"
#include "./Human/include/Platform/opengl/OPtextureAPIGL.h"
#include "./Human/include/Platform/opengl/OPtexture2DGL.h"
#include "./Human/include/Platform/opengl/OPtextureCubeGL.h"
#include "./Human/include/Platform/opengl/OPtextureDepthGL.h"
#include "./Human/include/Platform/opengl/OPvertexArrayGL.h"
#include "./Human/include/Platform/opengl/OPvertexBufferAPIGL.h"

OPrenderer OPRENDERERGL;
OPrenderer* OPrendererGL() {
	OPRENDERERGL.Init = OPrendererInitGL;
	OPRENDERERGL._Clear = OPrendererClearGL;
	OPRENDERERGL.Present = OPrendererPresentGL;
	OPRENDERERGL.SetDepthTesting = OPrendererSetDepthTestingGL;
	OPRENDERERGL.SetDepthWrite = OPrendererSetDepthWriteGL;
	OPRENDERERGL.SetCull = OPrendererSetCullGL;
	OPRENDERERGL.SetCullMode = OPrendererSetCullModeGL;
	OPRENDERERGL.SetBlend = OPrendererSetBlendGL;
	OPRENDERERGL.SwapBuffer = OPrendererSwapBufferGL;
	OPRENDERERGL.Shutdown = OPrendererShutdownGL;

	OPcontextGLInit(&OPRENDERERGL.Context); 
	OPeffectAPIGLInit(&OPRENDERERGL.Effect);
	OPframeBuffer2DGLInit(&OPRENDERERGL.FrameBuffer2D);
	OPframeBufferDepthGLInit(&OPRENDERERGL.FrameBufferDepth);
	OPindexBufferAPIGLInit(&OPRENDERERGL.IndexBuffer);
	OPshaderAPIGLInit(&OPRENDERERGL.Shader);
	OPshaderResourceGLInit(&OPRENDERERGL.ShaderResource);
	OPshaderUniformAPIGLInit(&OPRENDERERGL.ShaderUniform);
	OPtextureAPIGLInit(&OPRENDERERGL.Texture);
	OPtexture2DGLInit(&OPRENDERERGL.Texture2D);
	OPtextureCubeGLInit(&OPRENDERERGL.TextureCube);
	OPtextureDepthGLInit(&OPRENDERERGL.TextureDepth);
	OPvertexArrayGLInit(&OPRENDERERGL.VertexArray);
	OPvertexBufferAPIGLInit(&OPRENDERERGL.VertexBuffer);

	return &OPRENDERERGL;
}