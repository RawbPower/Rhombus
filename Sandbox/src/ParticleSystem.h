#pragma once

#include <Rhombus.h>

struct ParticleParams
{
	glm::vec2 position;
	glm::vec2 velocity, velocityVariation;
	glm::vec4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifetime;
};

class ParticleSystem
{
public:
	ParticleSystem(uint32_t maxParticles = 1000);

	void OnUpdate(rhombus::DeltaTime dt);
	void OnRender(rhombus::OrthographicCamera& camera);

	void Emit(const ParticleParams& params);
private:
	struct Particle
	{
		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec4 colorBegin, colorEnd;
		float rotation;
		float sizeBegin, sizeEnd;

		float lifetime = 1.0f;
		float lifeRemaining = 0.0f;

		bool active = false;
	};

	std::vector<Particle> m_particlePool;
	uint32_t m_poolIndex;
};
