#pragma once

#include "Rhombus/Math/Vector.h"
#include "Rhombus/Core/DeltaTime.h"
#include "EasingFunctions.h"
#include "Rhombus/Scenes/Entity.h"

#include <queue>

namespace rhombus 
{
	const int MAX_TWEEN_COMPONENTS = 4;

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

	class TweenStep
	{
	public:
		
		virtual float Step(DeltaTime dt) = 0;

	protected:
		TweenStep(float duration) : m_fDuration(duration), m_fCurrentTime(0.0f) {};

	protected:
		friend Tween;

		float m_fDuration;
		float m_fCurrentTime;
	};

	class TweenParameterStep : public TweenStep
	{
	public:
		TweenParameterStep(float* param, float begin, float finish, float duration, EasingType easingType)
			: m_iNumComponents(1), m_easingType(easingType), TweenStep(duration)
		{
			m_tweenParamsDOF[0] = { param, begin, finish };
		}

		TweenParameterStep(Vec2* param, Vec2 begin, Vec2 finish, float duration, EasingType easingType)
			: m_iNumComponents(2), m_easingType(easingType), TweenStep(duration)
		{
			m_tweenParamsDOF[0] = { &(param->x), begin.x, finish.x };
			m_tweenParamsDOF[1] = { &(param->y), begin.y, finish.y };
		}

		TweenParameterStep(Vec3* param, Vec3 begin, Vec3 finish, float duration, EasingType easingType)
			: m_iNumComponents(3), m_easingType(easingType), TweenStep(duration)
		{
			m_tweenParamsDOF[0] = { &(param->x), begin.x, finish.x };
			m_tweenParamsDOF[1] = { &(param->y), begin.y, finish.y };
			m_tweenParamsDOF[2] = { &(param->z), begin.z, finish.z };
		}

		TweenParameterStep(Vec4* param, Vec4 begin, Vec4 finish, float duration, EasingType easingType)
			: m_iNumComponents(4), m_easingType(easingType), TweenStep(duration)
		{
			m_tweenParamsDOF[0] = { &(param->x), begin.x, finish.x };
			m_tweenParamsDOF[1] = { &(param->y), begin.y, finish.y };
			m_tweenParamsDOF[2] = { &(param->z), begin.z, finish.z };
			m_tweenParamsDOF[3] = { &(param->w), begin.w, finish.w };
		}

		virtual float Step(DeltaTime dt) override;

		float ApplyEasing(EasingType easingType, float t, float b, float c, float d);

		inline TweenParamsDOF operator [] (const int idx) const
		{
			return m_tweenParamsDOF[idx];
		}

		inline TweenParamsDOF& operator [] (const int idx)
		{
			return m_tweenParamsDOF[idx];
		}
	private:
		TweenParamsDOF m_tweenParamsDOF[MAX_TWEEN_COMPONENTS];
		int m_iNumComponents = 1;
		EasingType m_easingType;
	};

	class TweenCallbackStep : public TweenStep
	{
	public:
		TweenCallbackStep(std::function<void(Entity)> callback, Entity entity)
			: m_callback(callback), m_callbackEntity(entity), TweenStep(0.0f)
		{
		}

		virtual float Step(DeltaTime dt) override;
	private:
		Entity m_callbackEntity;
		std::function<void(Entity)> m_callback;
	};

	class TweenWaitStep : public TweenStep
	{
	public:
		TweenWaitStep(float duration)
			: TweenStep(duration)
		{
		}

		virtual float Step(DeltaTime dt) override;
	};

	class Tween
	{
	public:
		Tween() = default;
		Tween(const TweenParameterStep& tweenStep);
		Tween(const TweenCallbackStep& tweenStep);
		Tween(const TweenWaitStep& tweenStep);
		Tween(float* param, float begin, float finish, float duration, EasingType easingType = EasingType::LINEAR);
		Tween(Vec2* param, Vec2 begin, Vec2 finish, float duration, EasingType easingType = EasingType::LINEAR);
		Tween(Vec3* param, Vec3 begin, Vec3 finish, float duration, EasingType easingType = EasingType::LINEAR);
		Tween(Vec4* param, Vec4 begin, Vec4 finish, float duration, EasingType easingType = EasingType::LINEAR);

		void Start();
		void Resume() { m_bIsRunning = true; }
		void Pause() { m_bIsRunning = false; }

		bool GetIsFinished() const { return m_bIsFinished; }

		void Step(DeltaTime dt);

		void AddTweenStep(const TweenParameterStep& tweenStep);
		void AddTweenStep(const TweenCallbackStep& tweenStep);
		void AddTweenStep(const TweenWaitStep& tweenStep);
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

		std::queue<Ref<TweenStep>> m_tweenSteps;
		bool m_bIsRunning;
		bool m_bIsFinished;
	};
}
