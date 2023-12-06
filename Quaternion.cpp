#include "Quaternion.h"

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion result;
	Vector3 cross = Cross({ lhs.num[0],lhs.num[1],lhs.num[2]}, {rhs.num[0],rhs.num[1],rhs.num[2]});
	float dot = Dot({ lhs.num[0],lhs.num[1],lhs.num[2] }, { rhs.num[0],rhs.num[1],rhs.num[2] });
	result.num[0] = cross.num[0] + rhs.num[3] * lhs.num[0] + lhs.num[3] * rhs.num[0];
	result.num[1] = cross.num[1] + rhs.num[3] * lhs.num[1] + lhs.num[3] * rhs.num[1];
	result.num[2] = cross.num[2] + rhs.num[3] * lhs.num[2] + lhs.num[3] * rhs.num[2];
	result.num[3] = lhs.num[3] * rhs.num[3] - dot;

	return result;
}

Quaternion IdentityQuaternion()
{
	return Quaternion({ 0.0f,0.0f,0.0f,1.0f });
}

Quaternion Conjugate(const Quaternion& quaternion)
{
	return Quaternion({ quaternion.num[0] * -1.0f,quaternion.num[1] * -1.0f ,quaternion.num[2] * -1.0f ,quaternion.num[3]});
}

float Norm(const Quaternion& quaternion)
{
	return sqrt(quaternion.num[0] * quaternion.num[0] + quaternion.num[1] * quaternion.num[1] + quaternion.num[2] * quaternion.num[2] + quaternion.num[3] * quaternion.num[3]);
}

Quaternion Normalize(const Quaternion& quaternion)
{
	Quaternion result{};
	float norm = Norm(quaternion);
	if (norm != 0.0f) {
		result.num[0] = quaternion.num[0] / norm;
		result.num[1] = quaternion.num[1] / norm;
		result.num[2] = quaternion.num[2] / norm;
		result.num[3] = quaternion.num[3] / norm;
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
		result.num[0] = conj.num[0] / norm;
		result.num[1] = conj.num[1] / norm;
		result.num[2] = conj.num[2] / norm;
		result.num[3] = conj.num[3] / norm;
	}
	return result;
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) 
{
	Quaternion result;
	result.num[0] = axis.num[0] * sin(angle / 2);
	result.num[1] = axis.num[1] * sin(angle / 2);
	result.num[2] = axis.num[2] * sin(angle / 2);
	result.num[3] = cos(angle / 2);
	return result;
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) 
{
	Quaternion fromVector = { vector.num[0],vector.num[1],vector.num[2],0.0f };
	Quaternion conj = Conjugate(quaternion);
	Quaternion rotate = Multiply(quaternion, Multiply(fromVector, conj));
	Vector3 result;
	result.num[0] = rotate.num[0];
	result.num[1] = rotate.num[1];
	result.num[2] = rotate.num[2];
	return result;
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion)
{
	Matrix4x4 result;
	result.m[0][0] = (quaternion.num[3] * quaternion.num[3]) + (quaternion.num[0] * quaternion.num[0]) - (quaternion.num[1] * quaternion.num[1]) - (quaternion.num[2] * quaternion.num[2]);
	result.m[0][1] = 2 * ((quaternion.num[0] * quaternion.num[1]) + (quaternion.num[3] * quaternion.num[2]));
	result.m[0][2] = 2 * ((quaternion.num[0] * quaternion.num[2]) - (quaternion.num[3] * quaternion.num[1]));
	result.m[0][3] = 0;

	result.m[1][0] = 2 * ((quaternion.num[0] * quaternion.num[1]) - (quaternion.num[3] * quaternion.num[2]));
	result.m[1][1] = (quaternion.num[3] * quaternion.num[3]) - (quaternion.num[0] * quaternion.num[0]) + (quaternion.num[1] * quaternion.num[1]) - (quaternion.num[2] * quaternion.num[2]);
	result.m[1][2] = 2 * ((quaternion.num[1] * quaternion.num[2]) + (quaternion.num[3] * quaternion.num[0]));
	result.m[1][3] = 0;

	result.m[2][0] = 2 * ((quaternion.num[0] * quaternion.num[2]) + (quaternion.num[3] * quaternion.num[1]));
	result.m[2][1] = 2 * ((quaternion.num[1] * quaternion.num[2]) - (quaternion.num[3] * quaternion.num[0]));
	result.m[2][2] = (quaternion.num[3] * quaternion.num[3]) - (quaternion.num[0] * quaternion.num[0]) - (quaternion.num[1] * quaternion.num[1]) + (quaternion.num[2] * quaternion.num[2]);
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}