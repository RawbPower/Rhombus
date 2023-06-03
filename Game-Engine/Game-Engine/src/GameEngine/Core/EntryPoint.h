#pragma once

#ifdef GE_PLATFORM_WINDOWS

extern ge::Application* ge::CreateApplication();
	
int main(int argc, char** argv) {

	ge::Log::Init();
	//ge::Log::GetCoreLogger()->warn("Initialized Log!");
	//ge::Log::GetClientLogger()->info("Hello!");
	GE_CORE_WARN("Initialized Log!");
	int a = 4;
	GE_INFO("Hello! Var={0}", a);

	//printf("Game Engine\n");
	auto app = ge::CreateApplication();
	app->Run();
	delete app;
}

#endif