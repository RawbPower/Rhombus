#pragma once

//#include "Rhombus.h"
#include "EditorExtension.h"

class GameEditorExtension : public rhombus::EditorExtension
{
public:
	//GameEditorExtension();
	//~GameEditorExtension() = default;

	virtual void DisplayAddComponentList(rhombus::Entity entity) override;
	virtual void DisplayComponentProperties(rhombus::Entity entity) override;

private:

};
