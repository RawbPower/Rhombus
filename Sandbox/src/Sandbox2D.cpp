#include "Sandbox2D.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const uint32_t s_MapWidth = 16;
static const char* s_MapTiles =
"BBBBBBBBBBBBBBBB"
"BBBBBBBBBBBBBBBB"
"BBBBBBBBBBBBBBBB"
"BBBBFFFFFFFFBBBB"
"BBBBFFFFFFFFBBBB"
"BBBBFFFFFFFFBBBB"
"BBBBFFFFFFFFBBBB"
"BBBBFFFFFFFFBBBB"
"BBBBFFFFFFFFBBBB"
"BBBBBBBBBBBBBBBB"
"BBBBBBBBBBBBBBBB"
"BBBBBBBBBBBBBBBB";

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(960.0f / 540.0f, true), m_SquareColor({ 0.2f, 0.3f, 0.8f, 1.0f })
{

}

void Sandbox2D::OnAttach()
{
	RB_PROFILE_FUNCTION();

	m_CheckerboardTexture = rhombus::Texture2D::Create("assets/textures/Checkerboard.png");
	m_LogoTexture = rhombus::Texture2D::Create("assets/textures/ChernoLogo.png");
	m_SpriteSheetIndoor = rhombus::Texture2D::Create("assets/PocketRPG/Pocket_RPG_v5/Pocket_RPG_v3/Indoors_misc.png");
	m_SpriteSheetOutdoor = rhombus::Texture2D::Create("assets/PocketRPG/Pocket_RPG_v5/Pocket_RPG_v3/Outdoors_misc.png");

	m_TextureStairs = rhombus::SubTexture2D::CreateFromCoords(m_SpriteSheetIndoor, { 7, 2 }, { 16, 16 });
	m_TextureDoor = rhombus::SubTexture2D::CreateFromCoords(m_SpriteSheetIndoor, { 4, 0 }, { 16, 16 });
	m_TextureVendingMachine = rhombus::SubTexture2D::CreateFromCoords(m_SpriteSheetOutdoor, { 4, 2 }, { 16, 16 }, { 1, 2 });

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / m_MapWidth;

	m_TextureMap['F'] = rhombus::SubTexture2D::CreateFromCoords(m_SpriteSheetIndoor, { 0, 5 }, { 16, 16 });
	m_TextureMap['B'] = rhombus::SubTexture2D::CreateFromCoords(m_SpriteSheetIndoor, { 5, 8 }, { 16, 16 });

	// Init particles
	m_Particle.colorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.colorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.sizeBegin = 0.5f, m_Particle.sizeVariation = 0.3f, m_Particle.sizeEnd = 0.0f;
	m_Particle.lifetime = 1.0f;
	m_Particle.velocity = { 0.0f, 0.0f };
	m_Particle.velocityVariation = { 3.0f, 1.0f };
	m_Particle.position = { 0.0f, 0.0f };

	//m_CameraController.SetZoomLevel(5.0f);
}

void Sandbox2D::OnDetach()
{
	RB_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(rhombus::DeltaTime dt)
{
	RB_PROFILE_FUNCTION();

	// Update
	m_CameraController.OnUpdate(dt);

	// Render
	rhombus::Renderer2D::ResetStats();
	{
		RB_PROFILE_SCOPE("Renderer Prep");
		rhombus::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		rhombus::RenderCommand::Clear();
	}

#if 1
	{
		static float rotation = 0.0f;
		rotation += dt * 50.0f;

		RB_PROFILE_SCOPE("Renderer Draw");

		rhombus::Renderer2D::BeginScene(m_CameraController.GetCamera());
		rhombus::Renderer2D::DrawQuad({ 0.5f, -0.5f }, 3.14f / 3.0f, { 0.5, 0.75 }, { 0.3f, 0.1f, 0.8f, 1.0f });
		rhombus::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.1f }, glm::radians(rotation), { 1.0, 1.0 }, m_CheckerboardTexture, 20.0f);
		rhombus::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, 0.0f, { 10.0, 10.0 }, m_CheckerboardTexture, 10.0f);
		rhombus::Renderer2D::DrawQuad({ -1.0f, 0.0f }, 0.0f, { 0.8, 0.8 }, { 0.8f, 0.2f, 0.3f, 1.0f });
		rhombus::Renderer2D::SetFPDStat(dt);
		rhombus::Renderer2D::EndScene();

		rhombus::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.5f };
				rhombus::Renderer2D::DrawQuad({ x, y, 0.0f }, 0.0f, { 0.45, 0.45 }, color);
			}
		}
		rhombus::Renderer2D::SetFPDStat(dt);
		rhombus::Renderer2D::EndScene();
	}
#endif

	if (rhombus::Input::IsMouseButtonPressed(RB_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = rhombus::Input::GetMousePosition();
		auto width = rhombus::Application::Get().GetWindow().GetWidth();
		auto height = rhombus::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 1; i++)
		{
			m_ParticleSystem.Emit(m_Particle);
		}
	}

	m_ParticleSystem.OnUpdate(dt);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());

	rhombus::Renderer2D::BeginScene(m_CameraController.GetCamera());

	/*for (uint32_t y = 0; y < m_MapHeight; y++)
	{
		for (uint32_t x = 0; x < m_MapWidth; x++)
		{
			char tileType = s_MapTiles[x + y * m_MapWidth];
			rhombus::Renderer2D::DrawQuad({ x - m_MapWidth * 0.5f, m_MapHeight - y - m_MapHeight * 0.5f, 0.0f }, 0.0f, { 1.0, 1.0 }, m_TextureMap[tileType]);
		}
	}*/

	//rhombus::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, 0.0f, { 1.0, 1.0 }, m_TextureStairs);
	//rhombus::Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.0f }, 0.0f, { 1.0, 1.0 }, m_TextureDoor);
	//rhombus::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, 0.0f, { 1.0, 2.0 }, m_TextureVendingMachine);
	rhombus::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	RB_PROFILE_FUNCTION();

	ImGui::ShowStyleEditor();

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
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			// Disabling fullscreen would allow the window to be moved to the front of other windows,
			// which we can't undo at the moment without finer window depth/z control.
			//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

			if (ImGui::MenuItem("Exit")) rhombus::Application::Get().Close();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::Begin("Settings");

	auto stats = rhombus::Renderer2D::GetStats();
	ImGui::Text("Renderer 2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
	ImGui::Text("FPS: %f", stats.FPS);

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	uint32_t textureID = m_CheckerboardTexture->GetRendererID();
	ImGui::Image((void*)textureID, ImVec2{256.0f,256.0f});

	ImGui::End();

	ImGui::End();
}

void Sandbox2D::OnEvent(rhombus::Event& e)
{
	m_CameraController.OnEvent(e);
}