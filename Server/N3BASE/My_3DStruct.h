#pragma once

#include <math.h>
#include <cfloat>

/* Here follow the extremely few remaining D3D8 components we require */

// NOTE: We're using this over M_PI because of the precision
#define D3DX_PI    ((float)  3.141592654f)
#define D3DXToRadian( degree ) ((degree) * (D3DX_PI / 180.0f))

/* D3D8 structs */
typedef struct _D3DVECTOR {
	float x;
	float y;
	float z;
} D3DVECTOR;

typedef struct _D3DMATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
} D3DMATRIX;

struct __Matrix44;
struct __Vector3 : public D3DVECTOR // 3D Vertex
{
public:
	INLINE __Vector3() {}
	INLINE __Vector3(float fx, float fy, float fz) { Set(fx, fy, fz); }

	INLINE void Zero() { Set(0.0f, 0.0f, 0.0f); }
	INLINE void Set(float fx, float fy, float fz) { x = fx; y = fy; z = fz; }

	void	Normalize();
	float	Magnitude() const;
	float	Dot(const D3DVECTOR& vec) const;
	void	Cross(const D3DVECTOR& v1, const D3DVECTOR& v2);
	void	Absolute();

	const __Vector3& operator = (const __Vector3& vec);

	const __Vector3 operator * (const D3DMATRIX& mtx) const;
	void operator *= (float fDelta);
	void operator *= (const D3DMATRIX& mtx);
	__Vector3 operator + (const D3DVECTOR& vec) const;
	__Vector3 operator - (const D3DVECTOR& vec) const;
	__Vector3 operator * (const D3DVECTOR& vec) const;
	__Vector3 operator / (const D3DVECTOR& vec) const;

	void operator += (const D3DVECTOR& vec);
	void operator -= (const D3DVECTOR& vec);
	void operator *= (const D3DVECTOR& vec);
	void operator /= (const D3DVECTOR& vec);

	__Vector3 operator + (float fDelta) const;
	__Vector3 operator - (float fDelta) const;
	__Vector3 operator * (float fDelta) const;
	__Vector3 operator / (float fDelta) const;
};

struct __Matrix44 : public D3DMATRIX // 4x4 Matrix
{
public:
	__Matrix44() {}
	void Zero();
	void Identity();
	void RotationY(float fDelta);
};

INLINE void	__Vector3::Normalize()
{
	float fn = sqrtf(x*x + y*y + z*z);
	if(fn == 0) return;
	x /= fn; y /= fn; z /= fn;
}

INLINE float __Vector3::Magnitude() const 
{
	return sqrtf(x*x + y*y + z*z);
}

INLINE float __Vector3::Dot(const D3DVECTOR& vec) const 
{
	return x*vec.x + y*vec.y + z*vec.z;
}

INLINE void __Vector3::Cross(const D3DVECTOR& v1, const D3DVECTOR& v2)
{
	x = v1.y * v2.z - v1.z * v2.y;
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
}

INLINE void __Vector3::Absolute()
{
	if(x < 0) x *= -1.0f;
	if(y < 0) y *= -1.0f;
	if(z < 0) z *= -1.0f;
}

INLINE const __Vector3& __Vector3::operator = (const __Vector3& vec)
{
	x = vec.x; y = vec.y; z = vec.z;
	return *this;
}

INLINE const __Vector3 __Vector3::operator * (const D3DMATRIX& mtx) const 
{
	static __Vector3 vTmp;

	vTmp.x = x*mtx._11 + y*mtx._21 + z*mtx._31 + mtx._41;
	vTmp.y = x*mtx._12 + y*mtx._22 + z*mtx._32 + mtx._42;
	vTmp.z = x*mtx._13 + y*mtx._23 + z*mtx._33 + mtx._43;

	return vTmp;
}

INLINE void __Vector3::operator *= (float fDelta)
{
	x *= fDelta;
	y *= fDelta;
	z *= fDelta;
}

INLINE void __Vector3::operator *= (const D3DMATRIX& mtx)
{
	static __Vector3 vTmp;

	vTmp.Set(x,y,z);
	x = vTmp.x*mtx._11 + vTmp.y*mtx._21 + vTmp.z*mtx._31 + mtx._41;
	y = vTmp.x*mtx._12 + vTmp.y*mtx._22 + vTmp.z*mtx._32 + mtx._42;
	z = vTmp.x*mtx._13 + vTmp.y*mtx._23 + vTmp.z*mtx._33 + mtx._43;
}

INLINE __Vector3 __Vector3::operator + (const D3DVECTOR& vec) const
{
	static __Vector3 vTmp;

	vTmp.x = x + vec.x;
	vTmp.y = y + vec.y;
	vTmp.z = z + vec.z;
	return vTmp;
}

INLINE __Vector3 __Vector3::operator - (const D3DVECTOR& vec) const 
{
	static __Vector3 vTmp;

	vTmp.x = x - vec.x;
	vTmp.y = y - vec.y;
	vTmp.z = z - vec.z;
	return vTmp;
}

INLINE __Vector3 __Vector3::operator * (const D3DVECTOR& vec) const 
{
	static __Vector3 vTmp;

	vTmp.x = x * vec.x;
	vTmp.y = y * vec.y;
	vTmp.z = z * vec.z;
	return vTmp;
}

