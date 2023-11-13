// CLIENT

#include <Rhombus.h>
#include <Rhombus/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "EditorLayer.h"

namespace rhombus
{

	class RhombusEditor : public Application
	{
	public:
		RhombusEditor() : Application("Rhombus Editor")
		{
			PushLayer(new EditorLayer());
		}

		~RhombusEditor() {

		}
	};

	Application* CreateApplication() {

		return new RhombusEditor();
	}
}