#pragma once

#ifndef OPMATH_MAT4
#define OPMATH_MAT4
#include "./Math/include/Vector2.h"
#include "./Math/include/Vector3.h"
#include "./Math/include/Vector4.h"
#include "./Core/include/DynamicMemory.h"
#include "./Core/include/MathHelpers.h"
#include "./Core/include/Log.h"

struct OPmat4;
inline OPvec4* OPmat4index(OPmat4* m, int idx);
inline void OPmat4mul(OPmat4* dst, OPmat4* m1, OPmat4* m2);
inline void OPmat4identity(OPmat4* m);
inline void OPmat4transpose(OPmat4* m);
inline void OPmat4buildRotX(OPmat4* m, OPfloat t);
inline void OPmat4buildRotY(OPmat4* m, OPfloat t);
inline void OPmat4buildRotZ(OPmat4* m, OPfloat t);
inline void OPmat4buildTranslate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4buildScl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4rotX(OPmat4* m, OPfloat x);
inline void OPmat4rotY(OPmat4* m, OPfloat x);
inline void OPmat4rotZ(OPmat4* m, OPfloat x);
inline void OPmat4translate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4scl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z);
inline void OPmat4ortho(OPmat4* m, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top, OPfloat zNear, OPfloat zFar );
inline void OPmat4look(OPmat4* m, OPvec3* campos, OPvec3* look, OPvec3* up);
inline void OPmat4Print(OPmat4 m);
inline void OPmat4perspective(OPmat4* m, OPfloat fovy, OPfloat aspect, OPfloat nearVal, OPfloat farVal);
inline void OPmat4transform(OPvec3* dst, OPvec3* a, OPmat4* b);

