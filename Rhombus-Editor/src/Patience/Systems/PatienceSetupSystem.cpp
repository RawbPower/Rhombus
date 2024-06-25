#include "PatienceSetupSystem.h"

#include "Patience/Components/PatienceComponent.h"
#include "Rhombus/Scripting/ScriptEngine.h"


void PatienceSetupSystem::Init()
{

	for (Entity entity : GetEntities())
	{
		PatienceComponent& patienceComponent = entity.GetComponent<PatienceComponent>();

		std::string scriptPath = Project::GetScriptDirectory().string() + "\\GameModeData_" + patienceComponent.m_setupScript + ".lua";
		
		if (ScriptEngine::DoScript(scriptPath))
		{
			std::list<std::string> cards = ScriptEngine::GetCardsTest(patienceComponent.m_setupScript);
		}
	}
}