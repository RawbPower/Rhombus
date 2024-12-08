// CLIENT

#include <Rhombus/Core/EntryPoint.h>

#include "GameLayer.h"
#include "Rhombus/Core/Log.h"

#define GAME_EDITOR 1

#if GAME_EDITOR
#include "EditorLayer.h";
#include "Rhombus/Scenes/Scene.h";
#include "GameScene.h";
#include "GameEditorExtension.h";
#endif

namespace rhombus
{

	class Game : public Application
	{
	public:
		Game(const ApplicationSpecification& specification)
			: Application(specification)
		{
#if GAME_EDITOR
			std::function<Ref<Scene>()> sceneCreationCB = []() { return CreateRef<GameScene>(); };
			std::function<Ref<EditorExtension>()> editorExtensionCB = []() { return CreateRef<GameEditorExtension>(); };
			PushLayer(new EditorLayer(sceneCreationCB, editorExtensionCB));
#else
			PushLayer(new GameLayer());
#endif
		}

		~Game() {

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.commandLineArgs = args;

#if GAME_EDITOR
		spec.name = "Rhombus Editor";
		spec.width = 1280;
		spec.height = 720;
		spec.fullscreen = true;
#else
		auto commandLineArgs = args;
		bool bLoadedProject = false;
		if (commandLineArgs.count > 1)
		{
			auto projectFilePath = commandLineArgs[1];
			if (Project::Load(projectFilePath))
			{
				spec.name = Project::GetName();
				spec.width = Project::GetWindowWidth();
				spec.height = Project::GetWindowHeight();
				spec.fullscreen = false;
				bLoadedProject = true;
			}
		}

		if (!bLoadedProject)
		{
			Log::Error("Invalid project file!\n");
		}
#endif

		return new Game(spec);
	}
}