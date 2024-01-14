#pragma once

extern "C"
{
	class lua_State;
}

namespace rhombus
{
	class ScriptGlue
	{
	public:
		static void RegisterFunctions(lua_State* L);
	};
}
