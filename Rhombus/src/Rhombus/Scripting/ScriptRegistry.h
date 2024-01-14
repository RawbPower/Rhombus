#pragma once

class lua_State;

namespace rhombus
{
	class ScriptRegistry
	{
	public:
		static void RegisterFunctions(lua_State* L);
	};
}
