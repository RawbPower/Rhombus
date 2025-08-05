#include "rbpch.h"
#include "ScriptGlue.h"

#include "Rhombus/Core/Log.h"
#include "Rhombus/Core/Input.h"
#include "Rhombus/Core/KeyCodes.h"
#include "Rhombus/Scenes/Scene.h"
#include "Rhombus/Scripting/ScriptEngine.h"
#include "Rhombus/Renderer/Renderer2D.h"
#include "Rhombus/ECS/Components/Rigidbody2DComponent.h"
#include "Rhombus/ECS/Components/TransformComponent.h"
#include "Rhombus/ECS/Components/Area2DComponent.h"

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

	UUID GetEntityUUIDFromLua(lua_State * state)
	{
		lua_getfield(state, 1, "entityIDa");		// indexed as if this is a fresh stack
		uint32_t entityID32a = (uint32_t)lua_tonumber(state, -1);		// indexed as if this is a fresh stack

		lua_getfield(state, 1, "entityIDb");		// indexed as if this is a fresh stack
		uint32_t entityID32b = (uint32_t)lua_tonumber(state, -1);		// indexed as if this is a fresh stack


		uint64_t entityID64 = ((uint64_t)entityID32a) << 32 | entityID32b;

		return (UUID)entityID64;
	}

	int HostFunction(lua_State* state)
	{
		Log::Assert(lua_gettop(state) == 2, "Invalid number of arguments passed to function");

		float a = (float)lua_tonumber(state, 1);		// indexed as if this is a fresh stack
		float b = (float)lua_tonumber(state	, 2);		// indexed as if this is a fresh stack
		float c = a * b;
		lua_pushnumber(state, c);		// Push result onto lua stack
		return 1;						// Number of return values that lua is expecting
	}

	int Log(lua_State* state)
	{
		Log::Assert(lua_gettop(state) == 1, "Invalid number of arguments passed to function");
		std::string logString = lua_tostring(state, 1);
		Log::Debug(logString.c_str());
		return 0;
	}

	int GetName(lua_State* state)
	{
		UUID entityID = GetEntityUUIDFromLua(state);
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);

		lua_pushstring(state, entity.GetName().c_str());		// Push result onto lua stack
		return 1;						// Number of return values that lua is expecting
	}

	int IsKeyDown(lua_State* state)
	{
		Log::Assert(lua_gettop(state) == 1, "Invalid number of arguments passed to function");

		int key = (int)lua_tonumber(state, 1);		// indexed as if this is a fresh stack
		bool keyPressed = Input::IsKeyPressed(key);
		lua_pushboolean(state, keyPressed);		// Push result onto lua stack
		return 1;						// Number of return values that lua is expecting
	}

	int ApplyLinearImpulse(lua_State* state)
	{
		Log::Assert(lua_gettop(state) == 3, "Invalid number of arguments passed to function");

		UUID entityID = GetEntityUUIDFromLua(state);
		float impulseX = (float)lua_tonumber(state, 2);		// indexed as if this is a fresh stack
		float impulseY = (float)lua_tonumber(state, 3);		// indexed as if this is a fresh stack
		Scene* scene = ScriptEngine::GetSceneContext();
		Log::Assert(scene, "Invalid Scene in ApplyLinearImpulse");
		Entity entity = scene->GetEntityByUUID(entityID);
		std::string name = entity.GetComponent<TagComponent>().m_tag;
		//RB_CORE_ASSERT(entity, "Invalid Entity in ApplyLinearImpulse");

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		// move to function
		b2Body* body = (b2Body*)rb2d.m_runtimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulseX, impulseY), true);
		return 0;						// Number of return values that lua is expecting
	}

	int Translate(lua_State* state)
	{
		Log::Assert(lua_gettop(state) == 3, "Invalid number of arguments passed to function");

		UUID entityID = GetEntityUUIDFromLua(state);
		float translateX = (float)lua_tonumber(state, 2);		// indexed as if this is a fresh stack
		float translateY = (float)lua_tonumber(state, 3);		// indexed as if this is a fresh stack
		Scene* scene = ScriptEngine::GetSceneContext();
		Log::Assert(scene, "Invalid Scene in Translate");
		Entity entity = scene->GetEntityByUUID(entityID);
		std::string name = entity.GetComponent<TagComponent>().m_tag;

		auto& transformComponent = entity.GetComponent<TransformComponent>();
		transformComponent.SetPosition(transformComponent.GetPosition() + Vec3(translateX, translateY, 0.0f));

		return 0;						// Number of return values that lua is expecting
	}

	int SetPosition(lua_State* state)
	{
		Log::Assert(lua_gettop(state) == 4, "Invalid number of arguments passed to function");

		UUID entityID = GetEntityUUIDFromLua(state);
		float positionX = (float)lua_tonumber(state, 2);		// indexed as if this is a fresh stack
		float positionY = (float)lua_tonumber(state, 3);		// indexed as if this is a fresh stack
		float positionZ = (float)lua_tonumber(state, 4);		// indexed as if this is a fresh stack
		Scene* scene = ScriptEngine::GetSceneContext();
		Log::Assert(scene, "Invalid Scene in Translate");
		Entity entity = scene->GetEntityByUUID(entityID);
		std::string name = entity.GetComponent<TagComponent>().m_tag;

		auto& transformComponent = entity.GetComponent<TransformComponent>();
		transformComponent.SetPosition(Vec3(positionX, positionY, positionZ));

		return 0;						// Number of return values that lua is expecting
	}

	int GetPosition(lua_State* state)
	{
		Log::Assert(lua_gettop(state) == 1, "Invalid number of arguments passed to function");

		UUID entityID = GetEntityUUIDFromLua(state);
		Scene* scene = ScriptEngine::GetSceneContext();
		Log::Assert(scene, "Invalid Scene in Translate");
		Entity entity = scene->GetEntityByUUID(entityID);
		std::string name = entity.GetComponent<TagComponent>().m_tag;

		auto& transformComponent = entity.GetComponent<TransformComponent>();
		lua_pushnumber(state, transformComponent.GetPosition().x);
		lua_pushnumber(state, transformComponent.GetPosition().y);
		lua_pushnumber(state, transformComponent.GetPosition().z);

		return 3;
	}

	int GetMousePosition(lua_State* state)
	{
		Log::Assert(lua_gettop(state) == 0, "Invalid number of arguments passed to function");

		Vec3 mousePosition = Renderer2D::ConvertScreenToWorldSpace((int)Input::GetMouseX(), (int)Input::GetMouseY());
		lua_pushnumber(state, mousePosition.x);
		lua_pushnumber(state, mousePosition.y);

		return 2;
	}

	int IsMouseInArea(lua_State* state)
	{
		Log::Assert(lua_gettop(state) == 1, "Invalid number of arguments passed to function");

		UUID entityID = GetEntityUUIDFromLua(state);
		Scene* scene = ScriptEngine::GetSceneContext();
		Entity entity = scene->GetEntityByUUID(entityID);

		auto& ba2d = entity.GetComponent<BoxArea2DComponent>();
		
		lua_pushboolean(state, ba2d.m_isMouseInArea);

		return 1;
	}

	static const luaL_Reg rhombus_funcs[] =
	{
		{ "HostFunction", HostFunction},
		{ "Log", Log},
		{ "GetName", GetName},
		{ "IsKeyDown", IsKeyDown},
		{ "ApplyLinearImpulse", ApplyLinearImpulse},
		{ "Translate", Translate},
		{ "SetPosition", SetPosition},
		{ "GetPosition", GetPosition},
		{ "GetMousePosition", GetMousePosition},
		{ "IsMouseInArea", IsMouseInArea},
		{ NULL, NULL }
	};

	void ScriptGlue::RegisterFunctions(lua_State* L)
	{
		lua_createtable(L, 0, 1);
		luaL_setfuncs(L, rhombus_funcs, 0);
		lua_pushvalue(L, -1);
		lua_setglobal(L, "rhombus");
		//ADD_INTERNAL_CALL(L, HostFunction);
		//ADD_INTERNAL_CALL(L, Log);
		//ADD_INTERNAL_CALL(L, IsKeyDown);
		//ADD_INTERNAL_CALL(L, ApplyLinearImpulse);
	}
}