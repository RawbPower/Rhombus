#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"

namespace rhombus {

	class Renderer {
	public:
		static void Init();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(OrthographicCamera& camera);		// We take in a reference to the camera because OrthographicCamera is a large class
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const Mat4& transform = Mat4::Identity());

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData 
		{
			Mat4 ViewProjectionMatrix;
		};

		static SceneData* s_SceneData;
	};
}
