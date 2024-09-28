#include "rbpch.h"
#include "Tween.h"

namespace rhombus
{
	float TweenParameterStep::ApplyEasing(EasingType easingType, float t, float b, float c, float d)
	{
		switch (easingType)
		{
		case rhombus::EasingType::LINEAR:
			return easing::Linear(t, b, c, d);
		case rhombus::EasingType::SINE_IN:
			return easing::SineIn(t, b, c, d);
		case rhombus::EasingType::SINE_OUT:
			return easing::SineOut(t, b, c, d);
		case rhombus::EasingType::SINE_IN_OUT:
			return easing::SineInOut(t, b, c, d);
		case rhombus::EasingType::QUAD_IN:
			return easing::QuadIn(t, b, c, d);
		case rhombus::EasingType::QUAD_OUT:
			return easing::QuadOut(t, b, c, d);
		case rhombus::EasingType::QUAD_IN_OUT:
			return easing::QuadInOut(t, b, c, d);
		case rhombus::EasingType::CUBIC_IN:
			return easing::CubicIn(t, b, c, d);
		case rhombus::EasingType::CUBIC_OUT:
			return easing::CubicOut(t, b, c, d);
		case rhombus::EasingType::CUBIC_IN_OUT:
			return easing::CubicInOut(t, b, c, d);
		case rhombus::EasingType::QUART_IN:
			return easing::QuartIn(t, b, c, d);
		case rhombus::EasingType::QUART_OUT:
			return easing::QuartOut(t, b, c, d);
		case rhombus::EasingType::QUART_IN_OUT:
			return easing::QuartInOut(t, b, c, d);
		case rhombus::EasingType::QUINT_IN:
			return easing::QuintIn(t, b, c, d);
		case rhombus::EasingType::QUINT_OUT:
			return easing::QuintOut(t, b, c, d);
		case rhombus::EasingType::QUINT_IN_OUT:
			return easing::QuintInOut(t, b, c, d);
		case rhombus::EasingType::EXPO_IN:
			return easing::ExpoIn(t, b, c, d);
		case rhombus::EasingType::EXPO_OUT:
			return easing::ExpoOut(t, b, c, d);
		case rhombus::EasingType::EXPO_IN_OUT:
			return easing::ExpoInOut(t, b, c, d);
		case rhombus::EasingType::CIRC_IN:
			return easing::CircIn(t, b, c, d);
		case rhombus::EasingType::CIRC_OUT:
			return easing::CircOut(t, b, c, d);
		case rhombus::EasingType::CIRC_IN_OUT:
			return easing::CircInOut(t, b, c, d);
		case rhombus::EasingType::BACK_IN:
			return easing::BackIn(t, b, c, d);
		case rhombus::EasingType::BACK_OUT:
			return easing::BackOut(t, b, c, d);
		case rhombus::EasingType::BACK_IN_OUT:
			return easing::BackInOut(t, b, c, d);
		case rhombus::EasingType::ELASTIC_IN:
			return easing::ElasticIn(t, b, c, d);
		case rhombus::EasingType::ELASTIC_OUT:
			return easing::ElasticOut(t, b, c, d);
		case rhombus::EasingType::ELASTIC_IN_OUT:
			return easing::ElasticInOut(t, b, c, d);
		case rhombus::EasingType::BOUNCE_IN:
			return easing::BounceIn(t, b, c, d);
		case rhombus::EasingType::BOUNCE_OUT:
			return easing::BounceOut(t, b, c, d);
		case rhombus::EasingType::BOUNCE_IN_OUT:
			return easing::BounceInOut(t, b, c, d);
		default:
			return b + c;
		}
	}

