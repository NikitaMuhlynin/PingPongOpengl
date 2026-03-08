#pragma once

#include <cmath>

class Vector2 {
public:
	float x;
	float y;

	Vector2(float xValue = 0.0f, float yValue = 0.0f)
		: x(xValue), y(yValue) {}

	Vector2 operator+(const Vector2& other) const {
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 operator-(const Vector2& other) const {
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 operator*(float scolar) const {
		return Vector2(x * scolar, y * scolar);
	}

	Vector2 operator/(float scolar) const {
		return Vector2(x / scolar, y / scolar);
	}

	Vector2& operator+=(const Vector2& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2& operator-=(const Vector2& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2& operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2& operator/=(float scalar)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	float length() const {
		return std::sqrt(x * x + y * y);
	}

	Vector2 normalized() const {
		float len = length();
		if (len < 1e-6f)
			return Vector2(0.0f, 0.0f);

		return Vector2(x / len, y / len);
	}
};

inline Vector2 operator*(float scalar, const Vector2& v) {
	return v * scalar;
}