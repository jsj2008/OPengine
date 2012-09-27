#pragma once

#include "Human\Resources\Material\Shader\Shader.h"
#include "Human\Resources\Material\Shader\ShaderTypes.h"

	class GLESShader : public Shader{
	public:
		GLESShader();
		GLESShader(ShaderType shaderType, const char* file);
		~GLESShader();
		void load(ShaderType shaderType, const char* file);
	};