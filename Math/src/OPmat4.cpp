#include "./Math/include/OPmat4.h"

const OPmat4 OPMAT4_ZERO = {
	OPvec4(0, 0, 0, 0),
	OPvec4(0, 0, 0, 0),
	OPvec4(0, 0, 0, 0),
	OPvec4(0, 0, 0, 0)
};

const OPmat4 OPMAT4_IDENTITY = {
	OPvec4(1, 0, 0, 0),
	OPvec4(0, 1, 0, 0),
	OPvec4(0, 0, 1, 0),
	OPvec4(0, 0, 0, 1)
};

OPmat4 OPmat4Ortho(OPfloat left, OPfloat right, OPfloat bottom, OPfloat top, OPfloat zNear, OPfloat zFar){
	OPmat4 m = OPMAT4_IDENTITY;
	OPfloat w = (right - left);
	OPfloat h = (top - bottom);
	OPfloat z = (zFar - zNear);

	m[0][0] = 2.0f / w;
	m[1][1] = 2.0f / h;
	m[2][2] = -2.0f / z;
	m[3][0] = -(right + left) / w;
	m[3][1] = -(top + bottom) / h;
	m[3][2] = -(zFar + zNear) / z;

	return m;
}

OPmat4 OPmat4LookAt(OPvec3 eye, OPvec3 center, OPvec3 up) {
	OPmat4 m = OPMAT4_IDENTITY;

	OPvec3 dist = center - eye;
	OPvec3 f = OPvec3Norm(dist);
	OPvec3 crossed = OPvec3Cross(f, up);
	OPvec3 s = OPvec3Norm(crossed);
	OPvec3 u = OPvec3Cross(s, f);

	m[0][0] = s.x;
	m[1][0] = s.y;
	m[2][0] = s.z;
	m[0][1] = u.x;
	m[1][1] = u.y;
	m[2][1] = u.z;
	m[0][2] =-f.x;
	m[1][2] =-f.y;
	m[2][2] =-f.z;
	m[3][0] =-OPvec3Dot(s, eye);
	m[3][1] =-OPvec3Dot(u, eye);
	m[3][2] = OPvec3Dot(f, eye);


	//OPvec3 X, Y, Z;

	//Z = eye - center;
	//Z = OPvec3Norm(Z);

	//Y = up;
	//X = OPvec3Cross(Y, Z);

	//Y = OPvec3Cross(Z, X);

	//X = OPvec3Norm(X);
	//Y = OPvec3Norm(Y);

	//m[0][0] = X.x;
	//m[1][0] = X.y;
	//m[2][0] = X.z;
	//m[3][0] = -OPvec3Dot(X, eye);
	//m[0][1] = Y.x;
	//m[1][1] = Y.y;
	//m[2][1] = Y.z;
	//m[3][1] = -OPvec3Dot(Y, eye);
	//m[0][2] = Z.x;
	//m[1][2] = Z.y;
	//m[2][2] = Z.z;
	//m[3][2] = -OPvec3Dot(Z, eye);
	//m[0][3] = 0;
	//m[1][3] = 0;
	//m[2][3] = 0;
	//m[3][3] = 1.0f;

	return m;
}

OPmat4 OPmat4Perspective(OPfloat fovy, OPfloat aspect, OPfloat nearVal, OPfloat farVal) {
	OPmat4 m = OPMAT4_IDENTITY;

	OPfloat tanHalfFovy = OPtan(fovy / 2.0f);
	m[0][0] = 1.0f / ( aspect * tanHalfFovy);
	m[1][1] = 1.0f / tanHalfFovy;
	m[2][2] = -(farVal + nearVal) / ( farVal - nearVal);
	m[2][3] = -1.0f;
	m[3][2] = -(2.0f * farVal * nearVal) / ( farVal - nearVal);

	return m;
}

