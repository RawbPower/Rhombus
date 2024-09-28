#pragma once

#include "Rhombus/Math/Math.h"

namespace rhombus
{
	enum class EasingType {
		LINEAR = 0,
		SINE_IN, SINE_OUT, SINE_IN_OUT,
		QUAD_IN, QUAD_OUT, QUAD_IN_OUT,
		CUBIC_IN, CUBIC_OUT, CUBIC_IN_OUT,
		QUART_IN, QUART_OUT, QUART_IN_OUT,
		QUINT_IN, QUINT_OUT, QUINT_IN_OUT,
		EXPO_IN, EXPO_OUT, EXPO_IN_OUT,
		CIRC_IN, CIRC_OUT, CIRC_IN_OUT,
		BACK_IN, BACK_OUT, BACK_IN_OUT,
		ELASTIC_IN, ELASTIC_OUT, ELASTIC_IN_OUT,
		BOUNCE_IN, BOUNCE_OUT, BOUNCE_IN_OUT
	};
}

namespace rhombus::easing
{
	static float Linear(float t, float b, float c, float d) 
	{
		return c * t / d + b;
	}

	static float SineIn(float t, float b, float c, float d) 
	{
		return c * (1.0f - math::Cos(t / d * (math::PI / 2.0f))) + b;
	}

	static float SineOut(float t, float b, float c, float d) 
	{
		return c * math::Sin(t / d * (math::PI / 2.0f)) + b;
	}

	static float SineInOut(float t, float b, float c, float d) 
	{
		return c / 2.0f * (1.0f - math::Cos(math::PI * t / d)) + b;
	}

	static float QuadIn(float t, float b, float c, float d)
	{
		t /= d;
		return c * t * t + b;
	}

	static float QuadOut(float t, float b, float c, float d)
	{
		t /= d;
		return -c * t * (t - 2) + b;
	}

	static float QuadInOut(float t, float b, float c, float d)
	{
		t = t / d * 2;
		if (t < 1)
			return c / 2 * t * t + b;
		return -c / 2 * ((--t) * (t - 2) - 1) + b;
	}

	static float CubicIn(float t, float b, float c, float d)
	{
		return c * math::Pow(t / d, 3) + b;
	}

	static float CubicOut(float t, float b, float c, float d)
	{
		return c * (math::Pow(t / d - 1, 3) + 1.0f) + b;
	}

	static float CubicInOut(float t, float b, float c, float d)
	{
		t /= d / 2.0f;
		if (t < 1)
			return c / 2.0f * math::Pow(t, 3) + b;
		return c / 2.0f * (math::Pow(t - 2.0f, 3) + 2.0f) + b;
	}

	static float QuartIn(float t, float b, float c, float d)
	{
		return c * math::Pow(t / d, 4) + b;
	}

	static float QuartOut(float t, float b, float c, float d)
	{
		return -c * (math::Pow(t / d - 1, 4) - 1.0f) + b;
	}

	static float QuartInOut(float t, float b, float c, float d)
	{
		t /= d / 2.0f;
		if (t < 1)
			return c / 2.0f * math::Pow(t, 4) + b;
		return -c / 2.0f * (math::Pow(t - 2.0f, 4) - 2.0f) + b;
	}

	static float QuintIn(float t, float b, float c, float d)
	{
		return c * math::Pow(t / d, 5) + b;
	}

	static float QuintOut(float t, float b, float c, float d)
	{
		return c * (math::Pow(t / d - 1, 5) + 1.0f) + b;
	}

	static float QuintInOut(float t, float b, float c, float d)
	{
		t /= d / 2.0f;
		if (t < 1)
			return c / 2.0f * math::Pow(t, 5) + b;
		return c / 2.0f * (math::Pow(t - 2.0f, 5) + 2.0f) + b;
	}

	static float ExpoIn(float t, float b, float c, float d)
	{
		return c * math::Pow(2.0f, 10.0f * (t / d - 1.0f)) + b;
	}

	static float ExpoOut(float t, float b, float c, float d)
	{
		return c * (-math::Pow(2.0f, -10.0f * t / d) + 1.0f) + b;
	}

