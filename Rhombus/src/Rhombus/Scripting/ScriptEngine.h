#pragma once

#include "Rhombus/ECS/Scene.h"
#include "Rhombus/ECS/Entity.h"

#include <list>

namespace rhombus {

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static void SetupEntity(Entity entity);

		static bool DoScript(std::string sciptPath);

		static void InitNewTable(const char* tableName);
		static void GetGlobal(const char* globalName);
		static void GetField(const char* fieldName, int tableIndex = -1);
		static void GetListOfStringValueFromField(const char* fieldName, std::list<std::string>& list, int tableIndex = -1);

		static const char* GetString(int valueIndex = -1);
		static int GetInt(int valueIndex = -1);
		static float GetFloat(int valueIndex = -1);

		static void Pop(int popCount = 1);

		static void StartLoopThroughTable();
		static int NextKey();
		static const char* GetKey();

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
