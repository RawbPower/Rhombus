#include "EditorLayer.h"

#include "imgui/imgui.h"

#include "Rhombus/ECS/SceneSerializer.h"
#include "Rhombus/Utils/PlatformUtils.h"
#include "Rhombus/Math/Math.h"
#include "ScreenResolutionPreset.h"

#include "PatienceScene.h"

#include "ImGuizmo.h"

namespace rhombus
{
	EditorLayer::EditorLayer()
		: Layer("Editor"), m_CameraController(1920.0f / 1080.0f, true), m_SquareColor({0.2f, 0.3f, 0.8f, 1.0f})
	{

	}

	void EditorLayer::OnAttach()
	{
		RB_PROFILE_FUNCTION();

		m_CheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		m_IconPlay = Texture2D::Create("Resources/Icons/Play.png");
		m_IconStop = Texture2D::Create("Resources/Icons/Stop.png");

		m_ActiveScene = CreateRef<Scene>();

		auto commandLineArgs = Application::Get().GetSpecification().commandLineArgs;
		if (commandLineArgs.count > 1)
		{
			auto projectFilePath = commandLineArgs[1];
			OpenProject(projectFilePath);
		}
		else
		{
			// Prompt user to select directory
			//NewProject();
			// Just close editor if there is no project
			if (!OpenProject())
			{
				Application::Get().Close();
			}
		}

		// Framebuffer
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = Project::GetGameWidth();
		fbSpec.Height = Project::GetGameHeight();
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 10000.0f);	// 1.778 = 16/9
		m_EditorCamera.SetDistance(Project::GetGameWidth() * 1.2f);

