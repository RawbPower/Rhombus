#pragma once
#include "Rhombus/Core/Core.h"
#include "Rhombus/Scenes/Entity.h"

namespace rhombus
{
	class EditorExtension
	{
	public:
		//EditorExtension();
		//~EditorExtension() = default;

		virtual void DisplayAddComponentList(Entity entity) {};
		virtual void DisplayComponentProperties(Entity entity) {};

		//template<typename T>
		//static void DisplayAddComponentEntry(const std::string& entryName, Entity entity);

	private:

	};
}

