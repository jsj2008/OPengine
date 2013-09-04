#pragma once

#include "./Human/include/Resources/Model/Model.h"

#include "./Core/include/DynamicMemory.h"
#include "./Human/include/Math/Vector3.h"

class Quad : public Model {
public:
	Quad() {}
	Quad(MaterialPtr material);
	void Init(MaterialPtr material);
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