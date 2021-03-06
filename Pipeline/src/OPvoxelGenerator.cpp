#include "./Pipeline/include/OPvoxelGenerator.h"
#include "./OPengine.h"

void OPvoxelGeneratorInit(struct OPvoxelGenerator* gen, ui32 features) {
	OPvertexLayoutBuilder builder;
	builder.Init(features);
	gen->VertexSize = builder.Build();

	gen->Features = features;
	gen->Vertices = OPlist::Create(512, gen->VertexSize.stride);
	gen->Indices = OPlist::Create(512, sizeof(ui32));
	gen->Vertex = OPalloc(gen->VertexSize.stride);
	gen->IndexOffset = 0;

	gen->HideFace[0] = 0;
	gen->HideFace[1] = 0;
	gen->HideFace[2] = 0;
	gen->HideFace[3] = 0;
	gen->HideFace[4] = 0;
	gen->HideFace[5] = 0;

	gen->Scale = 1.0f;
	gen->Center = true;
}

struct OPvoxelGenerator* OPvoxelGeneratorCreate(ui32 features) {
  struct OPvoxelGenerator* result = (struct OPvoxelGenerator*)OPalloc(sizeof(struct OPvoxelGenerator));
  OPvoxelGeneratorInit(result, features);
  return result;
}

void setData(struct OPvoxelGenerator* gen, struct OPvoxelGeneratorVertex* vertex, void* data, struct OPvoxelGeneratorData* voxelGenData) {
	OPuint loc = (OPuint)data;

	if (gen->Features & (OPuint)OPattributes::POSITION) {
		OPvec3* tmp = (OPvec3*)(void*)loc;
		*tmp = vertex->Position * gen->Scale;
		loc += sizeof(OPvec3);
	}

	if (gen->Features & (OPuint)OPattributes::NORMAL) {
		OPvec3* tmp = (OPvec3*)(void*)loc;
		*tmp = vertex->Normal;
		loc += sizeof(OPvec3);
	}

	if (gen->Features & (OPuint)OPattributes::TANGENT) {
		OPvec3* tmp = (OPvec3*)(void*)loc;
		*tmp = vertex->Tangent;
		loc += sizeof(OPvec3);
	}

	if (gen->Features & (OPuint)OPattributes::UV) {
		OPvec2* tmp = (OPvec2*)(void*)loc;
		*tmp = vertex->UV;
		loc += sizeof(OPvec2);
	}

	if (gen->Features & (OPuint)OPattributes::BONES) {
		OPvec4* tmp = (OPvec4*)(void*)loc;
		*tmp = vertex->Bones;
		loc += sizeof(OPvec4);

		tmp = (OPvec4*)(void*)loc;
		*tmp = vertex->Weights;
		loc += sizeof(OPvec4);
	}

	if (gen->Features & (OPuint)OPattributes::COLOR) {
		OPvec3* tmp = (OPvec3*)(void*)loc;
		*tmp = vertex->Color;
		loc += sizeof(OPvec3);
	}
}

void addSide(struct OPvoxelGenerator* gen, struct OPvoxelGeneratorData* data) {
	ui8 i = 0;

	for (i = 0; i < 4; i++) {
		data->Vertices[i].Position += data->Offset;
		setData(gen, &data->Vertices[i], gen->Vertex, data);
		gen->Vertices->Push((ui8*)gen->Vertex);
	}

	data->Indices[0] = gen->IndexOffset;
	data->Indices[1] = gen->IndexOffset + 1;
	data->Indices[2] = gen->IndexOffset + 2;
	data->Indices[3] = gen->IndexOffset;
	data->Indices[4] = gen->IndexOffset + 2;
	data->Indices[5] = gen->IndexOffset + 3;
	gen->IndexOffset += 4;

	for (i = 0; i < 6; i++) {
		gen->Indices->Push((ui8*)&data->Indices[i]);
	}
}

void addLeftSide(struct OPvoxelGenerator* gen, struct OPvoxelGeneratorData* data, OPint x, OPint y, OPint z) {
	data->Vertices[0].Position = OPvec3Create(-data->Size + x, data->Size+ y, data->Size + z);   // top left
	data->Vertices[1].Position = OPvec3Create(-data->Size + x, data->Size+ y, -data->Size + z);  // top right
	data->Vertices[2].Position = OPvec3Create(-data->Size + x, -data->Size+ y, -data->Size + z); // bottom right
	data->Vertices[3].Position = OPvec3Create(-data->Size + x, -data->Size+ y, data->Size + z);  // bottom left

	for (ui8 i = 0; i < 4; i++) {
		data->Vertices[i].Normal = OPvec3Create(-1, 0, 0);
		data->Vertices[i].Color = data->Color;
	}

	addSide(gen, data);
}

