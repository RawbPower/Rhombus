#include "rbpch.h"
#include "Renderer.h"
#include "Renderer2D.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace rhombus {

	Renderer::SceneData* Renderer::s_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RB_PROFILE_FUNCTION();

		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		s_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
		// Does nothing for now
	}

	// The vertex array will be submitted into a RenderCommand queue to be evaluated later and get rendered
	// For now it is much simpler
	// Shader needs to be a parameter in submit because it can change for different objects in the scene
	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const Mat4& transform)
	{
		shader->Bind();
		Mat4 viewProjectionMat = Mat4({ s_SceneData->ViewProjectionMatrix[0][0], s_SceneData->ViewProjectionMatrix[0][1], s_SceneData->ViewProjectionMatrix[0][2], s_SceneData->ViewProjectionMatrix[0][3] },
			{ s_SceneData->ViewProjectionMatrix[1][0], s_SceneData->ViewProjectionMatrix[1][1], s_SceneData->ViewProjectionMatrix[1][2], s_SceneData->ViewProjectionMatrix[1][3] },
			{ s_SceneData->ViewProjectionMatrix[2][0], s_SceneData->ViewProjectionMatrix[2][1], s_SceneData->ViewProjectionMatrix[2][2], s_SceneData->ViewProjectionMatrix[2][3] },
			{ s_SceneData->ViewProjectionMatrix[3][0], s_SceneData->ViewProjectionMatrix[3][1], s_SceneData->ViewProjectionMatrix[3][2], s_SceneData->ViewProjectionMatrix[3][3] });
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", viewProjectionMat);

		Mat4 transformMat = Mat4({ transform[0][0], transform[0][1], transform[0][2], transform[0][3] },
			{ transform[1][0], transform[1][1], transform[1][2], transform[1][3] },
			{ transform[2][0], transform[2][1], transform[2][2], transform[2][3] },
			{ transform[3][0], transform[3][1], transform[3][2], transform[3][3] });
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transformMat);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}