	static float ExpoInOut(float t, float b, float c, float d)
	{
		t /= d / 2.0f;
		if (t < 1)
			return c / 2.0f * math::Pow(2.0f, 10.0f * (t - 1.0f)) + b;
		return c / 2.0f * (-math::Pow(2.0f, -10.0f * (t - 1.0f)) + 2.0f) + b;
	}

	static float CircIn(float t, float b, float c, float d)
	{
		t /= d;
		return c * (1.0f - math::Sqrt(1.0f - t * t)) + b;
	}

	static float CircOut(float t, float b, float c, float d)
	{
		t = t / d - 1;
		return c * math::Sqrt(1.0f - t * t) + b;
	}

	static float CircInOut(float t, float b, float c, float d)
	{
		t /= d / 2.0f;
		if (t < 1)
			return c / 2.0f * (1.0f - math::Sqrt(1.0f - t * t)) + b;
		t -= 2;
		return c / 2.0f * (math::Sqrt(1.0f - t * t) + 1.0f) + b;
	}

	static float BackIn(float t, float b, float c, float d)
	{
		float s = 1.70158f;
		t /= d;

		return c * t * t * ((s + 1) * t - s) + b;
	}

	static float BackOut(float t, float b, float c, float d)
	{
		float s = 1.70158f;
		t = t / d - 1;

		return c * (t * t * ((s + 1) * t + s) + 1) + b;
	}

	static float BackInOut(float t, float b, float c, float d)
	{
		float s = 1.70158f * 1.525f;
		t /= d / 2;

		if (t < 1) {
			return c / 2 * (t * t * ((s + 1) * t - s)) + b;
		}

		t -= 2;
		return c / 2 * (t * t * ((s + 1) * t + s) + 2) + b;
	}

	static float ElasticIn(float t, float b, float c, float d)
	{
		if (t == 0.0f) 
		{
			return b;
		}

		t /= d;
		if (t == 1.0f) 
		{
			return b + c;
		}

		t -= 1;
		float p = d * 0.3f;
		float a = c * math::Pow(2, 10 * t);
		float s = p / 4;

		return -(a * math::Sin((t * d - s) * (2 * math::PI) / p)) + b;
	}

	static float ElasticOut(float t, float b, float c, float d)
	{
		if (t == 0.0f)
		{
			return b;
		}

		t /= d;
		if (t == 1.0f)
		{
			return b + c;
		}

		float p = d * 0.3f;
		float s = p / 4;

		return (c * math::Pow(2, -10 * t) * math::Sin((t * d - s) * (2 * math::PI) / p) + c + b);
	}

	static float ElasticInOut(float t, float b, float c, float d)
	{
		if (t == 0.0f)
		{
			return b;
		}

		t /= d / 2;
		if (t == 2.0f)
		{
			return b + c;
		}

		float p = d * (0.3f * 1.5f);
		float a = c;
		float s = p / 4;

		if (t < 1) 
		{
			t -= 1;
			a *= math::Pow(2, 10 * t);
			return -0.5f * (a * sin((t * d - s) * (2 * math::PI) / p)) + b;
		}

		t -= 1;
		a *= math::Pow(2, -10 * t);
		return a * sin((t * d - s) * (2 * math::PI) / p) * 0.5f + c + b;
	}

	static float BounceOut(float t, float b, float c, float d)
	{
		t /= d;

		if (t < (1 / 2.75f)) 
		{
			return c * (7.5625f * t * t) + b;
		}

		if (t < (2 / 2.75f)) 
		{
			t -= 1.5f / 2.75f;
			return c * (7.5625f * t * t + 0.75f) + b;
		}

		if (t < (2.5 / 2.75)) 
		{
			t -= 2.25f / 2.75f;
			return c * (7.5625f * t * t + 0.9375f) + b;
		}

		t -= 2.625f / 2.75f;
		return c * (7.5625f * t * t + 0.984375f) + b;
	}

	static float BounceIn(float t, float b, float c, float d)
	{
		return c - BounceOut(d - t, 0, c, d) + b;
	}

	static float BounceInOut(float t, float b, float c, float d)
	{
		if (t < d / 2) 
		{
			return BounceIn(t * 2, b, c / 2, d);
		}
		float h = c / 2;
		return BounceOut(t * 2 - d, b + h, h, d);

	}
}

