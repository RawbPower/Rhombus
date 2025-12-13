#pragma once

extern "C"
{
#include <lua.h>

	struct lua_State;
}

namespace rhombus
{
	class ScriptGlue
	{
	public:
		static void RegisterFunctions(lua_State* L);
	};
}
