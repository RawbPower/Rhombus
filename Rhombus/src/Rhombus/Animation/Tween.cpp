#include "rbpch.h"
#include "Tween.h"

namespace rhombus
{
	Tween::Tween(float* param, float begin, float finish, float duration, EasingType easingType)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		TweenParams tweenParams(1, duration, easingType);
		tweenParams[0] = { param, begin, finish };
		m_tweenSteps.push(tweenParams);
	}

	Tween::Tween(Vec2* param, Vec2 begin, Vec2 finish, float duration, EasingType easingType)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		TweenParams tweenParams(2, duration, easingType);
		tweenParams[0] = { &(param->x), begin.x, finish.x };
		tweenParams[1] = { &(param->y), begin.y, finish.y };
		m_tweenSteps.push(tweenParams);
	}

	Tween::Tween(Vec3* param, Vec3 begin, Vec3 finish, float duration, EasingType easingType)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		TweenParams tweenParams(3, duration, easingType);
		tweenParams[0] = { &(param->x), begin.x, finish.x };
		tweenParams[1] = { &(param->y), begin.y, finish.y };
		tweenParams[2] = { &(param->z), begin.z, finish.z };
		m_tweenSteps.push(tweenParams);
	}

	Tween::Tween(Vec4* param, Vec4 begin, Vec4 finish, float duration, EasingType easingType)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		TweenParams tweenParams(4, duration, easingType);
		tweenParams[0] = { &(param->x), begin.x, finish.x };
		tweenParams[1] = { &(param->y), begin.y, finish.y };
		tweenParams[2] = { &(param->z), begin.z, finish.z };
		tweenParams[3] = { &(param->w), begin.w, finish.w };
		m_tweenSteps.push(tweenParams);
	}

	Tween::Tween(std::function<void(Entity)> callback, Entity entity)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		TweenParams tweenParams(callback, entity);
		m_tweenSteps.push(tweenParams);
	}

	void Tween::Start() 
	{ 
		m_bIsRunning = true;
		TweenParams& activeTweenStep = m_tweenSteps.front();
		activeTweenStep.m_fCurrentTime = 0.0f;
	}

	void Tween::Step(DeltaTime dt)
	{
		if (m_bIsRunning)
		{
			TweenParams& activeTweenStep = m_tweenSteps.front();

			if (activeTweenStep.m_iNumComponents <= 0)
			{
				activeTweenStep.m_callback(activeTweenStep.m_callbackEntity);
				m_tweenSteps.pop();
				if (m_tweenSteps.size() == 0)
				{
					m_bIsRunning = false;
					m_bIsFinished = true;
					return;
				}

				activeTweenStep = m_tweenSteps.front();
			}


			activeTweenStep.m_fCurrentTime += dt;

			bool moveToNextStep = false;
			if (activeTweenStep.m_fCurrentTime > activeTweenStep.m_fDuration)
			{
				activeTweenStep.m_fCurrentTime = activeTweenStep.m_fDuration;
				moveToNextStep = true;
			}

			for (int i = 0; i < activeTweenStep.m_iNumComponents; i++)
			{
				if (activeTweenStep[i].m_fParameter)
				{
					*activeTweenStep[i].m_fParameter = ApplyEasing(activeTweenStep.m_easingType, activeTweenStep.m_fCurrentTime, activeTweenStep[i].m_fBegin, (activeTweenStep[i].m_fFinish - activeTweenStep[i].m_fBegin), activeTweenStep.m_fDuration);
				}
			}

			if (moveToNextStep)
			{
				m_tweenSteps.pop();
				if (m_tweenSteps.size() == 0)
				{
					m_bIsRunning = false;
					m_bIsFinished = true;
				}
			}
		}
	}

	float Tween::ApplyEasing(EasingType easingType, float t, float b, float c, float d)
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

	void Tween::AddTweenStep(float* param, float begin, float finish, float duration, EasingType easingType)
	{
		TweenParams tweenParams(1, duration, easingType);
		tweenParams[0] = { param, begin, finish };
		m_tweenSteps.push(tweenParams);
	}

	void Tween::AddTweenStep(Vec2* param, Vec2 begin, Vec2 finish, float duration, EasingType easingType)
	{
		TweenParams tweenParams(2, duration, easingType);
		tweenParams[0] = { &(param->x), begin.x, finish.x };
		tweenParams[1] = { &(param->y), begin.y, finish.y };
		m_tweenSteps.push(tweenParams);
	}

	void Tween::AddTweenStep(Vec3* param, Vec3 begin, Vec3 finish, float duration, EasingType easingType)
	{
		TweenParams tweenParams(3, duration, easingType);
		tweenParams[0] = { &(param->x), begin.x, finish.x };
		tweenParams[1] = { &(param->y), begin.y, finish.y };
		tweenParams[2] = { &(param->z), begin.z, finish.z };
		m_tweenSteps.push(tweenParams);
	}

	void Tween::AddTweenStep(Vec4* param, Vec4 begin, Vec4 finish, float duration, EasingType easingType)
	{
		TweenParams tweenParams(4, duration, easingType);
		tweenParams[0] = { &(param->x), begin.x, finish.x };
		tweenParams[1] = { &(param->y), begin.y, finish.y };
		tweenParams[2] = { &(param->z), begin.z, finish.z };
		tweenParams[3] = { &(param->w), begin.w, finish.w };
		m_tweenSteps.push(tweenParams);
	}

	void Tween::AddCallbackStep(std::function<void(Entity)> callback, Entity entity)
	{
		TweenParams tweenParams(callback, entity);
		m_tweenSteps.push(tweenParams);
	}
}