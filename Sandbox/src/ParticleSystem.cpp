#include "ParticleSystem.h"

#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

ParticleSystem::ParticleSystem(uint32_t maxParticles)
	: m_poolIndex(maxParticles-1)
{
	m_particlePool.resize(maxParticles);
}

void ParticleSystem::OnUpdate(rhombus::DeltaTime dt)
{
	for (auto& particle : m_particlePool)
	{
		if (!particle.active)
		{
			continue;
		}

		if (particle.lifeRemaining <= 0.0f)
		{
			particle.active = false;
			continue;
		}

		particle.lifeRemaining -= dt;
		particle.position += particle.velocity * (float)dt;
		particle.rotation += 0.01f * dt;
	}
}

void ParticleSystem::OnRender(rhombus::OrthographicCamera& camera)
{
	rhombus::Renderer2D::BeginScene(camera);
	for (auto& particle : m_particlePool)
	{
		if (!particle.active)
		{
			continue;
		}

		// Fade away particles
		float life = particle.lifeRemaining / particle.lifetime;
		glm::vec4 color = glm::lerp(particle.colorEnd, particle.colorBegin, life);

		float size = glm::lerp(particle.sizeEnd, particle.sizeBegin, life);
		glm::vec3 particlePosition = { particle.position.x, particle.position.y, 0.2f };
		rhombus::Renderer2D::DrawQuad(particlePosition, particle.rotation, { size, size }, color);
	}
	rhombus::Renderer2D::EndScene();
}

void ParticleSystem::Emit(const ParticleParams& params)
{
	Particle& particle = m_particlePool[m_poolIndex];
	particle.active = true;
	particle.position = params.position;
	particle.rotation = Random::Randf() * 2.0f * glm::pi<float>();

	// Velocity
	particle.velocity = params.velocity;
	particle.velocity.x += params.velocityVariation.x * (Random::Randf() - 0.5f);
	particle.velocity.y += params.velocityVariation.y * (Random::Randf() - 0.5f);

	// Color
	particle.colorBegin = params.colorBegin;
	particle.colorEnd = params.colorEnd;

	particle.lifetime = params.lifetime;
	particle.lifeRemaining = params.lifetime;
	particle.sizeBegin = params.sizeBegin + params.sizeVariation * (Random::Randf() - 0.5f);
	particle.sizeEnd = params.sizeEnd;

	m_poolIndex = --m_poolIndex % m_particlePool.size();
}