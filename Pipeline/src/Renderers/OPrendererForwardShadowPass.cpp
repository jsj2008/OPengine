#include "./Pipeline/include/Renderers/OPrendererForwardShadowPass.h"


void OPrendererForwardShadowPass::Init(OPcam** cam) {
	OPrendererPass::Init(cam);	

	renderBucket.Init(1000, cam);
	
	const ui32 SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
	depthBuffer.Init(OPtextureDesc(SHADOW_WIDTH, SHADOW_HEIGHT, OPtextureFormat::DEPTH, OPtextureWrap::CLAMP_TO_BORDER, OPtextureFilter::NEAREST, OPtextureType::FLOAT));
	

	shadowMaterial.Init(OPNEW(OPeffect("Common/DepthRTT.vert", "Common/DepthRTT.frag")));
	shadowMaterial.cullFace = OPcullFace::FRONT;
	shadowMaterial.cull = shadowMaterial.depth = true;

	shadowSkinnedMaterial.Init(OPNEW(OPeffect("Common/DepthRTTSkinned.vert", "Common/DepthRTT.frag")));
	shadowSkinnedMaterial.cullFace = OPcullFace::FRONT;
	shadowSkinnedMaterial.cull = shadowMaterial.depth = true;
}

void OPrendererForwardShadowPass::Submit(OPrendererEntity* rendererEntity) {
	renderBucket.Submit(rendererEntity->model, &rendererEntity->world, rendererEntity->shadowMaterial, rendererEntity->desc.materialPerMesh);
}

void OPrendererForwardShadowPass::Begin() {
}

void OPrendererForwardShadowPass::End() {

	depthBuffer.Bind();

	OPrenderClearDepth();

	OPrenderCullMode(OPcullFace::FRONT);

	renderBucket.Sort();
	renderBucket.Flush(false);

	depthBuffer.Unbind();

	OPrenderCullMode(OPcullFace::BACK);
}