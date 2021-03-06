#pragma once

struct OPvertexArrayAPI;
typedef struct OPvertexArrayAPI OPvertexArrayAPI;

#include "./Core/include/OPtypes.h"
struct OPvertexArray;
struct OPvertexLayout;

struct OPvertexArrayAPI {
	OPvertexArray*(*Create)(OPvertexLayout* vertexLayout) = 0;
	OPvertexArray*(*Init)(OPvertexArray* vertexArray, OPvertexLayout* vertexLayout) = 0;
	void(*Bind)(OPvertexArray* ptr) = 0;
	void(*SetLayout)(OPvertexArray* vertexArray, OPvertexLayout* vertexLayout) = 0;
	void(*_Draw)(OPvertexArray* ptr, OPuint count, OPuint offset) = 0;
	void(*_DrawIndexed)(OPvertexArray* ptr, OPuint count, OPuint offset) = 0;
	void(*Unbind)(OPvertexArray* ptr) = 0;
	void(*Destroy)(OPvertexArray* vertexArray) = 0;

	inline void Draw(OPvertexArray* ptr, OPuint count, OPuint offset) {
		_Draw(ptr, count, offset);
	}
	inline void Draw(OPvertexArray* ptr, OPuint count) {
		_Draw(ptr, count, 0);
	}

	inline void DrawIndexed(OPvertexArray* ptr, OPuint count, OPuint offset) {
		_DrawIndexed(ptr, count, offset);
	}
	inline void DrawIndexed(OPvertexArray* ptr, OPuint count) {
		_DrawIndexed(ptr, count, 0);
	}
};