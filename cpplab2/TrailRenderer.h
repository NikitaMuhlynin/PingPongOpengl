#pragma once

#include <vector>
#include <glad/glad.h>

#include "ShaderProgram.h"
#include "Vector2.h"

class TrailRenderer {
public:
	TrailRenderer();
	~TrailRenderer();

	TrailRenderer(const TrailRenderer&) = delete;
	TrailRenderer operator=(const TrailRenderer&) = delete;

	bool initialize(const char* vertexShaderPath, const char* fragmentShaderPath);
	void update(const Vector2& pointPosition, float pointSpeed, float dt);
	void render(const Vector2& pointPosition);
	void shutdown();

private:

	struct TrailSample {
		Vector2 position;
		float age = 0.0f;
		float life = 0.75f;
		float startSize = 8.0f;
	};

	struct PointVertex {
		float x;
		float y;
		float size;
		float alpha;
	};

private:
	std::vector<PointVertex> m_verticesCache;
	std::vector<TrailSample> m_trail;
	ShaderProgram m_shader;

	GLuint m_vao = 0;
	GLuint m_vbo = 0;

	float m_spawnAccumulator = 0.0f;

	static constexpr float TRAIL_SPAWN_STEP = 0.005f;
	static constexpr float MIN_SPEED_FOR_TRAIL = 0.01f;
	static constexpr int MAX_TRAIL_SAMPLES = 2500;
};