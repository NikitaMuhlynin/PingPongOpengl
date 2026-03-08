#pragma once

struct GLFWwindow;

#include "Vector2.h"

class MovingPoint {
public:
	MovingPoint();

	void processInput(GLFWwindow* window);
	void update(float dt);

	const Vector2& getPosition() const;
	const Vector2& getVelocity() const;
	float getSpeed() const;

private:
	void clampToBounds();

	Vector2 m_position;
	Vector2 m_velocity;
	Vector2 m_inputDirection;

	static constexpr float ACCELERATION = 15.0f;
	static constexpr float DRAG = 0.4f;
	static constexpr float MAX_SPEED = 2.0f;
	static constexpr float BORDER = 1.0f;
};