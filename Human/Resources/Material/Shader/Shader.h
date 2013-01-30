#pragma once

#include "./Human/Resources/Resource.h"
#include "Core/include/Types.h"
#include "ShaderTypes.h"
#include "./Human/Resources/Texture/Texture2D.h"
#include "./Human/Math/Matrix4.h"

#ifdef OPIFEX_OPENGL_ES_2
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

class Shader : public Resource {
public:
	Shader(ShaderType shaderType, const char* source);
	static Shader* FromFile(ShaderType shaderType, const char* file);
};

typedef Shader* ShaderPtr;