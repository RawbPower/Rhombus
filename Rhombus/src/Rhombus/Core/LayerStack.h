#pragma once

#include "Rhombus/Core/Core.h"
#include "Rhombus/Core/Layer.h"

#include <vector>

namespace rhombus {

	class RB_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		// Layers get pushed into first half of list and overlays get pushed to second half of the list (we always want overlays at the end)
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		// Keeps track of layer count
		unsigned int m_LayerInsertIndex = 0;
	};
}
