#pragma once

#include "Model.h"

#include ".\Core\include\DynamicMemory.h"
#include "./Human/Math/Vector3.h"
#include "./Human/Math/Vector2.h"

#include "./Human/Rendering/GL/GLBuffer.h"

#include "./Human/Rendering/GL/GLWorldTexturedMaterial.h"

class Quad : public Model {
public:
	Quad(GLWorldTexturedMaterial* material);
	~Quad();

	void SetPosition(Vector3* position);
	void SetRotation(Vector3* rotation);	
	void SetScale(Vector3* scale);
	void SetScale(f32 scale);
	
	Vector3* GetPosition();
	Vector3* GetRotation();
	Vector3* GetScale();
	
private:
	Mesh* GenMesh();
	void UpdateWorld();

protected:
	Vector3 _position;
	Vector3 _rotation;
	Vector3 _scale;
};