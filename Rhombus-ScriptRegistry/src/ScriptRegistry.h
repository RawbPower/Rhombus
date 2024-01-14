#pragma once

extern "C"
{
	class lua_State;
}

namespace rhombus
{
	class ScriptRegistry
	{
	public:
		static void RegisterFunctions(lua_State* L);
	};

	extern "C" __declspec(dllexport) int luaopen_rhombus(lua_State * L);
}
