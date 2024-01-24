#include "rbpch.h"
#include "ScriptGlue.h"

#include "Rhombus/Core/Input.h"
#include "Rhombus/Core/KeyCodes.h"
#include "Rhombus/ECS/Scene.h"
#include "Rhombus/Scripting/ScriptEngine.h"

#include "box2d/b2_body.h"

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

	int Log(lua_State* state)
	{
		RB_CORE_ASSERT(lua_gettop(state) == 1, "Invalid number of arguments passed to function");
		std::string logString = lua_tostring(state, 1);
		RB_CORE_INFO(logString);
		return 0;
	}

	int IsKeyDown(lua_State* state)
	{
		RB_CORE_ASSERT(lua_gettop(state) == 1, "Invalid number of arguments passed to function");

		int key = (int)lua_tonumber(state, 1);		// indexed as if this is a fresh stack
		bool keyPressed = Input::IsKeyPressed(key);
		lua_pushboolean(state, keyPressed);		// Push result onto lua stack
		return 1;						// Number of return values that lua is expecting
	}

	int ApplyLinearImpulse(lua_State* state)
	{
		RB_CORE_ASSERT(lua_gettop(state) == 3, "Invalid number of arguments passed to function");

		uint32_t entityID = (uint32_t)lua_tonumber(state, 1);		// indexed as if this is a fresh stack
		float impulseX = (float)lua_tonumber(state, 2);		// indexed as if this is a fresh stack
		float impulseY = (float)lua_tonumber(state, 3);		// indexed as if this is a fresh stack
		Scene* scene = ScriptEngine::GetSceneContext();
		RB_CORE_ASSERT(scene, "Invalid Scene in ApplyLinearImpulse");
		Entity entity = scene->GetEntityByUUID(entityID);
		std::string name = entity.GetComponent<TagComponent>().m_tag;
		RB_CORE_ASSERT(entity, "Invalid Entity in ApplyLinearImpulse");

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		// move to function
		b2Body* body = (b2Body*)rb2d.m_runtimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulseX, impulseY), true);
		return 0;						// Number of return values that lua is expecting
	}

	void ScriptGlue::RegisterFunctions(lua_State* L)
	{
		ADD_INTERNAL_CALL(L, HostFunction);
		ADD_INTERNAL_CALL(L, Log);
		ADD_INTERNAL_CALL(L, IsKeyDown);
		ADD_INTERNAL_CALL(L, ApplyLinearImpulse);
	}
}