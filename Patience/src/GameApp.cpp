// CLIENT

#include <Rhombus/Core/EntryPoint.h>

#include "GameLayer.h"

namespace rhombus
{

	class Game : public Application
	{
	public:
		Game(const ApplicationSpecification& specification)
			: Application(specification)
		{
			PushLayer(new GameLayer());
		}

		~Game() {

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.name = "Patience";
		spec.commandLineArgs = args;

		return new Game(spec);
	}
}