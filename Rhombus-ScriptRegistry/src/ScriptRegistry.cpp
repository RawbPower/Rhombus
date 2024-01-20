#include "ScriptRegistry.h"
#include "Rhombus/Core/Log.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace rhombus
{
	int Multiply(lua_State* state)
	{
		RB_CORE_ASSERT(lua_gettop(state) == 2, "Invalid number of arguments passed to function");

		float a = (float)lua_tonumber(state, 1);		// indexed as if this is a fresh stack
		float b = (float)lua_tonumber(state	, 2);		// indexed as if this is a fresh stack
		float c = a * b;
		lua_pushnumber(state, c);		// Push result onto lua stack
		return 1;						// Number of return values that lua is expecting
	}

	int Add(lua_State* state)
	{
		RB_CORE_ASSERT(lua_gettop(state) == 2, "Invalid number of arguments passed to function");

		float a = (float)lua_tonumber(state, 1);		// indexed as if this is a fresh stack
		float b = (float)lua_tonumber(state, 2);		// indexed as if this is a fresh stack
		float c = a + b;
		lua_pushnumber(state, c);		// Push result onto lua stack
		return 1;						// Number of return values that lua is expecting
	}

	// List of functions to wrap.
	static const luaL_Reg functions[] =
	{
		{ "Multiply", Multiply },
		{ "Add", Add },
		{ NULL, NULL }
	};

	extern "C" int luaopen_rhombus(lua_State * L)
	{
		luaL_newlib(L, functions);

		return 1;
	}
}