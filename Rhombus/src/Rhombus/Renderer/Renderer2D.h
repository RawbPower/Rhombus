#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "Camera.h"
#include "EditorCamera.h"
#include "Framebuffer.h"

#include "Rhombus/ECS/Components/SpriteRendererComponent.h"

namespace rhombus
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void BeginScene(const Camera& camera, const Mat4& transform);
		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const OrthographicCamera& camera);  // TODO: Remove
		static void EndScene();
		static void Flush();

		// Primitives
		static void DrawQuad(const Vec2& position, const float& angle, const Vec2& scale, const Color& color);
		static void DrawQuad(const Vec3& position, const float& angle, const Vec2& scale, const Color& color);
		static void DrawQuad(const Vec2& position, const float& angle, const Vec2& scale, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);
		static void DrawQuad(const Vec2& position, const float& angle, const Vec2& scale, const Ref<Texture2D>& texture, const Color& color = Color(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const Vec2& position, const float& angle, const Vec2& scale, const Ref<SubTexture2D>& subTexture, const Color& color = Color(1.0f), float tilingFactor = 1.0f);
		static void DrawQuad(const Vec3& position, const float& angle, const Vec2& scale, const Ref<Texture2D>& texture, float tilingFactor = 1.0f);
		static void DrawQuad(const Vec3& position, const float& angle, const Vec2& scale, const Ref<SubTexture2D>& subTexture, float tilingFactor = 1.0f);
		static void DrawQuad(const Vec3& position, const float& angle, const Vec2& scale, const Ref<Texture2D>& texture, const Color& color, float tilingFactor = 1.0f);
		static void DrawQuad(const Vec3& position, const float& angle, const Vec2& scale, const Ref<SubTexture2D>& subTexture, const Color& color, float tilingFactor = 1.0f);

		static void DrawQuad(const Mat4& transform, const Color& color, int entityID = -1);
		static void DrawQuad(const Mat4& transform, const Ref<Texture2D>& texture, const Color& color = Color(1.0f), float tilingFactor = 1.0f, int entityID = -1, Vec2 flip = Vec2(1.0f), bool pixelPerfect = true);
		static void DrawQuad(const Mat4& transform, const Ref<SubTexture2D>& subTexture, const Color& color = Color(1.0f), float tilingFactor = 1.0f, int entityID = -1, Vec2 flip = Vec2(1.0f), bool pixelPerfect = true);

		static void DrawQuadOverlay(const Vec2& position, const float& angle, const Vec2& scale, const Ref<Texture2D>& texture, const Color& color = Color(1.0f), float tilingFactor = 1.0f);

		static void DrawLine(const Vec3& p0, Vec3& p1, const Color& color, int entityID = -1);

		static void DrawRect(const Vec3& position, const Vec2& size, const Color& color, int entityID = -1);
		static void DrawRect(const Mat4& transform, const Color& color, int entityID = -1);

		static void DrawCircle(const Mat4& transform, const Color& color, float thickness = 1.0f, float fade = 0.0f, int entityID = -1);

		static void DrawSprite(const Mat4& transform, const SpriteRendererComponent& src, int entityID);

		static void DrawFrambuffer(Ref<Framebuffer> frameBuffer);

		static float GetLineWidth();
		static void SetLineWidth(float width);

		static Mat4 GetViewProjectionMatrix();
		static bool IsScreenPositionWithViewPort(int x, int y);
		static Vec3 ConvertScreenToWorldSpace(int x, int y);
		static Vec3 RaycastScreenPositionToWorldSpace(int x, int y, float planeDepth, const Mat4 projectionMatrix, const Mat4 viewMatrix);

		static Mat4 CorrectTransformForPixelPerfect(Mat4 transform, const Ref<Texture2D>& texture);

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			float FPS = 0.0f;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();
		static void SetFPDStat(float dt);

	private:
		static void StartBatch();
		static void NextBatch();

		static Vec3 ConvertScreenToWorldSpace(Vec3 ndc);
		static Vec3 RaycastScreenPositionToWorldSpace(Vec3 ndc, float planeDepth, const Mat4 projectionMatrix, const Mat4 viewMatrix);
		static Vec3 CalculateScreenRaycast(Vec3 ndc, const Mat4 projectionMatrix, const Mat4 viewMatrix);
	};
}
