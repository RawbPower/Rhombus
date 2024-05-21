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

		static void SetupEntity(Entity entity);

		static void OnInitEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, DeltaTime dt);

		static void OnMouseEnterArea(Entity entity);
		static void OnMouseExitArea(Entity entity);

		static void OnMouseButtonPressed(Entity entity, int button);
		static void OnMouseButtonReleased(Entity entity, int button);

		static Scene* GetSceneContext();
	private:
		static void InitLua();
		static void ShutdownLua();
	};

}
