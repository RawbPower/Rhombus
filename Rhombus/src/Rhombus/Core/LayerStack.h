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
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		std::vector<Layer*> m_Layers;
		// Keeps track of layer count
		unsigned int m_LayerInsertIndex = 0;
	};
}
