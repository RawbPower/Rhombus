#include "GameLayer.h"

#include "PatienceScene.h"
#include "Rhombus/ECS/SceneSerializer.h"
#include "Rhombus/Utils/PlatformUtils.h"

#include "imgui/imgui.h"

GameLayer::GameLayer()
	: Layer("GameLayer")
{
	
}

void GameLayer::OnAttach()
{
	RB_PROFILE_FUNCTION();

	// Framebuffer
	m_ActiveScene = CreateRef<Scene>();
	m_ActiveScene->OnViewportResize(960, 540);

	auto commandLineArgs = Application::Get().GetSpecification().commandLineArgs;
	if (commandLineArgs.count > 1)
	{
		auto projectFilePath = commandLineArgs[1];
		OpenProject(projectFilePath);
	}
	else
	{
		if (!OpenProject())
		{
			Application::Get().Close();
		}
	}
}

void GameLayer::OnDetach()
{
	RB_PROFILE_FUNCTION();
}

void GameLayer::OnUpdate(rhombus::DeltaTime dt)
{
	RB_PROFILE_FUNCTION();

	// Render
	Renderer2D::ResetStats();
	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	RenderCommand::Clear();

	m_ActiveScene->OnUpdateRuntime(dt);
}

void GameLayer::OnImGuiRender()
{
}

void GameLayer::OnEvent(rhombus::Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<KeyPressedEvent>(RB_BIND_EVENT_FN(GameLayer::OnKeyPressed));
	dispatcher.Dispatch<MouseButtonPressedEvent>(RB_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
	dispatcher.Dispatch<MouseButtonReleasedEvent>(RB_BIND_EVENT_FN(GameLayer::OnMouseButtonReleased));
	dispatcher.Dispatch<MouseMovedEvent>(RB_BIND_EVENT_FN(GameLayer::OnMouseMoved));
}

// Try KeyTypedEvent
bool GameLayer::OnKeyPressed(KeyPressedEvent& e)
{
	return false;
}

bool GameLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
{
	m_ActiveScene->OnMouseButtonPressed(e.GetMouseButton());

	return false;
}

bool GameLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
{
	m_ActiveScene->OnMouseButtonReleased(e.GetMouseButton());

	return false;
}

bool GameLayer::OnMouseMoved(MouseMovedEvent& e)
{
	m_ActiveScene->OnMouseMoved(e.GetX(), e.GetY());

	return false;
}

bool GameLayer::OnWindowResized(WindowResizeEvent& e)
{
	return false;
}

bool GameLayer::OpenProject()
{
	std::string filepath = FileDialogs::OpenFile("Rhombus Project (*.rproj)\0*.rproj\0");
	if (filepath.empty())
	{
		return false;
	}

	OpenProject(filepath);
	return true;
}

void GameLayer::OpenProject(const std::filesystem::path& path)
{
	if (Project::Load(path))
	{
		auto startScenePath = Project::GetAssetFileSystemPath(Project::GetActive()->GetConfig().StartScene);
		OpenScene(startScenePath);
	}
}

void GameLayer::OpenScene()
{
	std::string filepath = FileDialogs::OpenFile("Rhombus Scene (*.rb)\0*.rb\0");
	if (!filepath.empty())
	{
		OpenScene(filepath);
	}
}

void GameLayer::OpenScene(const std::filesystem::path& path)
{
	Ref<Scene> newScene = CreateRef<PatienceScene>();
	SceneSerializer serializer(newScene);
	if (serializer.Deserialize(path.string()))
	{
		m_ActiveScene = newScene;
		m_ActiveScene->OnViewportResize(960, 540);
		m_ActiveScene->OnRuntimeStart();
	}
}