INLINE __Vector3 __Vector3::operator / (const D3DVECTOR& vec) const
{
	static __Vector3 vTmp;

	vTmp.x = x / vec.x;
	vTmp.y = y / vec.y;
	vTmp.z = z / vec.z;
	return vTmp;
}

INLINE void __Vector3::operator += (const D3DVECTOR& vec) 
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
}

INLINE void __Vector3::operator -= (const D3DVECTOR& vec) 
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
}

INLINE void __Vector3::operator *= (const D3DVECTOR& vec) 
{
	x *= vec.x;
	y *= vec.y;
	z *= vec.z;
}

INLINE void __Vector3::operator /= (const D3DVECTOR& vec) 
{
	x /= vec.x;
	y /= vec.y;
	z /= vec.z;
}

INLINE __Vector3 __Vector3::operator + (float fDelta) const 
{ 
	static __Vector3 vTmp;

	vTmp.x = x + fDelta;
	vTmp.y = y + fDelta;
	vTmp.z = z + fDelta;
	return vTmp;
}

INLINE __Vector3 __Vector3::operator - (float fDelta) const 
{
	static __Vector3 vTmp;

	vTmp.x = x - fDelta;
	vTmp.y = y - fDelta;
	vTmp.z = z - fDelta;
	return vTmp;
}

INLINE __Vector3 __Vector3::operator * (float fDelta) const 
{
	static __Vector3 vTmp;

	vTmp.x = x * fDelta;
	vTmp.y = y * fDelta;
	vTmp.z = z * fDelta;
	return vTmp;
}

INLINE __Vector3 __Vector3::operator / (float fDelta) const 
{
	static __Vector3 vTmp;

	vTmp.x = x / fDelta;
	vTmp.y = y / fDelta;
	vTmp.z = z / fDelta;
	return vTmp;
}

INLINE void __Matrix44::Identity()
{
	_12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0;
	_11 = _22 = _33 = _44 = 1.0f;
}

INLINE void __Matrix44::RotationY(float fDelta)
{
	this->Identity();
	_11 = cosf(fDelta); _13 = -sinf(fDelta); _31 = -_13; _33 = _11;
}

INLINE bool _IntersectTriangle(const __Vector3& vOrig, const __Vector3& vDir,
							   const __Vector3& v0, const __Vector3& v1, const __Vector3& v2,
							   float& fT, float& fU, float& fV, __Vector3* pVCol = nullptr)
{
	// Find vectors for two edges sharing vert0
	static __Vector3 vEdge1, vEdge2;

	vEdge1 = v1 - v0;
	vEdge2 = v2 - v0;

	// Begin calculating determinant - also used to calculate U parameter
	__Vector3 pVec;	float fDet;

	pVec.Cross(vEdge1, vEdge2);
	fDet = pVec.Dot(vDir);
	if ( fDet > -0.0001f )
		return false;

	pVec.Cross(vDir, vEdge2);

	// If determinant is near zero, ray lies in plane of triangle
	fDet = vEdge1.Dot(pVec);
	if( fDet < 0.0001f )
		return false;

	// Calculate distance from vert0 to ray origin
	__Vector3 tVec = vOrig - v0;

	// Calculate U parameter and test bounds
	fU = tVec.Dot(pVec);
	if( fU < 0.0f || fU > fDet )
		return false;

	// Prepare to test V parameter
	__Vector3 qVec;
	qVec.Cross(tVec, vEdge1);

	// Calculate V parameter and test bounds
	fV = vDir.Dot(qVec);
	if( fV < 0.0f || fU + fV > fDet )
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	fT = vEdge2.Dot(qVec);
	float fInvDet = 1.0f / fDet;
	fT *= fInvDet;
	fU *= fInvDet;
	fV *= fInvDet;

	if(pVCol) (*pVCol) = vOrig + (vDir * fT);
	return (fT >= 0.0f);
}

INLINE bool _IntersectTriangle(const __Vector3& vOrig, const __Vector3& vDir, const __Vector3& v0, const __Vector3& v1, const __Vector3& v2)
{
	// Find vectors for two edges sharing vert0
	// Begin calculating determinant - also used to calculate U parameter
	static float fDet, fT, fU, fV;
	static __Vector3 vEdge1, vEdge2, tVec, pVec, qVec;

	vEdge1 = v1 - v0;
	vEdge2 = v2 - v0;

	pVec.Cross(vEdge1, vEdge2);
	fDet = pVec.Dot(vDir);
	if ( fDet > -0.0001f )
		return false;

	pVec.Cross(vDir, vEdge2);

	// If determinant is near zero, ray lies in plane of triangle
	fDet = vEdge1.Dot(pVec);
	if( fDet < 0.0001f )
		return false;

	// Calculate distance from vert0 to ray origin
	tVec = vOrig - v0;

	// Calculate U parameter and test bounds
	fU = tVec.Dot(pVec);
	if( fU < 0.0f || fU > fDet )
		return false;

	// Prepare to test V parameter
	qVec.Cross(tVec, vEdge1);

	// Calculate V parameter and test bounds
	fV = vDir.Dot(qVec);
	if( fV < 0.0f || fU + fV > fDet )
		return false;

	// Calculate t, scale parameters, ray intersects triangle
	fT = vEdge2.Dot(qVec) / fDet;
	return (fT >= 0.0f);
}