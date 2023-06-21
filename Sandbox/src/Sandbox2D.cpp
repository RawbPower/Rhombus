#include "Sandbox2D.h"

// Needs to be abstracted
#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(960.0f / 540.0f, true)
{

}

void Sandbox2D::OnAttach()
{
	m_SquareVA = rhombus::VertexArray::Create();

	float squareVertices[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
	};

	rhombus::Ref<rhombus::VertexBuffer> squareVB;
	squareVB.reset(rhombus::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

	squareVB->SetLayout({
		{ rhombus::ShaderDataType::Float3, "a_Position" }
	});

	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	rhombus::Ref<rhombus::IndexBuffer> squareIB;
	squareIB.reset(rhombus::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);

	m_FlatColourShader = rhombus::Shader::Create("assets/shaders/FlatColour.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(rhombus::DeltaTime dt)
{
	// Update
	m_CameraController.OnUpdate(dt);

	// Render
	rhombus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	rhombus::RenderCommand::Clear();

	rhombus::Renderer::BeginScene(m_CameraController.GetCamera());

	// Set up uniform
	std::dynamic_pointer_cast<rhombus::OpenGLShader>(m_FlatColourShader)->Bind();
	std::dynamic_pointer_cast<rhombus::OpenGLShader>(m_FlatColourShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	rhombus::Renderer::Submit(m_FlatColourShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	rhombus::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(rhombus::Event& e)
{
	m_CameraController.OnEvent(e);
}