// TODO: Refactor into appropriate namespace method call
OPint OPmat4Inverse(OPmat4* dst, OPmat4 a) {

	OPmat4 b = a;
	OPfloat* m = (OPfloat*)&b;


	// get cofactors of minor matrices
	OPfloat cofactor0 = OPmat4GetCofactor(m[5], m[6], m[7], m[9], m[10], m[11], m[13], m[14], m[15]);
	OPfloat cofactor1 = OPmat4GetCofactor(m[4], m[6], m[7], m[8], m[10], m[11], m[12], m[14], m[15]);
	OPfloat cofactor2 = OPmat4GetCofactor(m[4], m[5], m[7], m[8], m[9], m[11], m[12], m[13], m[15]);
	OPfloat cofactor3 = OPmat4GetCofactor(m[4], m[5], m[6], m[8], m[9], m[10], m[12], m[13], m[14]);

	// get determinant
	OPfloat determinant = m[0] * cofactor0 - m[1] * cofactor1 + m[2] * cofactor2 - m[3] * cofactor3;
	if (OPabsf(determinant) <= 0.00001f)
	{
		*dst = OPMAT4_IDENTITY;
		return 0;
	}

	// get rest of cofactors for adj(M)
	OPfloat cofactor4 = OPmat4GetCofactor(m[1], m[2], m[3], m[9], m[10], m[11], m[13], m[14], m[15]);
	OPfloat cofactor5 = OPmat4GetCofactor(m[0], m[2], m[3], m[8], m[10], m[11], m[12], m[14], m[15]);
	OPfloat cofactor6 = OPmat4GetCofactor(m[0], m[1], m[3], m[8], m[9], m[11], m[12], m[13], m[15]);
	OPfloat cofactor7 = OPmat4GetCofactor(m[0], m[1], m[2], m[8], m[9], m[10], m[12], m[13], m[14]);

	OPfloat cofactor8 = OPmat4GetCofactor(m[1], m[2], m[3], m[5], m[6], m[7], m[13], m[14], m[15]);
	OPfloat cofactor9 = OPmat4GetCofactor(m[0], m[2], m[3], m[4], m[6], m[7], m[12], m[14], m[15]);
	OPfloat cofactor10 = OPmat4GetCofactor(m[0], m[1], m[3], m[4], m[5], m[7], m[12], m[13], m[15]);
	OPfloat cofactor11 = OPmat4GetCofactor(m[0], m[1], m[2], m[4], m[5], m[6], m[12], m[13], m[14]);

	OPfloat cofactor12 = OPmat4GetCofactor(m[1], m[2], m[3], m[5], m[6], m[7], m[9], m[10], m[11]);
	OPfloat cofactor13 = OPmat4GetCofactor(m[0], m[2], m[3], m[4], m[6], m[7], m[8], m[10], m[11]);
	OPfloat cofactor14 = OPmat4GetCofactor(m[0], m[1], m[3], m[4], m[5], m[7], m[8], m[9], m[11]);
	OPfloat cofactor15 = OPmat4GetCofactor(m[0], m[1], m[2], m[4], m[5], m[6], m[8], m[9], m[10]);

	// build inverse matrix = adj(M) / det(M)
	// adjugate of M is the transpose of the cofactor matrix of M
	OPfloat invDeterminant = 1.0f / determinant;
	m[0] = invDeterminant * cofactor0;
	m[1] = -invDeterminant * cofactor4;
	m[2] = invDeterminant * cofactor8;
	m[3] = -invDeterminant * cofactor12;

	m[4] = -invDeterminant * cofactor1;
	m[5] = invDeterminant * cofactor5;
	m[6] = -invDeterminant * cofactor9;
	m[7] = invDeterminant * cofactor13;

	m[8] = invDeterminant * cofactor2;
	m[9] = -invDeterminant * cofactor6;
	m[10] = invDeterminant * cofactor10;
	m[11] = -invDeterminant * cofactor14;

	m[12] = -invDeterminant * cofactor3;
	m[13] = invDeterminant * cofactor7;
	m[14] = -invDeterminant * cofactor11;
	m[15] = invDeterminant * cofactor15;

	*dst = b;

	return 1;
}

OPmat4 OPmat4Interpolate(OPmat4 a, OPmat4 b, OPfloat percent) {
	OPmat4 result;

	for (i32 j = 0; j < 4; j++) {
		for (i32 k = 0; k < 4; k++) {
			result[j][k] = a[j][k] * (1.0f - percent) + b[j][k] * percent;
		}
	}

	return result;
}

