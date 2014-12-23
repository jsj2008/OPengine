#ifndef OPENGINE_HUMAN_RENDERER_FONT_ATLAS
#define OPENGINE_HUMAN_RENDERER_FONT_ATLAS

#include "./Data/include/OPvector.h"
#include "./Core/include/OPtypes.h"

typedef struct {
	i32 x;
	i32 y;
	i32 z;
} OPfontAtlasNode;

typedef struct {
	i32 x;
	i32 y;
	i32 width;
	i32 height;
} OPfontAtlasRegion;

typedef struct {
	OPvector* nodes;
	OPint width;
	OPint height;
	OPint depth;
	OPint used;
	ui8* data;
} OPfontAtlas;

#include "./Human/include/Rendering/Font/OPfont.h"
#include "./Human/include/Rendering/OPtexture.h"

OPfontAtlas* OPfontAtlasCreate(OPint width, OPint height, OPint depth);
void OPfontAtlasDestroy(OPfontAtlas* atlas);
void OPfontAtlasSetRegion(OPfontAtlas* atlas, OPint x, OPint y, OPint width, OPint height, ui8* data, OPint stride);
OPint OPfontAtlasFit(OPfontAtlas* atlas, OPint index, OPint width, OPint height);
void OPfontAtlasMerge(OPfontAtlas* atlas);
OPfontAtlasRegion OPfontAtlasGetRegion(OPfontAtlas* atlas, OPint width, OPint height);
OPtexture OPfontAtlasTexture(OPfontAtlas* atlas);
void OPfontAtlasSavePNG(OPfontAtlas* atlas, OPchar* filename);

#endif