		m_ViewportSize = { (float)fbSpec.Width, (float)fbSpec.Height };
		m_ViewportBounds[0] = { 0.0f, 0.0f };
		m_ViewportBounds[1] = m_ViewportSize;
		m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

#if 0
		m_SquareEntity = m_ActiveScene->CreateEntity("Green Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(Color{ 0.0f, 1.0f, 0.0f, 1.0f });

		auto& redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(Color{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondaryCameraEntity = m_ActiveScene->CreateEntity("Secondary Camera");
		auto& cameraComponent = m_SecondaryCameraEntity.AddComponent<CameraComponent>();
		cameraComponent.SetIsPrimaryCamera(false);

		class CameraController : public ScriptableEntity
		{
		public:
			virtual void OnReady() override
			{
			}

			virtual void OnDestroy() override
			{

			}

			virtual void OnUpdate(DeltaTime dt) override
			{
				auto& position = GetComponent<TransformComponent>().m_position;
				float speed = 5.0f;

				if (Input::IsKeyPressed(RB_KEY_A))
					position.x -= speed * dt;
				if (Input::IsKeyPressed(RB_KEY_D))
					position.x += speed * dt;
				if (Input::IsKeyPressed(RB_KEY_W))
					position.y += speed * dt;
				if (Input::IsKeyPressed(RB_KEY_S))
					position.y -= speed * dt;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

		m_sceneHierarchyPanel.SetContext(m_ActiveScene);

		CalculateScreenResolutionPreset();
	}

	void EditorLayer::OnDetach()
	{
		RB_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(DeltaTime dt)
	{
		RB_PROFILE_FUNCTION();

		// Resize
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&		// zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		// Render
		Renderer2D::ResetStats();
		Renderer2D::SetFPDStat(dt);
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		// Clear our entity ID attachment to -1
#if RB_EDITOR
		m_Framebuffer->ClearAttachment(1, -1);
#endif

		// Update Scene
		switch (m_SceneState)
		{
		case SceneState::Edit:
		{
			// Update
			if (m_ViewportFocused)
			{
				m_CameraController.OnUpdate(dt);
			}

			m_EditorCamera.OnUpdate(dt);

			m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);
			break;
		}
		case SceneState::Play:
		{
			m_ActiveScene->OnUpdateRuntime(dt);
			break;
		}
		}

#if RB_EDITOR
		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;			// Make top left (0,0)
		Vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;	// Flip the y axis
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((EntityID)pixelData, m_ActiveScene.get());
		}
		else
		{
			m_HoveredEntity = Entity();
		}
#endif

		OnOverlayRender();

		m_Framebuffer->Unbind();

#if !RB_EDITOR
		RenderInWindow();
#endif
	}

	void EditorLayer::RenderInWindow()
	{
		RenderCommand::SetClearColor({ 0.6f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();
		RenderCommand::SetViewport(0, 0, Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());
		Renderer2D::DrawFrambuffer(m_Framebuffer);
	}

	void EditorLayer::OnImGuiRender()
	{
#if !RB_EDITOR
		return;
#endif

		RB_PROFILE_FUNCTION();

		//ImGui::ShowDemoWindow();
		//ImGui::ShowStyleEditor();

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background 
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		//ImGuiStyle& style = ImGui::GetStyle();
		//float minWinSizeX = style.WindowMinSize.x;
		//style.WindowMinSize.x = 340.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		//style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
				{
					OpenProject();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				{
					SaveScene();
				}

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}


				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Project"))
			{
				DisplayScreenResolutionMenu();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Editor"))
			{
				if (ImGui::MenuItem("Pixel Snapping", NULL, m_PixelSnapping)) 
				{
					TogglePixelSnapping();
				}

				if (ImGui::MenuItem("Show Colliders", NULL, m_ShowPhysicsColliders))
				{
					m_ShowPhysicsColliders = !m_ShowPhysicsColliders;
				}

				if (ImGui::MenuItem("Show Game Screen Size Rect", NULL, m_ShowGameScreenSizeRect))
				{
					m_ShowGameScreenSizeRect = !m_ShowGameScreenSizeRect;
				}

				if (ImGui::MenuItem("Show Editor Settings", NULL, m_ShowEditorSettings))
				{
					m_ShowEditorSettings = !m_ShowEditorSettings;
				}

				if (ImGui::MenuItem("Show Rendering Statistics", NULL, m_ShowRenderStats))
				{
					m_ShowRenderStats = !m_ShowRenderStats;
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_sceneHierarchyPanel.OnImGuiRender(m_SceneState == SceneState::Edit, m_ActiveScene->GetEntityEnabledMap());
		m_contentBrowserPanel->OnImGuiRender();

		if (m_ShowRenderStats)
		{
			ImGui::Begin("Renderer Stats");

			std::string name = "None";
			if (m_HoveredEntity)
				name = m_HoveredEntity.GetComponentRead<TagComponent>().m_tag;

			ImGui::Text("Hovered Entity: %s", name.c_str());

			auto stats = Renderer2D::GetStats();
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quads: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			ImGui::Text("FPS: %f", stats.FPS);

			ImGui::End();
		}

		if (m_ShowEditorSettings)
		{
			ImGui::Begin("Settings");
			ImGui::Checkbox("Show physics colliders", &m_ShowPhysicsColliders);
			ImGui::Checkbox("Show game screen size rect", &m_ShowGameScreenSizeRect);
			ImGui::ColorEdit4("Physics colliders color", m_PhysicsColliderColor.ToPtr());
			ImGui::ColorEdit4("Area color", m_AreaColor.ToPtr());
			ImGui::End();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");
		
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();

		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		
		bool ctrlPressed = Input::IsKeyPressed(RB_KEY_LEFT_CONTROL) || Input::IsKeyPressed(RB_KEY_RIGHT_CONTROL);
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered && !ctrlPressed);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		Application::Get().SetViewport(m_ViewportBounds[0].x, m_ViewportBounds[0].y, viewportPanelSize.x, viewportPanelSize.y);

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		// TODO: Might be better to open a scene from double clicking in the content browser
		// Would need to listen to some even though
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(path);
			}
			ImGui::EndDragDropTarget();
		}
		
		// Gimzos
		// TODO: Get a independent selection from the mouse picker and use events to update different panels
		Entity selectedEntity = m_sceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_gizmoType != -1 && m_SceneState == SceneState::Edit)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

			// Runtime Camera
			// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().GetCamera();
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor Camera
			const Mat4& cameraProjection = m_EditorCamera.GetProjection();
			Mat4 cameraView = m_EditorCamera.GetViewMatrix();

			// Entity transform
			auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
			Mat4 transform = transformComponent.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(RB_KEY_LEFT_CONTROL) || m_PixelSnapping;
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(cameraView.ToPtr(), cameraProjection.ToPtr(),
				(ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, transform.ToPtr(),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				Vec4 row0 = Vec4(transform[0][0], transform[0][1], transform[0][2], transform[0][3]);
				Vec4 row1 = Vec4(transform[1][0], transform[1][1], transform[1][2], transform[1][3]);
				Vec4 row2 = Vec4(transform[2][0], transform[2][1], transform[2][2], transform[2][3]);
				Vec4 row3 = Vec4(transform[3][0], transform[3][1], transform[3][2], transform[3][3]);
				Mat4 transformMat(row0, row1, row2, row3);

				Vec3 translation, rotation, scale;

				math::DecomposeTransform(transformMat, translation, rotation, scale);

				transformComponent.m_position = translation;
				transformComponent.m_rotation = rotation;		// TODO: Look into gimbal lock issue
				transformComponent.m_scale = scale;
			}
		}
		
		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();

	}

	void EditorLayer::CalculateScreenResolutionPreset()
	{
		uint32_t width = Project::GetGameWidth();
		uint32_t height = Project::GetGameHeight();

		int i = 0;
		for (ScreenResolutionPreset preset : g_ScreenResolutionPresets)
		{
			if (preset.width == width && preset.height == height)
			{
				m_ScreenResolutionPreset = i;
				return;
			}
			i++;
		}
	}

	void EditorLayer::DisplayScreenResolutionMenu()
	{
		if (ImGui::BeginMenu("Screen Resolution"))
		{
			int lastSelectedPreset = m_ScreenResolutionPreset;
			uint32_t width = Project::GetGameWidth();
			uint32_t height = Project::GetGameHeight();

			int i = 0;
			for (ScreenResolutionPreset preset : g_ScreenResolutionPresets)
			{
				if (ImGui::MenuItem(preset.name, NULL, m_ScreenResolutionPreset == i))
				{
					m_ScreenResolutionPreset = i;
					width = preset.width;
					height = preset.height;
				};
				i++;
			}

			static int customResolution[] = { 480, 270 };
			ImGui::InputInt2("Custom Resolution", customResolution);
			if (ImGui::MenuItem("Custom", NULL, m_ScreenResolutionPreset == -1))
			{
				m_ScreenResolutionPreset = -1;
				width = customResolution[0];
				height = customResolution[1];
			}
			ImGui::EndMenu();

			if (lastSelectedPreset != m_ScreenResolutionPreset)
			{
				Project::SetGameResolution(width, height);
			}
		}
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##Toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		Ref<Texture2D> icon = m_SceneState == SceneState::Edit ? m_IconPlay : m_IconStop;
		ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f));
		if (ImGui::ImageButton((ImTextureID)icon->GetRendererID(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0))
		{
			if (m_SceneState == SceneState::Edit)
			{
				OnScenePlay();
			}
			else if (m_SceneState == SceneState::Play)
			{
				OnSceneStop();
			}
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);

		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_CameraController.OnEvent(e);
		if (m_SceneState == SceneState::Edit)
		{
			m_EditorCamera.OnEvent(e);
		}

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(RB_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(RB_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(RB_BIND_EVENT_FN(EditorLayer::OnMouseButtonReleased));
		dispatcher.Dispatch<MouseMovedEvent>(RB_BIND_EVENT_FN(EditorLayer::OnMouseMoved));
	}

	// Try KeyTypedEvent
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.IsRepeat())
			return false;

		bool ctrl = Input::IsKeyPressed(RB_KEY_LEFT_CONTROL) || Input::IsKeyPressed(RB_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(RB_KEY_LEFT_SHIFT) || Input::IsKeyPressed(RB_KEY_RIGHT_SHIFT);

		switch (e.GetKeyCode())
		{
			case RB_KEY_N:
			{
				if (ctrl)
				{
					NewScene();
				}

				break;
			}
			case RB_KEY_O:
			{
				if (ctrl)
				{
					OpenProject();
				}

				break;
			}
			case RB_KEY_S:
			{
				if (ctrl)
				{
					if (shift)
						SaveSceneAs();
					else
						SaveScene();
				}

				break;
			}
			case RB_KEY_D:
			{
				if (ctrl)
				{
					DuplicateSelectedEntities();
				}

				break;
			}
			case RB_KEY_DELETE:
			{
				DeleteSelectedEntities();
				m_sceneHierarchyPanel.ResetSelectedEntities();
			}

			// Gizmos
			case RB_KEY_Q:
			{
				if (!ImGuizmo::IsUsing())
				{
					m_gizmoType = -1;
				}
				break;
			}
			case RB_KEY_W:
			{
				if (!ImGuizmo::IsUsing())
				{
					m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
				}
				break;
			}
			case RB_KEY_E:
			{
				if (!ImGuizmo::IsUsing())
				{
					m_gizmoType = ImGuizmo::OPERATION::ROTATE;
				}
				break;
			}
			case RB_KEY_R:
			{
				if (!ImGuizmo::IsUsing())
				{
					m_gizmoType = ImGuizmo::OPERATION::SCALE;
				}
				break;
			}
			default:
			{
				// Do nothing
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (m_SceneState == SceneState::Play && !Application::Get().GetIsDebugPaused())
		{
			m_ActiveScene->OnMouseButtonPressed(e.GetMouseButton());
		}
		else
		{
			// Mouse picking
			if (e.GetMouseButton() == RB_MOUSE_BUTTON_LEFT)
			{
				if (m_ViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(RB_KEY_LEFT_ALT))
					m_sceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (m_SceneState == SceneState::Play && !Application::Get().GetIsDebugPaused())
		{
			m_ActiveScene->OnMouseButtonReleased(e.GetMouseButton());
		}

		return false;
	}

	bool EditorLayer::OnMouseMoved(MouseMovedEvent& e)
	{
		if (m_SceneState == SceneState::Play && !Application::Get().GetIsDebugPaused())
		{
			m_ActiveScene->OnMouseMoved(e.GetX(), e.GetY());
		}
		return false;
	}

	bool EditorLayer::OnWindowResized(WindowResizeEvent& e)
	{
#if !RB_EDITOR
		m_ViewportSize = { (float)e.GetWidth(), (float)e.GetHeight() };
#endif
		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_sceneHierarchyPanel.SetContext(m_ActiveScene);
		m_EditorScenePath = std::filesystem::path();
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Rhombus Scene (*.rb)\0*.rb\0");
		if (!filepath.empty())
		{
			OpenScene(filepath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
#if RB_EDITOR
		if (m_SceneState != SceneState::Edit)
		{
			OnSceneStop();
		}
#endif

		Ref<Scene> newScene = CreateRef<PatienceScene>();
		SceneSerializer serializer(newScene);
#if RB_EDITOR
		if (serializer.Deserialize(path.string(), true))
#else
		if (serializer.Deserialize(path.string()))
#endif
		{
			if (m_SceneState == SceneState::Edit)
			{
				m_EditorScene = newScene;
				m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_sceneHierarchyPanel.SetContext(m_EditorScene);

				m_ActiveScene = m_EditorScene;
				m_EditorScenePath = path;
			}
			else if (m_SceneState == SceneState::Play)
			{
				m_ActiveScene = newScene;
				m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
				m_ActiveScene->OnRuntimeStart();
			}
		}
	}

	void EditorLayer::SaveScene()
	{
		if (!m_EditorScenePath.empty())
		{
			SerializeScene(m_ActiveScene, m_EditorScenePath);
		}
		else
		{
			SaveSceneAs();
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Rhombus Scene (*.rb)\0*.rb\0");
		if (!filepath.empty())
		{
			SerializeScene(m_ActiveScene, filepath);
			m_EditorScenePath = filepath;
		}
	}

	void EditorLayer::TogglePixelSnapping()
	{
		m_PixelSnapping = !m_PixelSnapping;
	}

	void EditorLayer::NewProject()
	{
		Project::New();
	}

	bool EditorLayer::OpenProject()
	{
		std::string filepath = FileDialogs::OpenFile("Rhombus Project (*.rproj)\0*.rproj\0");
		if (filepath.empty())
		{
			return false;
		}

		OpenProject(filepath);
		return true;
	}

	void EditorLayer::OpenProject(const std::filesystem::path& path)
	{
		if (Project::Load(path))
		{
			auto startScenePath = Project::GetAssetFileSystemPath(Project::GetActive()->GetConfig().StartScene);
			OpenScene(startScenePath);
			m_contentBrowserPanel = CreateScope<ContentBrowserPanel>();
		}
	}

	void EditorLayer::SaveProject()
	{
		// Project::SaveActive();
	}

	void EditorLayer::SerializeScene(Ref<Scene> scene, const std::filesystem::path& path)
	{
		std::vector<EntityID> customOrdering = m_sceneHierarchyPanel.CalculateEntityOrdering();
		SceneSerializer serializer(scene);
		serializer.Serialize(path.string(), customOrdering);
	}

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;
		m_ActiveScene = CreateRef<PatienceScene>();

		Scene::Copy(m_ActiveScene, m_EditorScene);
		m_ActiveScene->OnRuntimeStart();

		m_sceneHierarchyPanel.SetContext(m_ActiveScene);
		m_sceneHierarchyPanel.SetHierarchyDirty();
	}

	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;
		m_ActiveScene->OnRuntimeStop();
		m_ActiveScene = m_EditorScene;

		m_sceneHierarchyPanel.SetContext(m_ActiveScene);
		m_sceneHierarchyPanel.SetHierarchyDirty();
	}

	void EditorLayer::DuplicateSelectedEntities()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		std::vector<Entity> selectedEntities;
		m_sceneHierarchyPanel.GetAllSelectedEntities(selectedEntities);
		for (Entity& selectedEntity : selectedEntities)
		{
			m_EditorScene->DuplicateEntity(selectedEntity);
			m_sceneHierarchyPanel.SetHierarchyDirty();
		}
	}

	void EditorLayer::DeleteSelectedEntities()
	{
		if (m_SceneState != SceneState::Edit)
			return;

		std::vector<Entity> selectedEntities;
		m_sceneHierarchyPanel.GetAllSelectedEntities(selectedEntities);
		for (Entity& selectedEntity : selectedEntities)
		{
			m_EditorScene->DestroyEntity(selectedEntity);
			m_sceneHierarchyPanel.SetHierarchyDirty();
		}
	}

	void EditorLayer::OnOverlayRender()
	{
		if (m_SceneState == SceneState::Play)
		{
			Entity camera = m_ActiveScene->GetPrimaryCameraEntity();
			Renderer2D::BeginScene(camera.GetComponent<CameraComponent>().GetCamera(), camera.GetComponent<TransformComponent>().GetTransform());
		}
		else
		{
			Renderer2D::BeginScene(m_EditorCamera);
		}

#if RB_EDITOR
		if (m_ShowGameScreenSizeRect)
		{
			Vec3 translation = Vec3(0.0f);
			Vec3 scale = Vec3(Project::GetGameWidth(), Project::GetGameHeight(), 1.0f);

			Mat4 transform = math::Translate(Mat4::Identity(), translation)
				* math::Rotate(Mat4::Identity(), 0.0f, Vec3(0.0f, 0.0f, 1.0f))
				* math::Scale(Mat4::Identity(), scale);
			Renderer2D::DrawRect(transform, Color(1.0f, 1.0f, 1.0f, 0.8f));
		}
#endif

		if (m_ShowPhysicsColliders)
		{
			// Box Collider
			{
				std::vector<EntityID> view = m_ActiveScene->GetAllEntitiesWith<BoxCollider2DComponent>();
				for (EntityID e : view)
				{
					if (m_ActiveScene->IsEntityDisabled(e))
					{
						continue;
					}

					Entity entity = { e, m_ActiveScene.get()};
					TransformComponent& tc = entity.GetComponent<TransformComponent>();
					BoxCollider2DComponent& bc2d = entity.GetComponent<BoxCollider2DComponent>();
					Vec3 translation = tc.m_position + Vec3(bc2d.m_offset, 0.01f);
					Vec3 scale = tc.m_scale * Vec3(bc2d.m_size * 2.0f, 1.0f);

					Mat4 transform = math::Translate(Mat4::Identity(), translation)
						* math::Rotate(Mat4::Identity(), tc.m_rotation.z, Vec3(0.0f, 0.0f, 1.0f))
						* math::Scale(Mat4::Identity(), scale);

					Renderer2D::DrawRect(transform, m_PhysicsColliderColor);
				}
			}

			// Circle Collider
			{
				std::vector<EntityID> view = m_ActiveScene->GetAllEntitiesWith<CircleCollider2DComponent>();
				for (EntityID e : view)
				{
					if (m_ActiveScene->IsEntityDisabled(e))
					{
						continue;
					}

					Entity entity = { e, m_ActiveScene.get() };
					TransformComponent& tc = entity.GetComponent<TransformComponent>();
					CircleCollider2DComponent& bc2d = entity.GetComponent<CircleCollider2DComponent>();
					Vec3 translation = tc.m_position + Vec3(bc2d.m_offset, 0.01f);
					Vec3 scale = tc.m_scale * bc2d.m_radius * 2.0f;

					Mat4 transform = math::Translate(Mat4::Identity(), translation)
						* math::Rotate(Mat4::Identity(), tc.m_rotation.z, Vec3(0.0f, 0.0f, 1.0f))
						* math::Scale(Mat4::Identity(), scale);

					Renderer2D::DrawCircle(transform, m_PhysicsColliderColor, 0.05f);
				}
			}

			// Box Area
			{
				std::vector<EntityID> view = m_ActiveScene->GetAllEntitiesWith<BoxArea2DComponent>();
				for (EntityID e : view)
				{
					if (m_ActiveScene->IsEntityDisabled(e))
					{
						continue;
					}

					Entity entity = { e, m_ActiveScene.get() };
					TransformComponent& tc = entity.GetComponent<TransformComponent>();
					BoxArea2DComponent& ba2d = entity.GetComponent<BoxArea2DComponent>();
					Vec3 translation = tc.m_position + Vec3(ba2d.m_offset, 0.01f);
					Vec3 scale = tc.m_scale * Vec3(ba2d.m_size * 2.0f, 1.0f);

					Mat4 transform = math::Translate(Mat4::Identity(), translation)
						* math::Rotate(Mat4::Identity(), tc.m_rotation.z, Vec3(0.0f, 0.0f, 1.0f))
						* math::Scale(Mat4::Identity(), scale);

					Color debugColor = ba2d.GetDebugColor().a > 0.0f ? ba2d.GetDebugColor() : m_AreaColor;
					Renderer2D::DrawRect(transform, debugColor);
					Color overlayColor = debugColor;
					overlayColor.a = 0.2f;
					Renderer2D::DrawQuad(transform, overlayColor);
				}
			}
		}

		// Draw selected entity outline 
		if (Entity selectedEntity = m_sceneHierarchyPanel.GetSelectedEntity()) 
		{
			TransformComponent transform = selectedEntity.GetComponent<TransformComponent>();

			//Red
			Renderer2D::DrawRect(transform.GetTransform(), Color(0.9f, 0.9f, 0.9f, 1.0f));
		}

		Renderer2D::EndScene();
	}
}