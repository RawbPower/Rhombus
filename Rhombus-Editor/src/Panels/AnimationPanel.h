#pragma once

#include "Rhombus/Renderer/Texture.h"

namespace rhombus
{
	class AnimationPanel
	{
	public:

		AnimationPanel();

		void OnImGuiRender();

	private:
		Ref<Texture2D> m_IconPlay, m_IconPause, m_IconForward, m_IconBackward, m_IconLoop, m_IconAdd, m_IconRename, m_IconDelete;
	};
}
