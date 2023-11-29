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