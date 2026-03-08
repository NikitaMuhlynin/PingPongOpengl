#include "MovingPoint.h"

#include <GLFW/glfw3.h>

MovingPoint::MovingPoint() :
	m_position(0.0f, 0.0f),
	m_velocity(0.0f, 0.0f),
	m_inputDirection(0.0f, 0.0f) {}

void MovingPoint::processInput(GLFWwindow* window) {
	m_inputDirection = Vector2(0.0f, 0.0f);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		m_inputDirection.x -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		m_inputDirection.x += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		m_inputDirection.y += 1.0f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		m_inputDirection.y -= 1.0f;

	if (m_inputDirection.length() > 0.0f)
		m_inputDirection = m_inputDirection.normalized();
}

void MovingPoint::update(float dt) {
	m_velocity += m_inputDirection * (ACCELERATION * dt);
	m_velocity *= 1.0f / (1.0f + DRAG * dt);

	if (m_velocity.length() > MAX_SPEED)
		m_velocity = m_velocity.normalized() * MAX_SPEED;

	m_position += m_velocity * dt;

	clampToBounds();
}

void MovingPoint::clampToBounds() {
	if (m_position.x < -BORDER) {
		m_position.x = -BORDER;
		m_velocity.x = -m_velocity.x;
	}

	if (m_position.x > BORDER) {
		m_position.x = BORDER;
		m_velocity.x = -m_velocity.x;
	}

	if (m_position.y < -BORDER) {
		m_position.y = -BORDER;
		m_velocity.y = -m_velocity.y;
	}

	if (m_position.y > BORDER) {
		m_position.y = BORDER;
		m_velocity.y = -m_velocity.y;
	}
}

const Vector2& MovingPoint::getPosition() const {
	return m_position;
}

const Vector2& MovingPoint::getVelocity() const {
	return m_velocity;
}

float MovingPoint::getSpeed() const {
	return m_velocity.length();
}