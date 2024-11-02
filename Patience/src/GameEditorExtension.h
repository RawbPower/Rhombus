#pragma once

#include "Rhombus.h"
#include "RhombusEditor.h"

class GameEditorExtension : public rhombus::EditorExtension
{
public:
	//GameEditorExtension();
	//~GameEditorExtension() = default;

	virtual void DisplayAddComponentList(Entity entity) override;
	virtual void DisplayComponentProperties(Entity entity) override;

private:

};
