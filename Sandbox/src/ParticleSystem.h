#pragma once

#include <Rhombus.h>

struct ParticleParams
{
	rhombus::Vec2 position;
	rhombus::Vec2 velocity, velocityVariation;
	rhombus::Color colorBegin, colorEnd;
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
		rhombus::Vec2 position;
		rhombus::Vec2 velocity;
		rhombus::Color colorBegin, colorEnd;
		float rotation;
		float sizeBegin, sizeEnd;

		float lifetime = 1.0f;
		float lifeRemaining = 0.0f;

		bool active = false;
	};

	std::vector<Particle> m_particlePool;
	uint32_t m_poolIndex;
	bool m_isAnyParticleActive = false;
};
