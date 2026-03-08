#include "TrailRenderer.h"

#include <algorithm>
#include <cstddef>

TrailRenderer::TrailRenderer() = default;

TrailRenderer::~TrailRenderer() {
	shutdown();
}

bool TrailRenderer::initialize(const char* vertexShaderPath, const char* fragmentShaderPath) {
	m_shader.loadFromFiles(vertexShaderPath, fragmentShaderPath);

	m_trail.reserve(MAX_TRAIL_SAMPLES);
	m_verticesCache.reserve(MAX_TRAIL_SAMPLES + 1);

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	const GLsizeiptr maxVertices = static_cast<GLsizeiptr>(MAX_TRAIL_SAMPLES + 1);
	const GLsizeiptr maxBytes = maxVertices * static_cast<GLsizeiptr>(sizeof(PointVertex));
	glBufferData(GL_ARRAY_BUFFER, maxBytes, nullptr, GL_STREAM_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(PointVertex), (void*)offsetof(PointVertex, x));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(PointVertex), (void*)offsetof(PointVertex, size));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(PointVertex), (void*)offsetof(PointVertex, alpha));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}

void TrailRenderer::update(const Vector2& pointPosition, float pointSpeed, float dt) {
	if (pointSpeed > MIN_SPEED_FOR_TRAIL) {
		m_spawnAccumulator += dt;

		while (m_spawnAccumulator >= TRAIL_SPAWN_STEP) {
			m_spawnAccumulator -= TRAIL_SPAWN_STEP/2;

			TrailSample sample;
			sample.position = pointPosition;
			sample.age = 0.0f;
			sample.life = 0.75f;
			sample.startSize = 5.0f;

			m_trail.push_back(sample);
		}
	}
	else {
		m_spawnAccumulator = 0.0f;
	}

	for (auto& sample : m_trail) 
		sample.age += dt;

	m_trail.erase(
		std::remove_if(
			m_trail.begin(),
			m_trail.end(),
			[](const TrailSample& sample) {
				return sample.age >= sample.life;
			}),
		m_trail.end());

	if (m_trail.size() > MAX_TRAIL_SAMPLES) {
		m_trail.erase(
			m_trail.begin(),
			m_trail.begin() + static_cast<long long>(m_trail.size() - MAX_TRAIL_SAMPLES));
	}
}

void TrailRenderer::render(const Vector2& pointPosition) {
	const std::size_t trailCount = m_trail.size();
	const std::size_t totalCount = trailCount + 1;

	m_verticesCache.resize(totalCount);

	for (std::size_t i = 0; i < trailCount; ++i)
	{
		const auto& sample = m_trail[i];

		const float t = sample.age / sample.life;
		const float alpha = 1.0f - t;
		const float size = sample.startSize + 22.0f * t;

		m_verticesCache[i] = PointVertex{
			sample.position.x,
			sample.position.y,
			size,
			alpha
		};
	}

	const PointVertex headVertex = {
		pointPosition.x,
		pointPosition.y,
		18.0f,
		1.0f
	};
	m_verticesCache[trailCount] = headVertex;

	m_shader.use();
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	const GLsizeiptr bytes = static_cast<GLsizeiptr>(totalCount * sizeof(PointVertex));
	glBufferSubData(GL_ARRAY_BUFFER, 0, bytes, m_verticesCache.data());

	if (trailCount > 0)
	{
		m_shader.setVec3("uColor", 0.20f, 0.75f, 1.00f);
		glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(trailCount));
	}

	m_shader.setVec3("uColor", 0.25f, 0.30f, 0.50f);
	glDrawArrays(GL_POINTS, static_cast<GLint>(trailCount), 1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TrailRenderer::shutdown()
{
		if (m_vbo != 0) {
			glDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}

		if (m_vao != 0) {
			glDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}
}