void OPmat4Mul(OPmat4* dst, OPmat4 m1, OPmat4 m2)
{
	OPmat4 result;
	result[0][0] = m1[0][0] * m2[0][0] + m1[1][0] * m2[0][1] + m1[2][0] * m2[0][2] + m1[3][0] * m2[0][3];
	result[1][0] = m1[0][0] * m2[1][0] + m1[1][0] * m2[1][1] + m1[2][0] * m2[1][2] + m1[3][0] * m2[1][3];
	result[2][0] = m1[0][0] * m2[2][0] + m1[1][0] * m2[2][1] + m1[2][0] * m2[2][2] + m1[3][0] * m2[2][3];
	result[3][0] = m1[0][0] * m2[3][0] + m1[1][0] * m2[3][1] + m1[2][0] * m2[3][2] + m1[3][0] * m2[3][3];
	result[0][1] = m1[0][1] * m2[0][0] + m1[1][1] * m2[0][1] + m1[2][1] * m2[0][2] + m1[3][1] * m2[0][3];
	result[1][1] = m1[0][1] * m2[1][0] + m1[1][1] * m2[1][1] + m1[2][1] * m2[1][2] + m1[3][1] * m2[1][3];
	result[2][1] = m1[0][1] * m2[2][0] + m1[1][1] * m2[2][1] + m1[2][1] * m2[2][2] + m1[3][1] * m2[2][3];
	result[3][1] = m1[0][1] * m2[3][0] + m1[1][1] * m2[3][1] + m1[2][1] * m2[3][2] + m1[3][1] * m2[3][3];
	result[0][2] = m1[0][2] * m2[0][0] + m1[1][2] * m2[0][1] + m1[2][2] * m2[0][2] + m1[3][2] * m2[0][3];
	result[1][2] = m1[0][2] * m2[1][0] + m1[1][2] * m2[1][1] + m1[2][2] * m2[1][2] + m1[3][2] * m2[1][3];
	result[2][2] = m1[0][2] * m2[2][0] + m1[1][2] * m2[2][1] + m1[2][2] * m2[2][2] + m1[3][2] * m2[2][3];
	result[3][2] = m1[0][2] * m2[3][0] + m1[1][2] * m2[3][1] + m1[2][2] * m2[3][2] + m1[3][2] * m2[3][3];
	result[0][3] = m1[0][3] * m2[0][0] + m1[1][3] * m2[0][1] + m1[2][3] * m2[0][2] + m1[3][3] * m2[0][3];
	result[1][3] = m1[0][3] * m2[1][0] + m1[1][3] * m2[1][1] + m1[2][3] * m2[1][2] + m1[3][3] * m2[1][3];
	result[2][3] = m1[0][3] * m2[2][0] + m1[1][3] * m2[2][1] + m1[2][3] * m2[2][2] + m1[3][3] * m2[2][3];
	result[3][3] = m1[0][3] * m2[3][0] + m1[1][3] * m2[3][1] + m1[2][3] * m2[3][2] + m1[3][3] * m2[3][3];
	*dst = result;
};

OPmat4 OPmat4From(OPquat a) {
	OPmat4 result = OPMAT4_IDENTITY;
	result[0][0] = 1.0f - 2.0f * (a.y * a.y + a.z * a.z);
	result[0][1] = 2.0f * (a.x * a.y - a.w * a.z);
	result[0][2] = 2.0f * (a.x * a.z + a.w * a.y);
	result[1][0] = 2.0f * (a.x * a.y + a.w * a.z);
	result[1][1] = 1.0f - 2.0f * (a.x * a.x + a.z * a.z);
	result[1][2] = 2.0f * (a.y * a.z - a.w * a.x);
	result[2][0] = 2.0f * (a.x * a.z - a.w * a.y);
	result[2][1] = 2.0f * (a.y * a.z + a.w * a.x);
	result[2][2] = 1.0f - 2.0f * (a.x * a.x + a.y * a.y);

	result = OPmat4Transpose(result);
	return result;
}

void OPmat4Log(const OPchar* msg, OPmat4 m) {
	OPlogInfo("%s:\n\t%f,\t%f,\t%f,\t%f\n\t%f,\t%f,\t%f,\t%f\n\t%f,\t%f,\t%f,\t%f\n\t%f,\t%f,\t%f,\t%f",
		msg,
		m[0][0], m[1][0], m[2][0], m[3][0],
		m[0][1], m[1][1], m[2][1], m[3][1],
		m[0][2], m[1][2], m[2][2], m[3][2],
		m[0][3], m[1][3], m[2][3], m[3][3]);
}

OPmat4 OPmat4Read(OPstream* str) {
	OPmat4 temp = {
		OPvec4::Read(str),
		OPvec4::Read(str),
		OPvec4::Read(str),
		OPvec4::Read(str)
	};
	return temp;
}

void OPmat4Write(OPmat4 v, OPstream* str) {
	v[0].Write(str);
	v[1].Write(str);
	v[2].Write(str);
	v[3].Write(str);
}

OPmat4 OPmat4RemoveScale(OPmat4 a) {
	OPvec3 r;
	r = OPvec3Norm(OPvec3Create(a[0][0], a[0][1], a[0][2]));
	a[0][0] = r.x; a[0][1] = r.y; a[0][2] = r.z;

	r = OPvec3Norm(OPvec3Create(a[1][0], a[1][1], a[1][2]));
	a[1][0] = r.x; a[1][1] = r.y; a[1][2] = r.z;

	r = OPvec3Norm(OPvec3Create(a[2][0], a[2][1], a[2][2]));
	a[2][0] = r.x; a[2][1] = r.y; a[2][2] = r.z;

	return a;
}

