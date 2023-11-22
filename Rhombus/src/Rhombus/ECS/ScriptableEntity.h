#pragma once

#include "Entity.h"

namespace rhombus
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}

		template<typename T>
		const T& GetComponentRead()
		{
			return m_entity.GetComponentRead<T>();
		}

		template<typename T>
		T& GetComponent()
		{
			return m_entity.GetComponent<T>();
		}

	protected:
		virtual void OnReady() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(DeltaTime dt) {}

	private:
		Entity m_entity;
		friend class Scene;
	};
}