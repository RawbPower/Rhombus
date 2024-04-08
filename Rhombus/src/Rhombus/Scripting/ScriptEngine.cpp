#include "rbpch.h"
#include "ScriptEngine.h"
#include "ScriptGlue.h"

#include "Rhombus/Project/Project.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace rhombus
{
	static lua_State* L = nullptr;
	static Scene* sceneContext = nullptr;

	bool CheckLua(lua_State* state, int r)
	{
		if (r != LUA_OK)
		{
			std::string errormsg = lua_tostring(state, -1);
			RB_CORE_ERROR("[Lua Error] {0}", errormsg);
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

		ScriptGlue::RegisterFunctions(L);

#if 0
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
#endif
	}

	void ScriptEngine::ShutdownLua()
	{
		lua_close(L);
		L = nullptr;
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		sceneContext = scene;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		sceneContext = nullptr;
	}

	void ScriptEngine::SetupEntity(Entity entity)
	{
		const auto& scriptComponent = entity.GetComponent<ScriptComponent>();

		lua_getglobal(L, scriptComponent.m_scriptName.c_str());
		UUID entityID = entity.GetUUID();
		uint32_t entityID32a = (uint32_t)((entityID & 0xFFFFFFFF00000000LL) >> 32);
		uint32_t entityID32b = (uint32_t)(entityID & 0xFFFFFFFFLL);

		lua_pushnumber(L, (int)entityID32a);
		lua_setfield(L, -2, "entityIDa");

		lua_pushnumber(L, (int)entityID32b);
		lua_setfield(L, -2, "entityIDb");
	}

	void ScriptEngine::OnInitEntity(Entity entity)
	{
		const auto& scriptComponent = entity.GetComponent<ScriptComponent>();
		
		// Todo set path in component
		std::string sciptPath = Project::GetScriptDirectory().string() + "\\" + scriptComponent.m_scriptName + ".lua";
		int r = luaL_dofile(L, sciptPath.c_str());

		if (CheckLua(L, r))
		{
			SetupEntity(entity);

			lua_getglobal(L, scriptComponent.m_scriptName.c_str());
			lua_getfield(L, -1, "Init");
			if (lua_isfunction(L, -1))
			{
				lua_pushvalue(L, -2);
				CheckLua(L, lua_pcall(L, 1, 0, 0));
			}
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, DeltaTime dt)
	{
		const auto& scriptComponent = entity.GetComponent<ScriptComponent>();

		// Todo set path in component
		std::string sciptPath = Project::GetScriptDirectory().string() + "\\" + scriptComponent.m_scriptName + ".lua";
		int r = luaL_loadfile(L, sciptPath.c_str());

		if (CheckLua(L, r))
		{
			SetupEntity(entity);

			lua_getglobal(L, scriptComponent.m_scriptName.c_str());
			lua_getfield(L, -1, "Update");
			if (lua_isfunction(L, -1))
			{
				lua_pushvalue(L, -2);
				lua_pushnumber(L, (float)dt);
				CheckLua(L, lua_pcall(L, 2, 0, 0));
			}
		}
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return sceneContext;
	}

	void ScriptEngine::OnMouseEnterArea(Entity entity)
	{
		if (entity.HasComponent<ScriptComponent>())
		{
			const auto& scriptComponent = entity.GetComponent<ScriptComponent>();

			// Todo set path in component
			std::string sciptPath = Project::GetScriptDirectory().string() + "\\" + scriptComponent.m_scriptName + ".lua";
			int r = luaL_loadfile(L, sciptPath.c_str());

			if (CheckLua(L, r))
			{
				SetupEntity(entity);

				lua_getglobal(L, scriptComponent.m_scriptName.c_str());
				lua_getfield(L, -1, "OnMouseEnterArea");
				if (lua_isfunction(L, -1))
				{
					lua_pushvalue(L, -2);
					CheckLua(L, lua_pcall(L, 1, 0, 0));
				}
			}
		}
	}

	void ScriptEngine::OnMouseExitArea(Entity entity)
	{
		if (entity.HasComponent<ScriptComponent>())
		{
			const auto& scriptComponent = entity.GetComponent<ScriptComponent>();

			// Todo set path in component
			std::string sciptPath = Project::GetScriptDirectory().string() + "\\" + scriptComponent.m_scriptName + ".lua";
			int r = luaL_loadfile(L, sciptPath.c_str());

			if (CheckLua(L, r))
			{
				SetupEntity(entity);

				lua_getglobal(L, scriptComponent.m_scriptName.c_str());
				lua_getfield(L, -1, "OnMouseExitArea");
				if (lua_isfunction(L, -1))
				{
					lua_pushvalue(L, -2);
					CheckLua(L, lua_pcall(L, 1, 0, 0));
				}
			}
		}
	}
}