	float TweenParameterStep::Step(DeltaTime dt)
	{
		m_fCurrentTime += dt;

		const float fTimeAboveDuration = m_fCurrentTime - m_fDuration;
		if (fTimeAboveDuration > 0.0f)
		{
			m_fCurrentTime = m_fDuration;
		}

		for (int i = 0; i < m_iNumComponents; i++)
		{
			if (m_tweenParamsDOF[i].m_fParameter)
			{
				*m_tweenParamsDOF[i].m_fParameter = ApplyEasing(m_easingType, m_fCurrentTime, m_tweenParamsDOF[i].m_fBegin, (m_tweenParamsDOF[i].m_fFinish - m_tweenParamsDOF[i].m_fBegin), m_fDuration);
			}
		}

		return fTimeAboveDuration > 0.0f ? fTimeAboveDuration : 0.0f;
	}

	float TweenCallbackStep::Step(DeltaTime dt)
	{
		m_callback(m_callbackEntity);
		return dt;
	}

	float TweenWaitStep::Step(DeltaTime dt)
	{
		m_fCurrentTime += dt;
		const float fTimeAboveDuration = m_fCurrentTime - m_fDuration;
		if (fTimeAboveDuration > 0.0f)
		{
			m_fCurrentTime = m_fDuration;
			return fTimeAboveDuration;
		}

		return 0.0f;
	}

	Tween::Tween(const TweenParameterStep& tweenStep)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		m_tweenSteps.push(CreateRef<TweenParameterStep>(tweenStep));
	}

	Tween::Tween(const TweenCallbackStep& tweenStep)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		m_tweenSteps.push(CreateRef<TweenCallbackStep>(tweenStep));
	}

	Tween::Tween(const TweenWaitStep& tweenStep)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		m_tweenSteps.push(CreateRef<TweenWaitStep>(tweenStep));
	}

	Tween::Tween(float* param, float begin, float finish, float duration, EasingType easingType)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		m_tweenSteps.push(CreateRef<TweenParameterStep>(param, begin, finish, duration, easingType));
	}

	Tween::Tween(Vec2* param, Vec2 begin, Vec2 finish, float duration, EasingType easingType)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		m_tweenSteps.push(CreateRef<TweenParameterStep>(param, begin, finish, duration, easingType));
	}

	Tween::Tween(Vec3* param, Vec3 begin, Vec3 finish, float duration, EasingType easingType)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		m_tweenSteps.push(CreateRef<TweenParameterStep>(param, begin, finish, duration, easingType));
	}

	Tween::Tween(Vec4* param, Vec4 begin, Vec4 finish, float duration, EasingType easingType)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		m_tweenSteps.push(CreateRef<TweenParameterStep>(param, begin, finish, duration, easingType));
	}

	void Tween::Start() 
	{ 
		m_bIsRunning = true;
		Ref<TweenStep> activeTweenStep = m_tweenSteps.front();
		activeTweenStep->m_fCurrentTime = 0.0f;
	}

	void Tween::Step(DeltaTime dt)
	{
		if (m_bIsRunning)
		{
			Ref<TweenStep> activeTweenStep = m_tweenSteps.front();

			float fTimeRemaining = dt;
			while (fTimeRemaining > 0.0f)
			{
				fTimeRemaining = activeTweenStep->Step(dt);
				if (fTimeRemaining > 0.0f)
				{
					m_tweenSteps.pop();
				}

				if (m_tweenSteps.size() == 0)
				{
					m_bIsRunning = false;
					m_bIsFinished = true;
					return;
				}

				activeTweenStep = m_tweenSteps.front();
			}
		}
	}

	void Tween::AddTweenStep(const TweenParameterStep& tweenStep)
	{
		m_tweenSteps.push(CreateRef<TweenParameterStep>(tweenStep));
	}

	void Tween::AddTweenStep(const TweenCallbackStep& tweenStep)
	{
		m_tweenSteps.push(CreateRef<TweenCallbackStep>(tweenStep));
	}

	void Tween::AddTweenStep(const TweenWaitStep& tweenStep)
	{
		m_tweenSteps.push(CreateRef<TweenWaitStep>(tweenStep));
	}
}