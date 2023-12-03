#pragma once

#ifdef RB_PLATFORM_WINDOWS

extern rhombus::Application* rhombus::CreateApplication();
	
int main(int argc, char** argv) {

	rhombus::Log::Init();

	std::string versionName = "";

	RB_PROFILE_BEGIN_SESSION("Startup", "RhombusProfile-Startup.json");
	auto app = rhombus::CreateApplication();
	RB_PROFILE_END_SESSION();

	RB_PROFILE_BEGIN_SESSION("Runtime", "RhombusProfile-Runtime.json");
	app->Run();
	RB_PROFILE_END_SESSION();

	RB_PROFILE_BEGIN_SESSION("Shutdown", "RhombusProfile-Shutdown.json");
	delete app;
	RB_PROFILE_END_SESSION();
}

#endif