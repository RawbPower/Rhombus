#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(960.0f / 540.0f, true)
{

}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = rhombus::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(rhombus::DeltaTime dt)
{
	RB_PROFILE_FUNCTION();

	// Update
	{
		RB_PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(dt);
	}

	// Render
	{
		RB_PROFILE_SCOPE("Renderer Prep");
		rhombus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		rhombus::RenderCommand::Clear();
	}

	{
		RB_PROFILE_SCOPE("Renderer Draw");
		rhombus::Renderer2D::BeginScene(m_CameraController.GetCamera());
		rhombus::Renderer2D::DrawQuad({ -1.0f, 0.0f }, 0.0f, { 0.8, 0.8 }, { 0.8f, 0.2f, 0.3f, 1.0f });
		rhombus::Renderer2D::DrawQuad({ 0.5f, -0.5f }, 3.14f / 3.0f, { 0.5, 0.75 }, { 0.3f, 0.1f, 0.8f, 1.0f });
		rhombus::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, 0.0f, { 10.0, 10.0 }, m_CheckerboardTexture);
		rhombus::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	RB_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(rhombus::Event& e)
{
	m_CameraController.OnEvent(e);
}