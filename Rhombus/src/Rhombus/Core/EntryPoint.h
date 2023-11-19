#pragma once

#ifdef RB_PLATFORM_WINDOWS

extern rhombus::Application* rhombus::CreateApplication();
	
int main(int argc, char** argv) {

	rhombus::Log::Init();

	std::string versionName = "";

	if (__cplusplus == 202101L) RB_CORE_INFO("C++23");
	else if (__cplusplus == 202002L) RB_CORE_INFO("C++20");
	else if (__cplusplus == 201703L) RB_CORE_INFO("C++17");
	else if (__cplusplus == 201402L) RB_CORE_INFO("C++14");
	else if (__cplusplus == 201103L) RB_CORE_INFO("C++11");
	else if (__cplusplus == 199711L) RB_CORE_INFO("C++98");
	else RB_CORE_INFO("pre-standard C++.");

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