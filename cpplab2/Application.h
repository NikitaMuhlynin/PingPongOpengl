#pragma once

#include <string>

#include "MovingPoint.h"
#include "TrailRenderer.h"

struct GLFWwindow;

class Application {
public:
	Application(int width, int heigth, const std::string& title);
	~Application();

	int run();

private:
	bool initialization();
	void shutdown();
	void processFrame(float dt);
	void renderFrame();

	int m_width;
	int m_height;
	std::string m_title;

	GLFWwindow* m_window = nullptr;

	MovingPoint m_point;
	TrailRenderer m_trailRenderer;
};