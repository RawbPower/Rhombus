#pragma once

#include "Rhombus.h"

#include "ParticleSystem.h"

class Sandbox2D : public rhombus::Layer
{

public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(rhombus::DeltaTime dt) override;
	virtual void OnImGuiRender() override;
	void OnEvent(rhombus::Event& e) override;

private:
	// Temp (These needs to be abstracted away)
	rhombus::Ref<rhombus::Shader> m_FlatColourShader;
	rhombus::Ref<rhombus::VertexArray> m_SquareVA;

	rhombus::Ref<rhombus::Texture2D> m_CheckerboardTexture;

	rhombus::OrthographicCameraController m_CameraController;
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

	ParticleSystem m_ParticleSystem;
	ParticleParams m_Particle;
};
