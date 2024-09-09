#include "rbpch.h"
#include "Tween.h"

namespace rhombus
{
	Tween::Tween(float* param, float begin, float finish, float duration)
		: m_fDuration(duration), m_fCurrentTime(0.0f), m_iNumComponents(1), m_bIsRunning(false), m_bIsFinished(false)
	{
		m_tweenParams[0] = { param, begin, finish };
	}

	Tween::Tween(Vec2* param, Vec2 begin, Vec2 finish, float duration)
		: m_fDuration(duration), m_fCurrentTime(0.0f), m_iNumComponents(2), m_bIsRunning(false), m_bIsFinished(false)
	{
		m_tweenParams[0] = { &(param->x), begin.x, finish.x };
		m_tweenParams[1] = { &(param->y), begin.y, finish.y };
	}

	Tween::Tween(Vec3* param, Vec3 begin, Vec3 finish, float duration)
		: m_fDuration(duration), m_fCurrentTime(0.0f), m_iNumComponents(3), m_bIsRunning(false), m_bIsFinished(false)
	{
		m_tweenParams[0] = { &(param->x), begin.x, finish.x };
		m_tweenParams[1] = { &(param->y), begin.y, finish.y };
		m_tweenParams[2] = { &(param->z), begin.z, finish.z };
	}

	Tween::Tween(Vec4* param, Vec4 begin, Vec4 finish, float duration)
		: m_fDuration(duration), m_fCurrentTime(0.0f), m_iNumComponents(4), m_bIsRunning(false), m_bIsFinished(false)
	{
		m_tweenParams[0] = { &(param->x), begin.x, finish.x };
		m_tweenParams[1] = { &(param->y), begin.y, finish.y };
		m_tweenParams[2] = { &(param->z), begin.z, finish.z };
		m_tweenParams[3] = { &(param->w), begin.w, finish.w };
	}

	void Tween::Step(DeltaTime dt)
	{
		if (m_bIsRunning)
		{
			m_fCurrentTime += dt;

			if (m_fCurrentTime > m_fDuration)
			{
				m_fCurrentTime = m_fDuration;
				m_bIsRunning = false;
				m_bIsFinished = true;
			}

			for (int i = 0; i < m_iNumComponents; i++)
			{
				*m_tweenParams[i].m_fParameter = m_tweenParams[i].m_fBegin + (m_fCurrentTime / m_fDuration) * (m_tweenParams[i].m_fFinish - m_tweenParams[i].m_fBegin);
			}
		}
	}
}