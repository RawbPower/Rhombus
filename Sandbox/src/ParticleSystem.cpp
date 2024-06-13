#include "ParticleSystem.h"

#include "Rhombus/Math/Math.h"

ParticleSystem::ParticleSystem(uint32_t maxParticles)
	: m_poolIndex(maxParticles-1)
{
	m_particlePool.resize(maxParticles);
}

void ParticleSystem::OnUpdate(rhombus::DeltaTime dt)
{
	m_isAnyParticleActive = false;
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

		m_isAnyParticleActive = true;

		particle.lifeRemaining -= dt;
		particle.position += particle.velocity * (float)dt;
		particle.rotation += 0.01f * dt;
	}
}

void ParticleSystem::OnRender(rhombus::OrthographicCamera& camera)
{
	if (m_isAnyParticleActive)
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
			rhombus::Color color = rhombus::math::Lerp(particle.colorEnd, particle.colorBegin, life);

			float size = rhombus::math::Lerp(particle.sizeEnd, particle.sizeBegin, life);
			rhombus::Vec3 particlePosition = { particle.position.x, particle.position.y, 0.2f };
			rhombus::Renderer2D::DrawQuad(particlePosition, particle.rotation, { size, size }, color);
		}
		rhombus::Renderer2D::EndScene();
	}
}

void ParticleSystem::Emit(const ParticleParams& params)
{
	Particle& particle = m_particlePool[m_poolIndex];
	particle.active = true;
	particle.position = params.position;
	particle.rotation = Random::Randf() * 2.0f * rhombus::math::PI;

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