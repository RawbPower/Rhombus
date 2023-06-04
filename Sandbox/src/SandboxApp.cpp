// CLIENT

#include <GameEngine.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Rhombus::Layer {
public:
	ExampleLayer()
		: Layer("Example"), m_CameraController(1280.0f / 720.0f, true)
	{
		/* Vertex Array (required for core OpenGL profile) */
		//m_VertexArray.reset(Rhombus::VertexArray::Create());
		//m_VertexArray->Bind();

		/* Vertex Buffer */

		// 3D coordinates and there are 3 of them
		// OpenGl default clip space is -1 to 1 x, y, z
		/*float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,				// vertex coordinate on each row
			0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Rhombus::Ref<Rhombus::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Rhombus::VertexBuffer::Create(vertices, sizeof(vertices)));

		Rhombus::BufferLayout layout = {
			{ Rhombus::ShaderDataType::Float3, "a_Position" },
			{ Rhombus::ShaderDataType::Float4, "a_Color" },
		};

		vertexBuffer->SetLayout(layout);

		// Tell OpenGL what the layout of the buffer is
		m_VertexArray->AddVertexBuffer(vertexBuffer);*/


		/* Index Buffer (gives index to vertices, describes what order to draw vertices) */

		/*uint32_t indices[3] = {0, 1, 2};		// 3 points in the triangle
		Rhombus::Ref<Rhombus::IndexBuffer> indexBuffer;
		indexBuffer.reset(Rhombus::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		// Square vertex array test

		m_SquareVA.reset(Rhombus::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,	0.0f, 0.0f,		// 3 vertex coordinate on each row and 2 texture coords
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		Rhombus::Ref<Rhombus::VertexBuffer> squareVB;
		squareVB.reset(Rhombus::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

		squareVB->SetLayout({
			{ Rhombus::ShaderDataType::Float3, "a_Position" },
			{ Rhombus::ShaderDataType::Float2, "a_TexCoord" }
			});

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Rhombus::Ref<Rhombus::IndexBuffer> squareIB;
		squareIB.reset(Rhombus::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);*/

		/* Shader (If we do nothing GPU drivers will make a default one) */

		// Shader source code
		// R prefix allows multiple line strings
		/* std::string vertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main() {
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		
		)";

		std::string pixelSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main() {
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		
		)";

		m_Shader = Rhombus::Shader::Create("VertexPosColor", vertexSrc, pixelSrc);


		std::string flatColorShaderVertexSrc = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main() {
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		
		)";

		std::string flatColorShaderPixelSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;

			void main() {
				color = vec4(u_Color, 1.0);
			}
		
		)";

		m_FlatColorShader = Rhombus::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderPixelSrc);

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Rhombus::Texture2D::Create("assets/textures/Checkerboard.png");
		m_BlendTexture = Rhombus::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Rhombus::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Rhombus::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);*/		// 0 is the texure slot of m_Texture
	}

	void OnUpdate(Rhombus::DeltaTime dt) override
	{
		// Update
		/*m_CameraController.OnUpdate(dt);

		// Render
		Rhombus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Rhombus::RenderCommand::Clear();

		Rhombus::Renderer::BeginScene(m_CameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		// Set up uniform
		std::dynamic_pointer_cast<Rhombus::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Rhombus::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		// Render grid of blue squares
		for (int x = -10; x < 11; x++) {
			for (int y = -10; y < 11; y++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Rhombus::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Rhombus::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_BlendTexture->Bind();
		Rhombus::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Render triangle
		//Rhombus::Renderer::Submit(m_Shader, m_VertexArray);

		Rhombus::Renderer::EndScene();*/
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Rhombus::Event& e) override
	{
		//m_CameraController.OnEvent(e);
	}

	bool OnKeyPressedEvent(Rhombus::KeyPressedEvent& event) 
	{

	}

private:
	Rhombus::ShaderLibrary m_ShaderLibrary;

	Rhombus::Ref<Rhombus::Shader> m_Shader;
	Rhombus::Ref<Rhombus::VertexArray> m_VertexArray;

	Rhombus::Ref<Rhombus::Shader> m_FlatColorShader;
	Rhombus::Ref<Rhombus::VertexArray> m_SquareVA;

	Rhombus::Ref<Rhombus::Texture2D> m_Texture, m_BlendTexture;

	Rhombus::OrthographicCameraController m_CameraController;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};


class Sandbox : public Rhombus::Application
{
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Rhombus::Application* Rhombus::CreateApplication() {

	return new Sandbox();
}