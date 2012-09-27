#include "GLMaterial.h"
#include <GL/glew.h>

void GLMaterial::load(ShaderPtr vertex, ShaderPtr fragment){
	m_handle = glCreateProgram();
	if(m_handle){
		glAttachShader(m_handle, (GLuint)vertex->handle());
		glAttachShader(m_handle, (GLuint)fragment->handle());
		glLinkProgram(m_handle);
	}
}

ui32 GLMaterial::attribute_location(const char* attribute){
	return glGetAttribLocation(m_handle, "vPosition");
}

void GLMaterial::set_data(ui32 attribute, ui32 size, bool normalize, ui32 stride, void* arrayData){
	glVertexAttribPointer((GLuint)attribute, size, GL_FLOAT, normalize, stride, arrayData);
}

void GLMaterial::enable_attrib(ui32 attribute){
	glEnableVertexAttribArray(attribute);
}

void GLMaterial::disable_attrib(ui32 attribute){
	glDisableVertexAttribArray(attribute);
}