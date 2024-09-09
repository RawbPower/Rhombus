#pragma once

#include "Component.h"
#include "Rhombus/Animation/Tween.h"

namespace rhombus
{
	// NOTE: This is a hidden component and should not be added in editor
	class TweenComponent : public ComponentBase
	{
	public:
		TweenComponent() = default;
		TweenComponent(const TweenComponent& other) = default;

		Ref<Tween> CreateTween(Vec3* param, Vec3 begin, Vec3 finish, float duration) { m_tween = std::make_shared<Tween>(param, begin, finish, duration); return m_tween; }

		Ref<Tween> GetTween() { return m_tween; }

	private:
		Ref<Tween> m_tween;
	};
}