OPvec3 OPmat4Eulor(OPmat4 a)
{
	d64 sinPitch, cosPitch, sinRoll, cosRoll, sinYaw, cosYaw;

	sinPitch = -a[2][0];
	cosPitch = OPsqrt(1 - sinPitch*sinPitch);

	if (OPabs(cosPitch) > 0.0001)
	{
		sinRoll = a[2][1] / cosPitch;
		cosRoll = a[2][2] / cosPitch;
		sinYaw = a[1][0] / cosPitch;
		cosYaw = a[0][0] / cosPitch;
	}
	else
	{
		sinRoll = -a[1][2];
		cosRoll = a[1][1];
		sinYaw = 0;
		cosYaw = 1;
	}

	return OPvec3Create(
		(OPfloat)(atan2(sinYaw, cosYaw) * 180 / OPpi),
		(OPfloat)(atan2(sinPitch, cosPitch) * 180 / OPpi),
		(OPfloat)(atan2(sinRoll, cosRoll) * 180 / OPpi));
}


OPvec3 OPmat4Transform(OPvec3 a, OPmat4 b) {
	OPvec3 result;
	result.x = b[0].x * a.x + b[1].x * a.y + b[2].x * a.z + b[3][0] * 1.0f;
	result.y = b[0].y * a.x + b[1].y * a.y + b[2].y * a.z + b[3][1] * 1.0f;
	result.z = b[0].z * a.x + b[1].z * a.y + b[2].z * a.z + b[3][2] * 1.0f;
	return result;
}

OPvec4 OPmat4Transform(OPvec4 a, OPmat4 b) {
	OPvec4 result;
	result.x = b[0][0] * a.x + b[1][0] * a.y + b[2][0] * a.z + b[3][0] * a.w;
	result.y = b[0][1] * a.x + b[1][1] * a.y + b[2][1] * a.z + b[3][1] * a.w;
	result.z = b[0][2] * a.x + b[1][2] * a.y + b[2][2] * a.z + b[3][2] * a.w;
	result.w = b[0][3] * a.x + b[1][3] * a.y + b[2][3] * a.z + b[3][3] * a.w;
	return result;
}

// Test methods

OPmat4 OPmat4RotationBetween(OPvec3 start, OPvec3 dest) {
	start = OPvec3Norm(start);
	dest = OPvec3Norm(dest);

	OPvec3 axis = OPvec3Cross(start, dest);
	f32 len = OPvec3Len(axis);
	if (len == 0) {
		return OPMAT4_IDENTITY;
	}

	f32 angle = OPasin(len);
	axis *= 1 / len;

	return OPmat4RotY(axis.y) * OPmat4RotZ(axis.z) * OPmat4RotX(axis.x);
}

OPmat4 OPmat4RotationNormal(OPvec3 normal) {
	OPvec3 up = OPVEC3_UP;
	normal = OPvec3Norm(normal);
	OPvec3 axis = OPvec3Norm(OPvec3Cross(up, normal));

	OPvec3 other = OPvec3Norm(OPvec3Cross(axis, normal));

	OPmat4 result = OPMAT4_IDENTITY;

	result[0][0] = axis.x;
	result[1][0] = axis.y;
	result[2][0] = axis.z;

	result[0][1] = normal.x;
	result[1][1] = normal.y;
	result[2][1] = normal.z;

	result[0][2] = other.x;
	result[1][2] = other.y;
	result[2][2] = other.z;

	return result;
}

OPmat4 OPmat4RotationNormal2(OPvec3 normal) {
	OPvec3 up = OPVEC3_UP;
	normal = OPvec3Norm(normal);
	OPvec3 axis = OPvec3Norm(OPvec3Cross(up, normal));

	f32 dot = OPvec3Dot(OPVEC3_UP, normal);
	f32 phi = OPacos(dot);

	OPmat4 result = OPMAT4_IDENTITY;

	f32 rcos = OPcos(phi);
	f32 rsin = OPsin(phi);
	f32 u = axis.x;
	f32 v = axis.y;
	f32 w = axis.z;
	result[0][0] = rcos + u*u*(1 - rcos);
	result[1][0] = w * rsin + v*u*(1 - rcos);
	result[2][0] = -v * rsin + w*u*(1 - rcos);
	result[0][1] = -w * rsin + u*v*(1 - rcos);
	result[1][1] = rcos + v*v*(1 - rcos);
	result[2][1] = u * rsin + w*v*(1 - rcos);
	result[0][2] = v * rsin + u*w*(1 - rcos);
	result[1][2] = -u * rsin + v*w*(1 - rcos);
	result[2][2] = rcos + w*w*(1 - rcos);

	return result;
}