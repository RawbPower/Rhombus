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

		Ref<Tween> CreateTween(float* param, float begin, float finish, float duration) { m_tweens.push_back(std::make_shared<Tween>(param, begin, finish, duration)); return m_tweens.back(); }
		Ref<Tween> CreateTween(Vec2* param, Vec2 begin, Vec2 finish, float duration) { m_tweens.push_back(std::make_shared<Tween>(param, begin, finish, duration)); return m_tweens.back(); }
		Ref<Tween> CreateTween(Vec3* param, Vec3 begin, Vec3 finish, float duration) { m_tweens.push_back(std::make_shared<Tween>(param, begin, finish, duration)); return m_tweens.back(); }
		Ref<Tween> CreateTween(Vec4* param, Vec4 begin, Vec4 finish, float duration) { m_tweens.push_back(std::make_shared<Tween>(param, begin, finish, duration)); return m_tweens.back(); }

		Ref<Tween> GetTween(int i) { return m_tweens[i]; }
		int GetTweenCount() const { return m_tweens.size(); }

	private:
		std::vector<Ref<Tween>> m_tweens;
	};
}
