#pragma once

#include "rbpch.h"
#include "Rhombus/Core/Core.h"

namespace rhombus {

	// Events in the game engine are currently blocking, meaning when an event occurs it 
	// immediately get dispatched and must be dealt with right there and then. For the 
	// future, a better strategy might be to buffer events in a event bus and process them 
	// during the "event" part of the update stage.

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	// This is used if we want to filter events
	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};


#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	// Event class (base class for events)
	class RB_API Event
	{
	public:
		// Says if event is handled so it doesn't propogate further in the layers
		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		// Checks if an event is in the given category
		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}
	};

	// Way to dispatch events based on type easily. When event is called
	// we receive it as an event reference and we dont know what type it
	// is. If the called function and the event match is will call that
	// function with that event. Otherwise it wont do anything.
	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{
		}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			// Check is the event type of the incoming event is equal to the static type of the template used between <>
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(static_cast<T&>(m_Event));
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}