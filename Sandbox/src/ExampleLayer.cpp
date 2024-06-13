#include "ExampleLayer.h"

#include "imgui/imgui.h"

#include "Platform/OpenGL/OpenGLShader.h"

ExampleLayer::ExampleLayer()
	: Layer("ExampleLayer"), m_CameraController(1280.0f / 720.0f, true)
{
	/* Vertex Array (required for core OpenGL profile) */
	m_VertexArray = rhombus::VertexArray::Create();
	m_VertexArray->Bind();

	/* Vertex Buffer */

	// 3D coordinates and there are 3 of them
	// OpenGl default clip space is -1 to 1 x, y, z
	float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,				// vertex coordinate on each row
		0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
	};

	rhombus::Ref<rhombus::VertexBuffer> vertexBuffer = rhombus::VertexBuffer::Create(vertices, sizeof(vertices));

	rhombus::BufferLayout layout = {
		{ rhombus::ShaderDataType::Float3, "a_Position" },
		{ rhombus::ShaderDataType::Float4, "a_Color" },
	};

	vertexBuffer->SetLayout(layout);

	// Tell OpenGL what the layout of the buffer is
	m_VertexArray->AddVertexBuffer(vertexBuffer);


	/* Index Buffer (gives index to vertices, describes what order to draw vertices) */

	uint32_t indices[3] = { 0, 1, 2 };		// 3 points in the triangle
	rhombus::Ref<rhombus::IndexBuffer> indexBuffer = rhombus::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
	m_VertexArray->SetIndexBuffer(indexBuffer);

	// Square vertex array test

	m_SquareVA = rhombus::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f,		// 3 vertex coordinate on each row and 2 texture coords
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
	};

	rhombus::Ref<rhombus::VertexBuffer> squareVB = rhombus::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

	squareVB->SetLayout({
		{ rhombus::ShaderDataType::Float3, "a_Position" },
		{ rhombus::ShaderDataType::Float2, "a_TexCoord" }
		});

	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	rhombus::Ref<rhombus::IndexBuffer> squareIB = rhombus::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
	m_SquareVA->SetIndexBuffer(squareIB);

	/* Shader (If we do nothing GPU drivers will make a default one) */

	// Shader source code
	// R prefix allows multiple line strings
	std::string vertexSrc = R"(
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

	m_Shader = rhombus::Shader::Create("VertexPosColor", vertexSrc, pixelSrc);


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

	m_FlatColorShader = rhombus::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderPixelSrc);

	auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

	m_Texture = rhombus::Texture2D::Create("assets/textures/Checkerboard.png");
	m_BlendTexture = rhombus::Texture2D::Create("assets/textures/ChernoLogo.png");

	std::dynamic_pointer_cast<rhombus::OpenGLShader>(textureShader)->Bind();
	std::dynamic_pointer_cast<rhombus::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);		// 0 is the texure slot of m_Texture
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(rhombus::DeltaTime dt)
{
	// Update
	m_CameraController.OnUpdate(dt);

	// Render
	rhombus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	rhombus::RenderCommand::Clear();

	rhombus::Renderer::BeginScene(m_CameraController.GetCamera());

	static rhombus::Mat4 scale = rhombus::math::Scale(rhombus::Mat4::Identity(), rhombus::Vec3(0.1f));

	// Set up uniform
	std::dynamic_pointer_cast<rhombus::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<rhombus::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

	// Render grid of blue squares
	for (int x = -10; x < 11; x++) {
		for (int y = -10; y < 11; y++) {
			rhombus::Vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
			rhombus::Mat4 transform = rhombus::math::Translate(rhombus::Mat4::Identity(), pos) * scale;
			rhombus::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
		}
	}

	auto textureShader = m_ShaderLibrary.Get("Texture");

	m_Texture->Bind();
	rhombus::Renderer::Submit(textureShader, m_SquareVA, rhombus::math::Scale(rhombus::Mat4::Identity(), rhombus::Vec3(1.5f)));

	m_BlendTexture->Bind();
	rhombus::Renderer::Submit(textureShader, m_SquareVA, rhombus::math::Scale(rhombus::Mat4::Identity(), rhombus::Vec3(1.5f)));

	// Render triangle
	//rhombus::Renderer::Submit(m_Shader, m_VertexArray);

	rhombus::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", m_SquareColor.ToPtr());
	ImGui::End();
}

void ExampleLayer::OnEvent(rhombus::Event& e)
{
	m_CameraController.OnEvent(e);
}