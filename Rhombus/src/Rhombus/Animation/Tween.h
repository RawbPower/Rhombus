#pragma once

#include "Rhombus/Math/Vector.h"
#include "Rhombus/Core/DeltaTime.h"

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

		void Start() { m_bIsRunning = true; m_fCurrentTime = 0.0f; }
		void Resume() { m_bIsRunning = true; }
		void Pause() { m_bIsRunning = false; }

		bool GetIsFinished() const { return m_bIsFinished; }

		void Step(DeltaTime dt);
	private:
		struct TweenParams
		{
			float* m_fParameter = nullptr;
			float m_fBegin = 0.0f;
			float m_fFinish = 0.0f;

			TweenParams() = default;
			TweenParams(float* x, float begin, float finish)
				: m_fParameter(x), m_fBegin(begin), m_fFinish(finish)
			{

			}
		};

		TweenParams m_tweenParams[MAX_TWEEN_COMPONENTS];

		int m_iNumComponents;

		float m_fDuration;
		float m_fCurrentTime;
		bool m_bIsRunning;
		bool m_bIsFinished;
	};
}
