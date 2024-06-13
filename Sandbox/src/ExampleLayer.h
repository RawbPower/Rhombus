#pragma once

#include "Rhombus.h"

class ExampleLayer : public rhombus::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(rhombus::DeltaTime dt) override;
	virtual void OnImGuiRender() override;
	void OnEvent(rhombus::Event& e) override;

private:
	rhombus::ShaderLibrary m_ShaderLibrary;

	rhombus::Ref<rhombus::Shader> m_Shader;
	rhombus::Ref<rhombus::VertexArray> m_VertexArray;

	rhombus::Ref<rhombus::Shader> m_FlatColorShader;
	rhombus::Ref<rhombus::VertexArray> m_SquareVA;

	rhombus::Ref<rhombus::Texture2D> m_Texture, m_BlendTexture;

	rhombus::OrthographicCameraController m_CameraController;

	rhombus::Vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};
