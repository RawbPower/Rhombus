#pragma once

#ifdef RB_PLATFORM_WINDOWS

extern Rhombus::Application* Rhombus::CreateApplication();
	
int main(int argc, char** argv) {

	Rhombus::Log::Init();
	//rhombus::Log::GetCoreLogger()->warn("Initialized Log!");
	//rhombus::Log::GetClientLogger()->info("Hello!");
	RB_CORE_WARN("Initialized Log!");
	int a = 4;
	RB_INFO("Hello! Var={0}", a);

	//printf("Game Engine\n");
	auto app = Rhombus::CreateApplication();
	app->Run();
	delete app;
}

#endif