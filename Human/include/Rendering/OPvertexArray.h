#ifndef OPENGINE_HUMAN_RENDERING_VERTEX_ARRAY
#define OPENGINE_HUMAN_RENDERING_VERTEX_ARRAY

struct OPvertexArray;
typedef struct OPvertexArray OPvertexArray;

#include "./Human/include/Rendering/OPrender.h"
#include "./Human/include/Rendering/OPvertexLayout.h"

struct OPvertexArray {
	void* internalPtr;
	//OPvertexLayout* vertexLayout;

	inline OPvertexArray* Init(OPvertexLayout* vertexLayout) {
		return OPRENDERER_ACTIVE->VertexArray.Init(this, vertexLayout);
	}

	inline void Bind() {
		OPRENDERER_ACTIVE->VertexArray.Bind(this);
	}
};

#endif