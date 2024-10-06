#include "rbpch.h"

#include "ScriptRegistry.h"

#include "Rhombus/Scripting/ScriptEngine.h"

#include "Rhombus/Core/Input.h"
#include "Rhombus/Core/Log.h"
#include "Rhombus/Scenes/Scene.h"
#include "Rhombus/Scenes/Entity.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace rhombus
{
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

	/*int ApplyLinearImpulse(lua_State* state)
	{
		RB_CORE_ASSERT(lua_gettop(state) == 2, "Invalid number of arguments passed to function");

		float impulseX = (float)lua_tonumber(state, 1);		// indexed as if this is a fresh stack
		float impulseY = (float)lua_tonumber(state, 2);		// indexed as if this is a fresh stack
		Scene* scene = ScriptEngine::GetSceneContext();
		RB_CORE_ASSERT(scene, "Invalid Scene in ApplyLinearImpulse");
		//Entity entity = scene->GetEntityByUUID(entityID);
		//RB_CORE_ASSERT(entity, "Invalid Entity in ApplyLinearImpulse");

		//auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		//rb2d.ApplyImpulse({ impulseX, impulseY });
		//b2Body* body = (b2Body*)rb2d.RuntimeBody;
		//body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), true);
		return 0;						// Number of return values that lua is expecting
	}*/

	// List of functions to wrap.
	static const luaL_Reg functions[] =
	{
		{ "Log", Log },
		{ "IsKeyDown", IsKeyDown },
		//{ "ApplyLinearImpulse", ApplyLinearImpulse },
		{ NULL, NULL }
	};

	extern "C" int luaopen_rhombus(lua_State * L)
	{
		luaL_newlib(L, functions);

		return 1;
	}
}