#pragma once

#include "SceneCamera.h"
#include "Rhombus/Core/UUID.h"
#include "Rhombus/Renderer/Texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace rhombus
{
	class IDComponent
	{
	public:
		UUID m_id;

		IDComponent() = default;
		IDComponent(const IDComponent&) = default;
	};

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
		TransformComponent(const glm::vec3 & position) : m_position(position) {}

		glm::mat4 GetTransform() const 
		{ 
			/*glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), m_rotation.x, {1, 0, 0})
				* glm::rotate(glm::mat4(1.0f), m_rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), m_rotation.z, { 0, 0, 1 });*/

			glm::mat4 rotation = glm::toMat4(glm::quat(m_rotation));

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_position)
				* rotation
				* glm::scale(glm::mat4(1.0f), m_scale);

			return transform;
		}

		glm::vec3 m_position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_scale = { 1.0f, 1.0f, 1.0f };
	};

	class SpriteRendererComponent
	{
	public:
		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) : m_color(color) {}

		glm::vec4& GetColor() { return m_color; }
		glm::vec4 GetColor() const { return m_color; }
		void SetColor(glm::vec4 color) { m_color = color; }

		Ref<Texture2D> m_texture;

	private:
		glm::vec4 m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	struct CircleRendererComponent
	{
		glm::vec4 m_color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float m_thickness = 1.0f;
		float m_fade = 0.0f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

	class CameraComponent
	{
	public:
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;

		SceneCamera& GetCamera() { return m_camera; }
		void SetIsPrimaryCamera(bool primary) { m_primary = primary; }
		bool& GetIsPrimaryCamera() { return m_primary; }

		void SetHasFixedAspectRatio(bool fixedAspectRatio) { m_fixedAspectRatio = fixedAspectRatio; }
		bool& GetHasFixedAspectRatio() { return m_fixedAspectRatio; }
	private:
		SceneCamera m_camera;
		bool m_primary = true;	// Maybe move this to the scene and out of the component
		bool m_fixedAspectRatio = false;
	};

	// Forward declaration
	class ScriptableEntity;

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

	// Physics
	class Rigidbody2DComponent
	{
	public:
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType m_type = BodyType::Static;
		bool m_fixedRotation = false;

		// Storage for runtime
		void* m_runtimeBody = nullptr;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent& other) = default;
	};

	class BoxCollider2DComponent
	{
	public:
		glm::vec2 m_offset = { 0.0f, 0.0f };
		glm::vec2 m_size = { 0.5f, 0.5f };

		// TODO: Move to physics material
		float m_density = 1.0f;
		float m_friction = 0.5f;
		float m_restitution = 0.0f;
		float m_restitutionThreshold = 0.5f;

		// Storage for runtime
		void* m_runtimeFixture = nullptr;

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent& other) = default;
	};
}
