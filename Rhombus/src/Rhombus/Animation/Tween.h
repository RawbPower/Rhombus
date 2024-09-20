#pragma once

#include "Rhombus/Math/Vector.h"
#include "Rhombus/Core/DeltaTime.h"
#include "Rhombus/ECS/Entity.h"

#include <queue>

namespace rhombus 
{
	const int MAX_TWEEN_COMPONENTS = 4;

	class Tween
	{
	public:
		Tween() = default;
		Tween(float* param, float begin, float finish, float duration);
		Tween(Vec2* param, Vec2 begin, Vec2 finish, float duration);
		Tween(Vec3* param, Vec3 begin, Vec3 finish, float duration);
		Tween(Vec4* param, Vec4 begin, Vec4 finish, float duration);
		Tween(std::function<void(Entity)> callback, Entity entity);

		void Start();
		void Resume() { m_bIsRunning = true; }
		void Pause() { m_bIsRunning = false; }

		bool GetIsFinished() const { return m_bIsFinished; }

		void Step(DeltaTime dt);

		void AddTweenStep(float* param, float begin, float finish, float duration);
		void AddCallbackStep(std::function<void(Entity)> callback, Entity entity);
	private:
		struct TweenParamsDOF
		{
			float* m_fParameter = nullptr;
			float m_fBegin = 0.0f;
			float m_fFinish = 0.0f;

			TweenParamsDOF() = default;
			TweenParamsDOF(float* x, float begin, float finish)
				: m_fParameter(x), m_fBegin(begin), m_fFinish(finish)
			{

			}
		};

		struct TweenParams
		{
			TweenParamsDOF m_tweenParamsDOF[MAX_TWEEN_COMPONENTS];
			int m_iNumComponents = 1;
			float m_fDuration = 0.0f;
			float m_fCurrentTime = 0.0f;

			Entity m_callbackEntity;
			std::function<void(Entity)> m_callback;

			TweenParams() = default;
			TweenParams(int numComponents, float duration)
				: m_tweenParamsDOF(), m_iNumComponents(numComponents), m_fDuration(duration), m_fCurrentTime(0.0f)
			{

			}

			TweenParams(std::function<void(Entity)> callback, Entity entity)
				: m_tweenParamsDOF(), m_callback(callback), m_callbackEntity(entity), m_iNumComponents(0), m_fDuration(0.0f), m_fCurrentTime(0.0f)
			{

			}

			inline TweenParamsDOF operator [] (const int idx) const
			{
				return m_tweenParamsDOF[idx];
			}

			inline TweenParamsDOF& operator [] (const int idx)
			{
				return m_tweenParamsDOF[idx];
			}
		};

		std::queue<TweenParams> m_tweenSteps;
		bool m_bIsRunning;
		bool m_bIsFinished;
	};
}
