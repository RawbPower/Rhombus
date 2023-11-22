#pragma once

#include "SceneCamera.h"
#include "ScriptableEntity.h"

#include <glm/glm.hpp>

namespace rhombus
{
	class TagComponent
	{
	public:
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : m_tag(tag) {}

		std::string m_tag;
	private:
		friend class Scene;
	};

	class TransformComponent
	{
	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4 & transform) : m_transform(transform) {}

		glm::mat4& GetTransform() { return m_transform; }

	private:
		glm::mat4 m_transform{ 1.0f };
	};

	class SpriteRendererComponent
	{
	public:
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : m_color(color) {}

		glm::vec4& GetColor() { return m_color; }

	private:
		glm::vec4 m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class CameraComponent
	{
	public:
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		SceneCamera& GetCamera() { return m_camera; }
		void SetIsPrimaryCamera(bool primary) { m_primary = primary; }
		bool GetIsPrimaryCamera() { return m_primary; }

		void SetHasFixedAspectRatio(bool fixedAspectRatio) { m_fixedAspectRatio = fixedAspectRatio; }
		bool GetHasFixedAspectRatio() { return m_fixedAspectRatio; }
	private:
		SceneCamera m_camera;
		bool m_primary = true;	// Maybe move this to the scene and out of the component
		bool m_fixedAspectRatio = false;
	};

	class NativeScriptComponent
	{
	public:
		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->m_instance; nsc->m_instance = nullptr; };
		}

	private:
		ScriptableEntity* m_instance = nullptr;
		friend class Scene;
	};
}
