#include "rbpch.h"
#include "ScriptEngine.h"
#include "ScriptRegistry.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace rhombus
{
	static lua_State* L = nullptr;

	bool CheckLua(lua_State* state, int r)
	{
		if (r != LUA_OK)
		{
			std::string errormsg = lua_tostring(state, -1);
			RB_CORE_ERROR(errormsg);
			return false;
		}
		return true;
	}

	void ScriptEngine::Init()
	{
		InitLua();
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownLua();
		delete L;
	}

	void ScriptEngine::InitLua()
	{
		L = luaL_newstate();
		luaL_openlibs(L);

		ScriptRegistry::RegisterFunctions(L);

		int r = luaL_dofile(L, "Resources/Scripts/Main.lua");

		if (CheckLua(L, r))
		{
			lua_getglobal(L, "PrintMessage");
			if (lua_isfunction(L, -1))
			{
				if (CheckLua(L, lua_pcall(L, 0, 1, 0)))
				{
					if (lua_isstring(L, -1))
					{
						RB_CORE_INFO("[LUA] {0}", lua_tostring(L, -1));
					}
				}
			}

			lua_getglobal(L, "PrintInt");
			if (lua_isfunction(L, -1))
			{
				lua_pushnumber(L, 345.36);
				if (CheckLua(L, lua_pcall(L, 1, 1, 0)))
				{
					if (lua_isstring(L, -1))
					{
						RB_CORE_INFO("[LUA] {0}", lua_tostring(L, -1));
					}
				}
			}

			lua_getglobal(L, "PrintInts");
			if (lua_isfunction(L, -1))
			{
				lua_pushnumber(L, 345.36);
				lua_pushnumber(L, 1.45);
				if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
				{
					if (lua_isstring(L, -1))
					{
						RB_CORE_INFO("[LUA] {0}", lua_tostring(L, -1));
					}
				}
			}

			lua_getglobal(L, "PrintCustomMessage");
			if (lua_isfunction(L, -1))
			{
				lua_pushstring(L, "Hello from C++!");
				if (CheckLua(L, lua_pcall(L, 1, 1, 0)))
				{
					if (lua_isstring(L, -1))
					{
						RB_CORE_INFO("[LUA] {0}", lua_tostring(L, -1));
					}
				}
			}

			lua_getglobal(L, "CallHostFunction");
			if (lua_isfunction(L, -1))
			{
				lua_pushnumber(L, 5);
				lua_pushnumber(L, 6);
				if (CheckLua(L, lua_pcall(L, 2, 1, 0)))
				{
					if (lua_isnumber(L, -1))
					{
						RB_CORE_INFO("Lua used HostFunction to got {0}", lua_tonumber(L, -1));
					}
				}
			}
		}
	}

	void ScriptEngine::ShutdownLua()
	{
		lua_close(L);
		L = nullptr;
	}
}