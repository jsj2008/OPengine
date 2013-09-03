#pragma once
#include "./Core/include/Types.h"
#include "PackedMesh.h"
#include "./Human/include/Math/Matrix4.h"
#include "./Human/include/Resources/Material/Material.h"

class PackedModel{
public:
	PackedModel(PackedMeshPtr mesh, MaterialPtr material);
	~PackedModel();
	void SetMeshData();

	PackedMeshPtr ModelMesh;
	MaterialPtr ModelMaterial;
	Matrix4* WorldMatrix;
};

typedef PackedModel* PackedModelPtr; 