void addRightSide(struct OPvoxelGenerator* gen, struct OPvoxelGeneratorData* data, OPint x, OPint y, OPint z) {
	data->Vertices[0].Position = OPvec3Create(data->Size + x, data->Size+ y, -data->Size + z);
	data->Vertices[1].Position = OPvec3Create(data->Size + x, data->Size+ y, data->Size + z);
	data->Vertices[2].Position = OPvec3Create(data->Size + x, -data->Size+ y, data->Size + z);
	data->Vertices[3].Position = OPvec3Create(data->Size + x, -data->Size+ y, -data->Size + z);

	for (ui8 i = 0; i < 4; i++) {
		data->Vertices[i].Normal = OPvec3Create(1, 0, 0);
		data->Vertices[i].Color = data->Color;
	}

	addSide(gen, data);
}

void addTopSide(struct OPvoxelGenerator* gen, struct OPvoxelGeneratorData* data, OPint x, OPint y, OPint z) {
	data->Vertices[0].Position = OPvec3Create(-data->Size + x, data->Size+ y, data->Size + z); // left back
	data->Vertices[1].Position = OPvec3Create(data->Size + x, data->Size+ y, data->Size + z);  // right back
	data->Vertices[2].Position = OPvec3Create(data->Size + x, data->Size+ y, -data->Size + z); // right front
	data->Vertices[3].Position = OPvec3Create(-data->Size + x, data->Size+ y, -data->Size + z); // left front

	for (ui8 i = 0; i < 4; i++) {
		data->Vertices[i].Normal = OPvec3Create(0, 1, 0);
		data->Vertices[i].Color = data->Color;
	}

	addSide(gen, data);
}

void addFrontSide(struct OPvoxelGenerator* gen, struct OPvoxelGeneratorData* data, OPint x, OPint y, OPint z) {
	data->Vertices[0].Position = OPvec3Create(data->Size + x, data->Size+ y, data->Size + z); // right top
	data->Vertices[1].Position = OPvec3Create(-data->Size + x, data->Size+ y, data->Size + z); // left top
	data->Vertices[2].Position = OPvec3Create(-data->Size + x, -data->Size+ y, data->Size + z); // left bottom
	data->Vertices[3].Position = OPvec3Create(data->Size + x, -data->Size+ y, data->Size + z); // right bottom

	for (ui8 i = 0; i < 4; i++) {
		data->Vertices[i].Normal = OPvec3Create(0, 0, 1);
		data->Vertices[i].Color = data->Color;
	}

	addSide(gen, data);
}

void addBottomSide(struct OPvoxelGenerator* gen, struct OPvoxelGeneratorData* data, OPint x, OPint y, OPint z) {
	data->Vertices[0].Position = OPvec3Create(-data->Size + x, -data->Size+ y, -data->Size + z);
	data->Vertices[1].Position = OPvec3Create(data->Size + x, -data->Size+ y, -data->Size + z);
	data->Vertices[2].Position = OPvec3Create(data->Size + x, -data->Size+ y, data->Size + z);
	data->Vertices[3].Position = OPvec3Create(-data->Size + x, -data->Size+ y, data->Size + z);

	for (ui8 i = 0; i < 4; i++) {
		data->Vertices[i].Normal = OPvec3Create(0, -1, 0);
		data->Vertices[i].Color = data->Color;
	}

	addSide(gen, data);
}

void addBackSide(struct OPvoxelGenerator* gen, struct OPvoxelGeneratorData* data, OPint x, OPint y, OPint z) {
	data->Vertices[0].Position = OPvec3Create(-data->Size + x, data->Size + y, -data->Size + z);
	data->Vertices[1].Position = OPvec3Create(data->Size + x, data->Size + y, -data->Size + z);
	data->Vertices[2].Position = OPvec3Create(data->Size + x, -data->Size + y, -data->Size + z);
	data->Vertices[3].Position = OPvec3Create(-data->Size + x, -data->Size + y, -data->Size + z);

	for (ui8 i = 0; i < 4; i++) {
		data->Vertices[i].Normal = OPvec3Create(0, 0, -1);
		data->Vertices[i].Color = data->Color;
	}

	addSide(gen, data);
}

