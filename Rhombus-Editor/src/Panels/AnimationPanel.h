#pragma once

#include "Rhombus/Scenes/Entity.h"
#include "Rhombus/Renderer/Texture.h"

namespace rhombus
{
	class AnimationPanel
	{
	public:

		AnimationPanel();

		void OnImGuiRender();

		void SetCurrentEntity(Entity entity) { m_currentEntity = entity; m_currentSampleIndex = -1; }
		Entity GetCurrentEntity() const { return m_currentEntity; }
		bool HasValidCurrentEntity() const { return (EntityID)m_currentEntity != INVALID_ENTITY; }
		bool IsPlaying() const { return m_isPlaying; }

	private:
		Ref<Texture2D> m_IconPlay, m_IconPause, m_IconForward, m_IconBackward, m_IconLoop, m_IconAdd, m_IconRename, m_IconDelete;
		Entity m_currentEntity;
		int m_currentSampleIndex = -1;
		bool m_isPlaying = false;
	};
}
