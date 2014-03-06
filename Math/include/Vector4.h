#pragma once
#ifndef OP_MATH_VECTOR4
#define OP_MATH_VECTOR4

#include "./Core/include/Types.h"
#include "./Core/include/DynamicMemory.h"
#include "./Core/include/MathHelpers.h"
#include "./Math/include/Vector3.h"

struct OPvec4;
inline OPfloat* OPvec4index(OPvec4* v, int idx);
inline void OPvec4norm(OPvec4* dst, OPvec4* a);

struct OPvec4 {
	OPfloat x, y, z, w;
	OPvec4 operator=(OPvec4 vhs) { 
		OPmemcpy(this, &vhs, sizeof(OPvec4)); return *this;
	}
};

extern const OPvec4 OPvec4Zero;
extern const OPvec4 OPvec4One;

inline OPvec4 OPvec4Create(OPfloat x, OPfloat y, OPfloat z, OPfloat w) {
	OPvec4 tmp = { x, y, z, w };
	return tmp;
}
inline OPvec4 OPvec4CreateFromVec3(OPvec3 xyz, OPfloat w) {
	OPvec4 tmp = { xyz.x, xyz.y, xyz.z, w };
	return tmp;
}

inline OPfloat* OPvec4index(OPvec4* v, int idx){
	return &((OPfloat*)(v))[idx];
}

inline void OPvec4norm(OPvec4* dst, OPvec4* a) {
	OPfloat l = OPsqrt(a->x * a->x + a->y * a->y + a->z * a->z + a->w * a->w);
	dst->x = a->x / l;
	dst->y = a->x / l;
	dst->z = a->x / l;
	dst->w = a->x / l;
}
#endif