void OPvoxelGeneratorAdd(struct OPvoxelGenerator* gen, struct OPvoxels voxelData, OPvec4 bones, OPvec4 weights, OPvec3 offset) {

	bool generate[6];
	OPvoxelGeneratorData data;
	data.Size = 0.5f;
	if(gen->Center) {
		data.Offset = OPvec3Create(
			-(OPint)voxelData.size.x / 2.0f,
			-(OPint)voxelData.size.y / 2.0f,
			-(OPint)voxelData.size.z / 2.0f
			);
	} else {
		data.Offset = OPVEC3_ZERO;
	}

	data.Offset += offset;

	//OPlog("VOXEL SIZE %d, %d, %d", voxelData.size.x, voxelData.size.y, voxelData.size.z);
	//OPvec3Log("OFFSET VOXELS", data.Offset);

	for (ui8 i = 0; i < 4; i++) {
		data.Vertices[i].Bones = bones;
		data.Vertices[i].Weights = weights;
	}

	for (OPint x = 0; x < voxelData.size.x; x++) {
		for (OPint y = 0; y < voxelData.size.y; y++) {
			for (OPint z = 0; z < voxelData.size.z; z++) {
				OPint pos = x + Y_LINE + Z_SLICE;
				OPvecInt3 v = voxelData.voxels[pos];

				if (v.x == 0 && v.y == 0 && v.z == 0) continue;

				// Otherwise, this Voxel is not empty

				data.Color = OPvec3Create(v.x / 255.0f, v.y / 255.0f, v.z / 255.0f);

				OPbzero(generate, sizeof(bool)* 6);

				generate[vLeft] = x == 0 || // Generate Left Side Regardless
					IS_BLOCK_INVISIBLE(VAL(x - 1, y, z)); // Left Sided Neighbor is empty

				generate[vRight] = x == voxelData.size.x - 1 || // Generate Right Side Regardless
					IS_BLOCK_INVISIBLE(VAL(x + 1, y, z)); // Right Sided Neighbor is empty

				generate[vDown] = y == 0 || // Generate Bottom Side Regardless
					IS_BLOCK_INVISIBLE(VAL(x, y - 1, z)); // Bottom Sided Neighbor is empty

				generate[vUp] = y == voxelData.size.y - 1 || // Generate Top Side Regardless
					IS_BLOCK_INVISIBLE(VAL(x, y + 1, z)); // Top Sided Neighbor is empty

				generate[vBackward] = z == 0 || // Generate Backward Side Regardless
					IS_BLOCK_INVISIBLE(VAL(x, y, z - 1)); // Backward Sided Neighbor is empty

				generate[vForward] = z == voxelData.size.z - 1 || // Generate Forward Side Regardless
					IS_BLOCK_INVISIBLE(VAL(x, y, z + 1)); // Forward Sided Neighbor is empty

				if (!gen->HideFace[0] && generate[vLeft])	addLeftSide(gen, &data, x, y, z);
				if (!gen->HideFace[1] && generate[vRight])	addRightSide(gen, &data, x, y, z);
				if (!gen->HideFace[2] && generate[vUp])		addTopSide(gen, &data, x, y, z);
				if (!gen->HideFace[3] && generate[vDown])	addBottomSide(gen, &data, x, y, z);
				if (!gen->HideFace[4] && generate[vForward]) addFrontSide(gen, &data, x, y, z);
				if (!gen->HideFace[5] && generate[vBackward]) addBackSide(gen, &data, x, y, z);
			}
		}
	}
}

OPmodel* OPvoxelGeneratorBuild(struct OPvoxelGenerator* gen) {


	//OPmeshDesc desc = OPvoxelGeneratorBuildDesc(gen);

	ui32 verticeCount = (ui32)gen->Vertices->Size();
	ui32 indiceCount = (ui32)gen->Indices->Size();

	void* verts = OPalloc(gen->VertexSize.stride * verticeCount);
	ui32* inds = (ui32*)OPalloc(sizeof(ui32)* indiceCount);

	for (ui32 i = 0; i < verticeCount; i++) {
		ui8* data = gen->Vertices->Get(i);
		OPmemcpy((void*)((OPuint)verts + gen->VertexSize.stride * i), data, gen->VertexSize.stride);
	}

	OPlog("Index count: %d", indiceCount);

	for (ui32 i = 0; i < indiceCount; i++) {
		inds[i] = (*(ui32*)gen->Indices->Get(i));
	}

	OPmodel* mesh = OPmodel::Create(1, gen->VertexSize);
	mesh->Build(verticeCount, indiceCount, OPindexSize::INT, verts, inds);

	return mesh;
}

void OPvoxelGeneratorDestroy(struct OPvoxelGenerator* gen) {
	gen->Vertices->Destroy();
	OPfree(gen->Vertices);
	gen->Indices->Destroy();
	OPfree(gen->Indices);
}
