// CLIENT

#include <Rhombus/Core/EntryPoint.h>

#include "GameLayer.h"

#define GAME_EDITOR 1

#if GAME_EDITOR
#include "EditorLayer.h";
#include "Rhombus/Scenes/Scene.h";
#include "PatienceScene.h";
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
			std::function<Ref<Scene>()> sceneCreationCB = []() { return CreateRef<PatienceScene>(); };
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
		ApplicationSpecification spec;
		spec.name = "Patience";
#endif

		return new Game(spec);
	}
}