// Data Structure 4 * 4 = 16 floats
//		32 bit = 16 * 32 = 512 bits or 64 bytes
//		64 bit = 16 * 64 = 1024 bits or 128 bytes
struct OPmat4 {
	OPvec4 cols[4];
	OPmat4 operator=(OPmat4 vhs) { 
		OPmemcpy(this, (void*)&vhs, sizeof(OPmat4)); return *this;
	}
	inline OPmat4 operator*(OPmat4 vhs) { 
		OPmat4 temp = {
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
		};
		OPmat4mul(&temp, this, &vhs); 
		return temp; 
	}
	inline OPmat4& operator*=(OPmat4 vhs) { 
		OPmat4mul(this, this, &vhs); 
		return *this; 
	}
	inline OPmat4& operator*=(OPvec2 vhs) {
		OPmat4scl(this, vhs.x, vhs.y, 1);
		return *this;
	}
	inline OPmat4& operator*=(OPvec3 vhs) {
		OPmat4scl(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator*=(OPvec4 vhs) {
		OPmat4scl(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator/=(OPvec2 vhs) {
		OPmat4scl(this, vhs.x, vhs.y, 1);
		return *this;
	}
	inline OPmat4& operator/=(OPvec3 vhs) { 
		OPmat4scl(this, vhs.x, vhs.y, vhs.z); 
		return *this; 
	}
	inline OPmat4& operator/=(OPvec4 vhs) { 
		OPmat4scl(this, vhs.x, vhs.y, vhs.z); 
		return *this; 
	}
	inline OPmat4& operator+=(OPvec2 vhs) {
		OPmat4translate(this, vhs.x, vhs.y, 0);
		return *this;
	}
	inline OPmat4& operator+=(OPvec2* vhs) {
		OPmat4translate(this, vhs->x, vhs->y, 0);
		return *this;
	}
	inline OPmat4& operator+=(OPvec3 vhs) {
		OPmat4translate(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator+=(OPvec3* vhs) {
		OPmat4translate(this, vhs->x, vhs->y, vhs->z);
		return *this;
	}
	inline OPmat4& operator+=(OPvec4 vhs) {
		OPmat4translate(this, vhs.x, vhs.y, vhs.z);
		return *this;
	}
	inline OPmat4& operator+=(OPvec4* vhs) {
		OPmat4translate(this, vhs->x, vhs->y, vhs->z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec2 vhs) {
		OPmat4translate(this, -vhs.x, -vhs.y, 0);
		return *this;
	}
	inline OPmat4& operator-=(OPvec2* vhs) {
		OPmat4translate(this, -vhs->x, -vhs->y, 0);
		return *this;
	}
	inline OPmat4& operator-=(OPvec3 vhs) {
		OPmat4translate(this, -vhs.x, -vhs.y, -vhs.z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec3* vhs) {
		OPmat4translate(this, -vhs->x, -vhs->y, -vhs->z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec4 vhs) {
		OPmat4translate(this, -vhs.x, -vhs.y, -vhs.z);
		return *this;
	}
	inline OPmat4& operator-=(OPvec4* vhs) {
		OPmat4translate(this, -vhs->x, -vhs->y, -vhs->z);
		return *this;
	}
};

inline OPvec3 operator*(OPvec3 lhs, OPmat4 rhs) { 
	OPvec3 temp = { 0, 0, 0 };
	OPmat4transform(&temp, &lhs, &rhs);
	return temp; 
}

inline OPvec3 operator*(OPmat4 lhs, OPvec3 rhs) { 
	OPvec3 temp = { 0, 0, 0 };
	OPmat4transform(&temp, &rhs, &lhs);
	return temp; 
}

inline OPvec4* OPmat4index(OPmat4* m, int idx){
	return &((OPvec4*)(m))[idx];
}

inline void OPmat4mul(OPmat4* dst, OPmat4* m1, OPmat4* m2) {
	OPmat4 result;
	OPfloat sum;
	i32 i, j, k = 0;
	OPfloat* cols1 = (OPfloat*)m1->cols;
	OPfloat* cols2 = (OPfloat*)m2->cols;
	OPfloat* colsr = (OPfloat*)result.cols;
	OPmemcpy(&result, m1, sizeof(OPmat4));
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			sum = 0;
			for(k = 0; k < 4; k++){
				sum += cols2[(k << 2) + j] * cols1[(i << 2) + k];
			}
			colsr[(i << 2) + j] = sum;
		}
	}
	OPmemcpy(dst, &result, sizeof(OPmat4));
}

inline void OPmat4identity(OPmat4* m) {
	OPbzero(m, sizeof(OPmat4));
	m->cols[0].x = 1;
	m->cols[1].y = 1;
	m->cols[2].z = 1;
	m->cols[3].w = 1;
}


 inline void OPmat4transpose(OPmat4* m){
	OPmat4* tmp;
	OPint i = 0, j = 0;
	OPmemcpy(&tmp, &m, sizeof(OPmat4*));
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			(*OPvec4index(OPmat4index(m, i),j)) = (*OPvec4index(OPmat4index(tmp, i),j));
		}
	}
}

inline void OPmat4buildRotX(OPmat4* m, OPfloat t) {
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4identity(m);
	m->cols[1].y = t1;
	m->cols[1].z = t2;
	m->cols[2].y = -t2;
	m->cols[2].z = t1;
}

inline void OPmat4buildRotY(OPmat4* m, OPfloat t){
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4identity(m);
	m->cols[0].x = t1;
	m->cols[0].z = -t2;
	m->cols[2].x = t2;
	m->cols[2].z = t1;
}

inline void OPmat4buildRotZ(OPmat4* m, OPfloat t) {
	OPfloat t1 = OPcos(t);
	OPfloat t2 = OPsin(t);
	OPmat4identity(m);
	m->cols[0].x = t1;
	m->cols[0].y = t2;
	m->cols[1].x = -t2;
	m->cols[1].y = t1;
}

inline void OPmat4buildTranslate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	OPmat4identity(m);
	m->cols[3].x = x;
	m->cols[3].y = y;
	m->cols[3].z = z;
}

inline void OPmat4buildScl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	OPmat4identity(m);
	m->cols[0].x = x;
	m->cols[1].y = y;
	m->cols[2].z = z;
}


inline void OPmat4rotX(OPmat4* m, OPfloat x) {
	OPmat4 temp = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	OPmat4buildRotX(&temp, x);
	OPmat4mul(m, m, &temp);
}

inline void OPmat4rotZ(OPmat4* m, OPfloat x) {
	OPmat4 temp = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	OPmat4buildRotZ(&temp, x);
	OPmat4mul(m, m, &temp);
}

inline void OPmat4translate(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	OPmat4 temp = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	OPmat4buildTranslate(&temp, x, y, z);
	OPmat4mul(m, m, &temp);
}

inline void OPmat4scl(OPmat4* m, OPfloat x, OPfloat y, OPfloat z) {
	OPmat4 temp = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	OPmat4buildScl(&temp, x, y, z);
	OPmat4mul(m, m, &temp);
}

inline void OPmat4rotY(OPmat4* m, OPfloat x) {
	OPmat4 temp = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	OPmat4buildRotY(&temp, x);
	OPmat4mul(m, m, &temp);
}

inline void OPmat4ortho(OPmat4* m, OPfloat left, OPfloat right, OPfloat bottom, OPfloat top, OPfloat zNear, OPfloat zFar ){
	OPbzero(m, sizeof(OPmat4));
	m->cols[0].x = 2.0f/(right - left);
	m->cols[1].y = -2.0f/(top - bottom);
	m->cols[2].z = 2.0f/(zNear - zFar);
	m->cols[3].w = 1.0f;
	m->cols[3].x = -(right + left)/(right - left);
	m->cols[3].y = -(top + bottom)/(top - bottom);
	m->cols[3].z = -(zFar + zNear)/(zFar - zNear);
}

inline void OPmat4look(OPmat4* m, OPvec3* campos, OPvec3* look, OPvec3* up) {
	OPvec3 f;
	OPvec3 s;
	OPvec3 u;
	OPint i = 0;
	OPvec3sub(&f, look, campos);
	OPvec3norm(&f, &f);
	OPvec3norm(up, up);
	OPvec3cross(&s, &f, up);
	OPvec3norm(&s, &s);
	OPvec3cross(&u, &s, &f);
	OPmat4identity(m);
	for(i = 0; i < 3; i++){
		m->cols[i].x = ((OPfloat*)&s)[i];
	}
	for(i = 0; i < 3; i++){
		m->cols[i].y = ((OPfloat*)&u)[i];
	}
	for(i = 0; i < 3; i++){
		m->cols[i].z = -((OPfloat*)&f)[i];
	}
	OPmat4 trans;
	OPmat4identity(&trans);
	trans.cols[3].x = -campos->x;
	trans.cols[3].y = -campos->y;
	trans.cols[3].z = -campos->z;
	OPmat4mul(m, m, &trans);
} 

inline void OPmat4Print(OPmat4 m){
	OPlog("[ %.6f  %.6f  %.6f  %.6f ]", m.cols[0].x,m.cols[1].x,m.cols[2].x,m.cols[3].x);
	OPlog("[ %.6f  %.6f  %.6f  %.6f ]", m.cols[0].y,m.cols[1].y,m.cols[2].y,m.cols[3].y);
	OPlog("[ %.6f  %.6f  %.6f  %.6f ]", m.cols[0].z,m.cols[1].z,m.cols[2].z,m.cols[3].z);
	OPlog("[ %.6f  %.6f  %.6f  %.6f ]", m.cols[0].w,m.cols[1].w,m.cols[2].w,m.cols[3].w);
}

inline void OPmat4perspective(OPmat4* m, OPfloat fovy, OPfloat aspect, OPfloat nearVal, OPfloat farVal) {
	OPfloat top = OPtan(fovy * OPpi / 360.0f) * nearVal;
	OPfloat right = top * aspect;
	OPfloat range = farVal - nearVal;
	OPbzero(m, sizeof(OPmat4));
	OPvec4 c0 = {nearVal/right, 0, 0, 0};
	OPvec4 c1 = {0, nearVal/top, 0, 0};
	OPvec4 c2 = {0, 0, -(farVal+nearVal)/range, -1};
	OPvec4 c3 = {0, 0, -2.0f*farVal*nearVal/range, 1};
	m->cols[0] = c0;
	m->cols[1] = c1;
	m->cols[2] = c2;
	m->cols[3] = c3;
}

inline void OPmat4transform(OPvec3* dst, OPvec3* a, OPmat4* b) {
	dst->x = b->cols[0].x * a->x + b->cols[1].x * a->y + b->cols[2].x * a->z;
	dst->y = b->cols[0].y * a->x + b->cols[1].y * a->y + b->cols[2].y * a->z;
	dst->z = b->cols[0].z * a->x + b->cols[1].z * a->y + b->cols[2].z * a->z;
}

#endif