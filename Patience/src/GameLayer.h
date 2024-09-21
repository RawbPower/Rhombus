#pragma once

#include "Rhombus.h"

class GameLayer : public rhombus::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(rhombus::DeltaTime dt) override;
	virtual void OnImGuiRender() override;
	void OnEvent(rhombus::Event& e) override;

	void RenderInWindow();

private:
	bool OnKeyPressed(KeyPressedEvent& e);
	bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
	bool OnMouseMoved(MouseMovedEvent& e);
	bool OnWindowResized(WindowResizeEvent& e);

	bool OpenProject();
	void OpenProject(const std::filesystem::path& path);

	void OpenScene();
	void OpenScene(const std::filesystem::path& path);

	Ref<Scene> m_ActiveScene;
	Ref<Framebuffer> m_Framebuffer;
};
