#include "Application.h"

#include <algorithm>
#include <exception>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace {
	void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
		(void)window;
		glViewport(0, 0, width, height);
	}
}

Application::Application(int width, int height, const std::string& title)
	: m_width(width), m_height(height), m_title(title) {}

Application::~Application() {
	shutdown();
}

bool Application::initialization() {
	if (!glfwInit()) {
		std::cerr << "Не удалось инициализировать GLFW\n";
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window) {
        const char* description = nullptr;
        int errorCode = glfwGetError(&description);

        std::cerr << "Не удалось создать окно GLFW\n";
        std::cerr << "GLFW error code: " << errorCode << '\n';
        std::cerr << "GLFW description: " << (description ? description : "unknown") << '\n';

        glfwTerminate();
        return false;
    }

	glfwMakeContextCurrent(m_window);
    //glfwSwapInterval(1);
	glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Не удалось инициализировать GLAD\n";
		return false;
	}

	glViewport(0, 0, m_width, m_height);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);

    m_trailRenderer.initialize("shaders/point.vert", "shaders/point.frag");

    return true;
}

void Application::shutdown() {
    m_trailRenderer.shutdown();

    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    glfwTerminate();
}

void Application::processFrame(float dt) {
    glfwPollEvents();

    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);

    m_point.processInput(m_window);
    m_point.update(dt);
    m_trailRenderer.update(m_point.getPosition(), m_point.getSpeed(), dt);
}

void Application::renderFrame() {
    glClearColor(0.05f, 0.06f, 0.10f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_trailRenderer.render(m_point.getPosition());

    glfwSwapBuffers(m_window);
}

int Application::run() {
    try {
        if (!initialization())
            return -1;

        double previousTime = glfwGetTime();

        while (!glfwWindowShouldClose(m_window)) {
            double currentTime = glfwGetTime();
            float dt = static_cast<float>(currentTime - previousTime);
            previousTime = currentTime;

            dt = std::min(dt, 0.033f);

            processFrame(dt);
            renderFrame();
        }
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
        return -1;
    }

    return 0;
}