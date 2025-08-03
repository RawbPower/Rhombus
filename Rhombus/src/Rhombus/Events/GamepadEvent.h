#pragma once

#include "Event.h"

namespace rhombus {

	class RB_API GamepadAxisEvent : public Event
	{
	public:
		GamepadAxisEvent(int axis, float value)
			: m_Axis(axis), m_Value(value) {}

		inline int GetAxis() const { return m_Axis; }
		inline float GetValue() const { return m_Value; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "GamepadAxisEvent: Axis " << GetAxis() << ", " << GetValue();
			return ss.str();
		}

		EVENT_CLASS_TYPE(GamepadAxis)
		EVENT_CLASS_CATEGORY(EventCategoryGamepad | EventCategoryInput)
	private:
		int m_Axis;
		float m_Value;
	};

	class RB_API GamepadButtonDownEvent : public Event
	{
	public:
		GamepadButtonDownEvent(int button)
			: m_Button(button) {}

		inline int GetGamepadButton() const { return m_Button; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "GamepadButtonDownEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(GamepadButtonDown);
		EVENT_CLASS_CATEGORY(EventCategoryGamepad | EventCategoryInput | EventCategoryGamepadButton)
	private:
		int m_Button;
	};

	class RB_API GamepadButtonUpEvent : public Event
	{
	public:
		GamepadButtonUpEvent(int button)
			: m_Button(button) {}

		inline int GetGamepadButton() const { return m_Button; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "GamepadButtonUpEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(GamepadButtonUp);
		EVENT_CLASS_CATEGORY(EventCategoryGamepad | EventCategoryInput | EventCategoryGamepadButton)
	private:
		int m_Button;
	};
}
