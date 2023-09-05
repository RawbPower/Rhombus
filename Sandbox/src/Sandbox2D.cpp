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
	RB_PROFILE_FUNCTION();

	m_CheckerboardTexture = rhombus::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	RB_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(rhombus::DeltaTime dt)
{
	RB_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(dt);

	// Render
	rhombus::Renderer2D::ResetStats();
	{
		RB_PROFILE_SCOPE("Renderer Prep");
		rhombus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		rhombus::RenderCommand::Clear();
	}

	{
		RB_PROFILE_SCOPE("Renderer Draw");

		rhombus::Renderer2D::BeginScene(m_CameraController.GetCamera());
		rhombus::Renderer2D::DrawQuad({ 0.5f, -0.5f }, 3.14f / 3.0f, { 0.5, 0.75 }, { 0.3f, 0.1f, 0.8f, 1.0f });
		rhombus::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, 3.14f / 4.0f, { 1.0, 1.0 }, m_CheckerboardTexture, 20.0f);
		rhombus::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, 0.0f, { 10.0, 10.0 }, m_CheckerboardTexture, 10.0f);
		rhombus::Renderer2D::DrawQuad({ -1.0f, 0.0f }, 0.0f, { 0.8, 0.8 }, { 0.8f, 0.2f, 0.3f, 1.0f });
		rhombus::Renderer2D::EndScene();

		rhombus::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
				rhombus::Renderer2D::DrawQuad({ x, y }, 0.0f, { 0.45, 0.45 }, color);
			}
		}
		rhombus::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	RB_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = rhombus::Renderer2D::GetStats();
	ImGui::Text("Renderer 2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(rhombus::Event& e)
{
	m_CameraController.OnEvent(e);
}