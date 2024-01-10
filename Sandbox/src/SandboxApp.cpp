// CLIENT

#include <Rhombus.h>
#include <Rhombus/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public rhombus::Application
{
public:
	Sandbox(const rhombus::ApplicationSpecification& specification)
		: rhombus::Application(specification) 
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox() {

	}
};

rhombus::Application* rhombus::CreateApplication(ApplicationCommandLineArgs args) 
{
	ApplicationSpecification spec;
	spec.name = "Sandbox";
	spec.workingDirectory = "../Rhombus-Editor";
	spec.commandLineArgs = args;

	return new Sandbox(spec);
}