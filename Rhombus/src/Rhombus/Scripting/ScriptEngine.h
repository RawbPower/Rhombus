#pragma once

#include "Rhombus/ECS/Scene.h"
#include "Rhombus/ECS/Entity.h"

namespace rhombus {

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static void OnInitEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, DeltaTime dt);

		static Scene* GetSceneContext();
	private:
		static void InitLua();
		static void ShutdownLua();
	};

}
