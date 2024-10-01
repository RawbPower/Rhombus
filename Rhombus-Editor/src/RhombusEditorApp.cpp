// CLIENT

#include <Rhombus/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "EditorLayer.h"

namespace rhombus
{

	class RhombusEditor : public Application
	{
	public:
		RhombusEditor(const ApplicationSpecification& specification)
			: Application(specification)
		{
			PushLayer(new EditorLayer());
		}

		~RhombusEditor() {

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args) 
	{
		ApplicationSpecification spec;
		spec.name = "Rhombus Editor";
		spec.commandLineArgs = args;

		// TODO: Get these from project data
#if RB_EDITOR
		spec.width = 1280;
		spec.height = 720;
		spec.fullscreen = true;
#else
		spec.width = 1280;
		spec.height = 720;
		spec.fullscreen = false;
#endif

		return new RhombusEditor(spec);
	}
}