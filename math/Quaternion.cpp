#include "Quaternion.h"

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion result;
	Vector3 cross = Cross({ lhs.x,lhs.y,lhs.z }, { rhs.x,rhs.y,rhs.z });
	float dot = Dot({ lhs.x,lhs.y,lhs.z }, { rhs.x,rhs.y,rhs.z });
	result.x = cross.num[0] + rhs.w * lhs.x + lhs.w * rhs.x;
	result.y = cross.num[1] + rhs.w * lhs.y + lhs.w * rhs.y;
	result.z = cross.num[2] + rhs.w * lhs.z + lhs.w * rhs.z;
	result.w = lhs.w * rhs.w - dot;

	return result;
}

Quaternion IdentityQuaternion()
{
	return Quaternion({ 0.0f,0.0f,0.0f,1.0f });
}

Quaternion Conjugate(const Quaternion& quaternion)
{
	return Quaternion({ quaternion.x * -1.0f,quaternion.y * -1.0f ,quaternion.z * -1.0f ,quaternion.w });
}

float Norm(const Quaternion& quaternion)
{
	return sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
}

Quaternion Normalize(const Quaternion& quaternion)
{
	Quaternion result{};
	float norm = Norm(quaternion);
	if (norm != 0.0f) {
		result.x = quaternion.x / norm;
		result.y = quaternion.y / norm;
		result.z = quaternion.z / norm;
		result.w = quaternion.w / norm;
	}
	return result;
}

Quaternion Inverse(const Quaternion& quaternion)
{
	Quaternion result;
	float norm = Norm(quaternion);
	norm = norm * norm;
	Quaternion conj = Conjugate(quaternion);
	if (norm != 0.0f) {
		result.x = conj.x / norm;
		result.y = conj.y / norm;
		result.z = conj.z / norm;
		result.w = conj.w / norm;
	}
	return result;
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
{
	Quaternion result;
	result.x = axis.num[0] * sin(angle / 2);
	result.y = axis.num[1] * sin(angle / 2);
	result.z = axis.num[2] * sin(angle / 2);
	result.w = cos(angle / 2);
	return result;
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Quaternion fromVector = { vector.num[0],vector.num[1],vector.num[2],0.0f};
	Quaternion conj = Conjugate(quaternion);
	Quaternion rotate = Multiply(quaternion, Multiply(fromVector, conj));
	Vector3 result;
	result.num[0] = rotate.x;
	result.num[1] = rotate.y;
	result.num[2] = rotate.z;

	return result;
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion)
{
	Matrix4x4 result;
	result.m[0][0] = (quaternion.w * quaternion.w) + (quaternion.x * quaternion.x) - (quaternion.y * quaternion.y) - (quaternion.z * quaternion.z);
	result.m[0][1] = 2 * ((quaternion.x * quaternion.y) + (quaternion.w * quaternion.z));
	result.m[0][2] = 2 * ((quaternion.x * quaternion.z) - (quaternion.w * quaternion.y));
	result.m[0][3] = 0;

	result.m[1][0] = 2 * ((quaternion.x * quaternion.y) - (quaternion.w * quaternion.z));
	result.m[1][1] = (quaternion.w * quaternion.w) - (quaternion.x * quaternion.x) + (quaternion.y * quaternion.y) - (quaternion.z * quaternion.z);
	result.m[1][2] = 2 * ((quaternion.y * quaternion.z) + (quaternion.w * quaternion.x));
	result.m[1][3] = 0;

	result.m[2][0] = 2 * ((quaternion.x * quaternion.z) + (quaternion.w * quaternion.y));
	result.m[2][1] = 2 * ((quaternion.y * quaternion.z) - (quaternion.w * quaternion.x));
	result.m[2][2] = (quaternion.w * quaternion.w) - (quaternion.x * quaternion.x) - (quaternion.y * quaternion.y) + (quaternion.z * quaternion.z);
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}