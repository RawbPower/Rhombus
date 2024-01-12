#pragma once

namespace rhombus {

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();
	private:
		static void InitLua();
		static void ShutdownLua();
	};

}
