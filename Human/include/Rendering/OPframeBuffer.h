#pragma once

struct OPframeBuffer;
typedef struct OPframeBuffer OPframeBuffer;

#include "./Human/include/Rendering/OPtexture.h"
#include "OPrender.h"
#include "./Core/include/OPtypes.h"

extern OPframeBuffer* OPRENDER_CURR_FRAMEBUFFER;



struct OPframeBuffer {
	void* internalPtr;

	OPtexture* texture;
	ui32 count;
	OPtexture depthTexture;

	inline void Init(OPtextureDesc textureDesc) {
		OPRENDERER_ACTIVE->FrameBuffer.Init(this, textureDesc);
	}

	inline void Init(OPtextureDesc textureDesc, OPtextureDesc depthDesc) {
		OPRENDERER_ACTIVE->FrameBuffer.Init(this, textureDesc, depthDesc);
	}

	inline void Init(OPtextureDesc* textureDesc, ui32 count) {
		OPRENDERER_ACTIVE->FrameBuffer.Init(this, textureDesc, count);
	}

	inline void Init(OPtextureDesc textureDesc, OPtexture* depthTexture) {
		OPRENDERER_ACTIVE->FrameBuffer.Init(this, textureDesc, depthTexture);
	}

	inline void Init(OPtextureDesc* textureDesc, ui32 count, OPtexture* depthTexture) {
		OPRENDERER_ACTIVE->FrameBuffer.Init(this, textureDesc, count, depthTexture);
	}

	inline void Bind() {
		OPRENDERER_ACTIVE->FrameBuffer.Bind(this);
	}

	inline void Bind(OPframeBufferMode mode) {
		OPRENDERER_ACTIVE->FrameBuffer.Bind(mode, this);
	}

	inline void Destroy() {
		OPRENDERER_ACTIVE->FrameBuffer.Destroy(this);
	}

	static inline void Unbind() {
		OPRENDERER_ACTIVE->FrameBuffer.Unbind(OPframeBufferMode::BOTH);
	}

	static inline void Unbind(OPframeBufferMode mode) {
		OPRENDERER_ACTIVE->FrameBuffer.Unbind(mode);
	}
};