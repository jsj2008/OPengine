#ifndef OPENGINE_HUMAN_RENDERING_VERTEX_BUFFER
#define OPENGINE_HUMAN_RENDERING_VERTEX_BUFFER

struct OPvertexBuffer;
typedef struct OPvertexBuffer OPvertexBuffer;

#include "./Core/include/OPtypes.h"
#include "./Human/include/Rendering/OPrender.h"

struct OPvertexBuffer {
	void* internalPtr;
	OPuint ElementCount;
	ui32 ElementSize;

	inline OPvertexBuffer* Init() {
		return OPRENDERER_ACTIVE->VertexBuffer.Init(this);
	}

	inline void SetData(ui32 elementSize, OPuint count, const void* data) {
		OPRENDERER_ACTIVE->VertexBuffer.SetData(this, elementSize, count, data);
	}

	inline void SetLayout(OPvertexLayout* vertexLayout) {
		OPRENDERER_ACTIVE->VertexBuffer.SetLayout(this, vertexLayout);
	}

	inline void Bind() {
		OPRENDERER_ACTIVE->VertexBuffer.Bind(this);
	}

	inline void Unbind() {
		OPRENDERER_ACTIVE->VertexBuffer.Unbind(this);
	}

	inline void Destroy() {
		OPRENDERER_ACTIVE->VertexBuffer.Destroy(this);
	}
};

inline OPvertexBuffer* OPvertexBufferCreate() {
	return OPRENDERER_ACTIVE->VertexBuffer.Create();
}

#endif