// CLIENT

#include <Rhombus.h>
#include <Rhombus/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public rhombus::Application
{
public:
	Sandbox() {
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox() {

	}
};

rhombus::Application* rhombus::CreateApplication() {

	return new Sandbox();
}