#include "rbpch.h"
#include "ScriptRegistry.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace rhombus
{
#define ADD_INTERNAL_CALL(L, Name) lua_register(L, #Name, Name)

	int HostFunction(lua_State* state)
	{
		RB_CORE_ASSERT(lua_gettop(state) == 2, "Invalid number of arguments passed to function");

		float a = (float)lua_tonumber(state, 1);		// indexed as if this is a fresh stack
		float b = (float)lua_tonumber(state	, 2);		// indexed as if this is a fresh stack
		float c = a * b;
		lua_pushnumber(state, c);		// Push result onto lua stack
		return 1;						// Number of return values that lua is expecting
	}

	void ScriptRegistry::RegisterFunctions(lua_State* L)
	{
		ADD_INTERNAL_CALL(L, HostFunction);
	}
}