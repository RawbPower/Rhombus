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

		return new RhombusEditor(spec);
	}
}