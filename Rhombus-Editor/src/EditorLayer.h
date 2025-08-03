#pragma once

#include "Rhombus.h"

#include "EditorExtension.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowerPanel.h";
#include "Panels/TilesetPanel.h";
#include "Panels/EntityViewPanel.h";
#include "Panels/AnimationPanel.h";
#include "Rhombus/Renderer/EditorCamera.h"

#define RB_EDITOR 1

namespace rhombus
{
	class EditorLayer : public Layer
	{

	public:
		EditorLayer(std::function<Ref<Scene>()> sceneCB, std::function<Ref<EditorExtension>()> editorCB);
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(DeltaTime dt) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

		void RenderInWindow();
		SceneGraphNode* GetCommonParentOfEntities(std::vector<Entity> entities);

	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnKeyReleased(KeyReleasedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnGamepadAxis(GamepadAxisEvent& e);
		bool OnGamepadButtonDown(GamepadButtonDownEvent& e);
		bool OnGamepadButtonUp(GamepadButtonUpEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();
		void TogglePixelSnapping();

		void NewProject();
		bool OpenProject();
		void OpenProject(const std::filesystem::path& path);
		void SaveProject();

		void SerializeScene(Ref<Scene> scene, const std::filesystem::path& path);

		void OnScenePlay();
		void OnSceneStop();
		void OnEntitySelected(Entity entity);

		void DuplicateSelectedEntities();
		void DeleteSelectedEntities();

		void OnOverlayRender();

		void CalculateScreenResolutionPreset();
		void DisplayScreenResolutionMenu();

		// UI Panel
		void UI_Toolbar();

		// Temp (These needs to be abstracted away)
		Ref<Shader> m_FlatColourShader;
		Ref<VertexArray> m_SquareVA;
		Ref<Framebuffer> m_Framebuffer;

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;
		std::filesystem::path m_EditorScenePath;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondaryCameraEntity;
		Entity m_HoveredEntity;

		bool m_PrimaryCamera = true;
		bool m_PixelSnapping = true;

		EditorCamera m_EditorCamera;

		Ref<Texture2D> m_CheckerboardTexture;

		OrthographicCameraController m_CameraController;

		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		Vec2 m_ViewportSize = { 0.0f, 0.0f };
		Vec2 m_ViewportBounds[2];

		Color m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int m_gizmoType = 7;		// ImGuizmo::OPERATION::TRANSLATE

		bool m_ShowEditorSettings = false;
		bool m_ShowRenderStats = false;
		bool m_ShowPhysicsColliders = false;
		bool m_ShowGameScreenSizeRect = true;
		bool m_ShowTileMapGrid = false;
		int m_ScreenResolutionPreset = -1;
		Color m_PhysicsColliderColor = Color(0.0, 0.5, 1.0, 1.0);
		float m_PhysicsColliderAlpha = 0.5f;
		Color m_AreaColor = Color(0.0, 0.0, 1.0, 1.0);

		enum SceneState
		{
			Edit = 0,
			Play = 1
		};

#if RB_EDITOR
		SceneState m_SceneState = SceneState::Edit;
#else
		SceneState m_SceneState = SceneState::Play;
#endif

		// Panels
		SceneHierarchyPanel m_sceneHierarchyPanel;
		EntityViewPanel m_entityViewPanel;
		AnimationPanel m_animationPanel;
		Scope<ContentBrowserPanel> m_contentBrowserPanel;
		Scope<TilesetPanel> m_tilesetPanel;
		Ref<EditorExtension> m_editorExtension;

		// To be used when storing separate entity view panels for each entity
		//std::unordered_map<EntityID, Ref<EntityViewPanel>> m_entityViewPanels;

		// Callbacks
		std::function<Ref<Scene>()> m_sceneCreationCallback;
		std::function<Ref<EditorExtension>()> m_editorExtensionCreationCallback;

		// Editor resources
		Ref<Texture2D> m_IconPlay, m_IconStop;
	};
}
