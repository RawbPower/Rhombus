#include "rbpch.h"
#include "Tween.h"
#include "EasingFunctions.h"

namespace rhombus
{
	Tween::Tween(float* param, float begin, float finish, float duration)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		TweenParams tweenParams(1, duration);
		tweenParams[0] = { param, begin, finish };
		m_tweenSteps.push(tweenParams);
	}

	Tween::Tween(Vec2* param, Vec2 begin, Vec2 finish, float duration)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		TweenParams tweenParams(2, duration);
		tweenParams[0] = { &(param->x), begin.x, finish.x };
		tweenParams[1] = { &(param->y), begin.y, finish.y };
		m_tweenSteps.push(tweenParams);
	}

	Tween::Tween(Vec3* param, Vec3 begin, Vec3 finish, float duration)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		TweenParams tweenParams(3, duration);
		tweenParams[0] = { &(param->x), begin.x, finish.x };
		tweenParams[1] = { &(param->y), begin.y, finish.y };
		tweenParams[2] = { &(param->z), begin.z, finish.z };
		m_tweenSteps.push(tweenParams);
	}

	Tween::Tween(Vec4* param, Vec4 begin, Vec4 finish, float duration)
		: m_bIsRunning(false), m_bIsFinished(false)
	{
		TweenParams tweenParams(4, duration);
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
				*activeTweenStep[i].m_fParameter = easing::linear(activeTweenStep.m_fCurrentTime, activeTweenStep[i].m_fBegin, (activeTweenStep[i].m_fFinish - activeTweenStep[i].m_fBegin), activeTweenStep.m_fDuration);
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

	void Tween::AddTweenStep(float* param, float begin, float finish, float duration)
	{
		TweenParams tweenParams(1, duration);
		tweenParams[0] = { param, begin, finish };
		m_tweenSteps.push(tweenParams);
	}

	void Tween::AddCallbackStep(std::function<void(Entity)> callback, Entity entity)
	{
		TweenParams tweenParams(callback, entity);
		m_tweenSteps.push(tweenParams);
	}
}