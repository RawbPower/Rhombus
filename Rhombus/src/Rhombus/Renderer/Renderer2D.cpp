#include "rbpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "Rhombus/Core/Application.h"
#include "Rhombus/Math/Math.h"

namespace rhombus
{
	float screenVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	struct QuadVertex
	{
		Vec3 Position;
		Color Color;
		Vec2 TexCoord;
		float TextureIndex;
		float TilingFactor;

		// Editor only
		int EntityID;
	};

	struct CircleVertex
	{
		Vec3 WorldPosition;
		Vec3 LocalPosition;
		Color Color;
		float Thickness;
		float Fade;

		// Editor-only
		int EntityID;
	};

	struct LineVertex
	{
		Vec3 Position;
		Color Color;

		// Editor-only
		int EntityID;
	};

	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 20000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;		// TODO: Renderer Capabilities

		Ref<VertexArray> ScreenVertexArray;
		Ref<VertexBuffer> ScreenVertexBuffer;
		Ref<Shader> ScreenShader;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> QuadShader;
		Ref<Texture2D> BlankTexture;

		Ref<VertexArray> CircleVertexArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> CircleShader;

		Ref<VertexArray> LineVertexArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> LineShader;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t CircleIndexCount = 0;
		CircleVertex* CircleVertexBufferBase = nullptr;
		CircleVertex* CircleVertexBufferPtr = nullptr;

		uint32_t LineVertexCount = 0;
		LineVertex* LineVertexBufferBase = nullptr;
		LineVertex* LineVertexBufferPtr = nullptr;

		float LineWidth = 2.0f;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;		// 0 is white texture

		Vec4 QuadVertexPosition[4];
		Mat4 ViewProjectionMatrix;

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_Data;

	void Renderer2D::Init()
	{
		RB_PROFILE_FUNCTION();

		s_Data.ScreenVertexArray = VertexArray::Create();

		s_Data.ScreenVertexBuffer = VertexBuffer::Create(&screenVertices[0], sizeof(screenVertices));

		s_Data.ScreenVertexBuffer->SetLayout({
			{ ShaderDataType::Float2, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		});

		s_Data.ScreenVertexArray->AddVertexBuffer(s_Data.ScreenVertexBuffer);

		s_Data.QuadVertexArray = VertexArray::Create();

		s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));

		s_Data.QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TextureIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" },
			{ ShaderDataType::Int, "a_EntityID" }
		});

		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;

		// Circle
		s_Data.CircleVertexArray = VertexArray::Create();

		s_Data.CircleVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(CircleVertex));
		s_Data.CircleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_WorldPosition" },
			{ ShaderDataType::Float3, "a_LocalPosition" },
			{ ShaderDataType::Float4, "a_Color"         },
			{ ShaderDataType::Float,  "a_Thickness"     },
			{ ShaderDataType::Float,  "a_Fade"          },
			{ ShaderDataType::Int,    "a_EntityID"      }
		});
		s_Data.CircleVertexArray->AddVertexBuffer(s_Data.CircleVertexBuffer);
		s_Data.CircleVertexArray->SetIndexBuffer(quadIB); // Use quad IB
		s_Data.CircleVertexBufferBase = new CircleVertex[s_Data.MaxVertices];

		// Lines
		s_Data.LineVertexArray = VertexArray::Create();

		s_Data.LineVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(LineVertex));
		s_Data.LineVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color"    },
			{ ShaderDataType::Int,    "a_EntityID" }
			});
		s_Data.LineVertexArray->AddVertexBuffer(s_Data.LineVertexBuffer);
		s_Data.LineVertexBufferBase = new LineVertex[s_Data.MaxVertices];

		s_Data.BlankTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data.BlankTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		s_Data.ScreenShader = Shader::Create(Application::Get().GetPathRelativeToEngineDirectory("resources/shaders/Renderer2D_Screen.glsl"));
		s_Data.ScreenShader->Bind();
		s_Data.ScreenShader->SetInt("u_ScreenTexture", 0);

		s_Data.QuadShader = Shader::Create(Application::Get().GetPathRelativeToEngineDirectory("resources/shaders/Renderer2D_Quad.glsl"));
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);

		s_Data.CircleShader = Shader::Create(Application::Get().GetPathRelativeToEngineDirectory("resources/shaders/Renderer2D_Circle.glsl"));
		s_Data.LineShader = Shader::Create(Application::Get().GetPathRelativeToEngineDirectory("resources/shaders/Renderer2D_Line.glsl"));

		s_Data.TextureSlots[0] = s_Data.BlankTexture;

		s_Data.QuadVertexPosition[0] = { -0.5, -0.5, 0.0, 1.0f };
		s_Data.QuadVertexPosition[1] = { 0.5, -0.5, 0.0, 1.0f };
		s_Data.QuadVertexPosition[2] = { 0.5, 0.5, 0.0, 1.0f };
		s_Data.QuadVertexPosition[3] = { -0.5, 0.5, 0.0, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		RB_PROFILE_FUNCTION();

		delete[] s_Data.QuadVertexBufferBase;	
	}

	// TODO: Manage multiple shaders better
	void SetShaderViewProjection(Mat4 viewProjection)
	{
		s_Data.ViewProjectionMatrix = viewProjection;
		s_Data.QuadShader->Bind();
		s_Data.QuadShader->SetMat4("u_ViewProjection", viewProjection);
		s_Data.CircleShader->Bind();
		s_Data.CircleShader->SetMat4("u_ViewProjection", viewProjection);
		s_Data.LineShader->Bind();
		s_Data.LineShader->SetMat4("u_ViewProjection", viewProjection);
	}

	void Renderer2D::BeginScene()
	{
		RB_PROFILE_FUNCTION();

		Mat4 viewProjection = Mat4::Identity();

		SetShaderViewProjection(viewProjection);

		StartBatch();
	}

	void Renderer2D::BeginScene(const Camera& camera, const Mat4& transform)
	{
		RB_PROFILE_FUNCTION();

		Mat4 viewProjection = camera.GetProjection() * transform.Inverse();

		SetShaderViewProjection(viewProjection);

		StartBatch();
	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		RB_PROFILE_FUNCTION();

		Mat4 viewProjection = camera.GetViewProjection();

		SetShaderViewProjection(viewProjection);

		StartBatch();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		RB_PROFILE_FUNCTION();

		Mat4 viewProjection = camera.GetViewProjectionMatrix();

		SetShaderViewProjection(viewProjection);

		StartBatch();
	}

	void Renderer2D::EndScene()
	{
		RB_PROFILE_FUNCTION();

		Flush();
	}

	void Renderer2D::Flush()
	{
		RB_PROFILE_FUNCTION();

		if (s_Data.QuadIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.QuadVertexBufferPtr - (uint8_t*)s_Data.QuadVertexBufferBase);
			s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);

			for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
			{
				s_Data.TextureSlots[i]->Bind(i);
			}

			s_Data.QuadShader->Bind();
			RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.CircleIndexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.CircleVertexBufferPtr - (uint8_t*)s_Data.CircleVertexBufferBase);
			s_Data.CircleVertexBuffer->SetData(s_Data.CircleVertexBufferBase, dataSize);

			s_Data.CircleShader->Bind();
			RenderCommand::DrawIndexed(s_Data.CircleVertexArray, s_Data.CircleIndexCount);
			s_Data.Stats.DrawCalls++;
		}

		if (s_Data.LineVertexCount)
		{
			uint32_t dataSize = (uint32_t)((uint8_t*)s_Data.LineVertexBufferPtr - (uint8_t*)s_Data.LineVertexBufferBase);
			s_Data.LineVertexBuffer->SetData(s_Data.LineVertexBufferBase, dataSize);

			s_Data.LineShader->Bind();
			RenderCommand::SetLineWidth(s_Data.LineWidth);
			RenderCommand::DrawLines(s_Data.LineVertexArray, s_Data.LineVertexCount);
			s_Data.Stats.DrawCalls++;
		}
	}

	void Renderer2D::StartBatch()
	{
		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.CircleIndexCount = 0;
		s_Data.CircleVertexBufferPtr = s_Data.CircleVertexBufferBase;

		s_Data.LineVertexCount = 0;
		s_Data.LineVertexBufferPtr = s_Data.LineVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::NextBatch()
	{
		Flush();
		StartBatch();
	}

	void Renderer2D::DrawQuad(const Vec2& position, const float& angle, const Vec2& scale, const Color& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, angle, scale, color);
	}

	void Renderer2D::DrawQuad(const Vec3& position, const float& angle, const Vec2& scale, const Color& color)
	{
		Mat4 transform = Mat4::Identity();
		if (abs(angle) > 0.001f)
		{
			transform = math::Translate(Mat4::Identity(), position) * math::Rotate(Mat4::Identity(), angle, Vec3(0.0f, 0.0f, 1.0f)) * math::Scale(Mat4::Identity(), {scale.x, scale.y, 1.0f});
		}
		else
		{
			transform = math::Translate(Mat4::Identity(), position) * math::Scale(Mat4::Identity(), {scale.x, scale.y, 1.0f});
		}

		DrawQuad(transform, color);
	}

	void Renderer2D::DrawQuad(const Vec2& position, const float& angle, const Vec2& scale, const Ref<Texture2D>& texture, float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, angle, scale, texture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const Vec2& position, const float& angle, const Vec2& scale, const Ref<Texture2D>& texture, const Color& color, float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, angle, scale, texture, color, tilingFactor);
	}

	void Renderer2D::DrawQuad(const Vec2& position, const float& angle, const Vec2& scale, const Ref<SubTexture2D>& subTexture, const Color& color, float tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, angle, scale, subTexture, color, tilingFactor);
	}

	void Renderer2D::DrawQuad(const Vec3& position, const float& angle, const Vec2& scale, const Ref<Texture2D>& texture, float tilingFactor)
	{
		DrawQuad(position, angle, scale, texture, Color(1.0f), tilingFactor);
	}

	void Renderer2D::DrawQuad(const Vec3& position, const float& angle, const Vec2& scale, const Ref<SubTexture2D>& subTexture, float tilingFactor)
	{
		DrawQuad(position, angle, scale, subTexture, Color(1.0f), tilingFactor);
	}

	void Renderer2D::DrawQuad(const Vec3& position, const float& angle, const Vec2& scale, const Ref<Texture2D>& texture, const Color& color, float tilingFactor)
	{
		Mat4 transform = Mat4::Identity();
		if (abs(angle) > 0.001f)
		{
			transform = math::Translate(Mat4::Identity(), position) * math::Rotate(Mat4::Identity(), angle, Vec3(0.0f, 0.0f, 1.0f)) * math::Scale(Mat4::Identity(), {scale.x, scale.y, 1.0f});
		}
		else
		{
			transform = math::Translate(Mat4::Identity(), position) * math::Scale(Mat4::Identity(), {scale.x, scale.y, 1.0f});
		}

		DrawQuad(transform, texture, color, tilingFactor);
	}

	void Renderer2D::DrawQuad(const Vec3& position, const float& angle, const Vec2& scale, const Ref<SubTexture2D>& subTexture, const Color& color, float tilingFactor)
	{
		Mat4 transform = Mat4::Identity();
		if (abs(angle) > 0.001f)
		{
			transform = math::Translate(Mat4::Identity(), position) * math::Rotate(Mat4::Identity(), angle, Vec3(0.0f, 0.0f, 1.0f)) * math::Scale(Mat4::Identity(), {scale.x, scale.y, 1.0f});
		}
		else
		{
			transform = math::Translate(Mat4::Identity(), position) * math::Scale(Mat4::Identity(), {scale.x, scale.y, 1.0f});
		}

		DrawQuad(transform, subTexture, color, tilingFactor);
	}

	void Renderer2D::DrawQuad(const Mat4& transform, const Color& color, int entityID)
	{
		RB_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		constexpr float textureIndex = 0.0f;		// Blank texture
		const Vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
		constexpr float tilingFactor = 1.0f;

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			NextBatch();
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const Mat4& transform, const Ref<Texture2D>& texture, const Color& color, float tilingFactor, int entityID, Vec2 flip, bool pixelPerfect)
	{
		RB_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const Vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		Mat4 renderTransform = pixelPerfect ? CorrectTransformForPixelPerfect(transform, texture) : transform;
		
		// TODO: Add PPU
		Mat4 scaledTransform = math::Scale(renderTransform, Vec3(flip.x * (float)texture->GetWidth(), flip.y * (float)texture->GetHeight(), 1.0f));

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			NextBatch();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			{
				NextBatch();
			}

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = scaledTransform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const Mat4& transform, const Ref<SubTexture2D>& subTexture, const Color& color, float tilingFactor, int entityID, Vec2 flip, bool pixelPerfect)
	{
		RB_PROFILE_FUNCTION();

		constexpr size_t quadVertexCount = 4;
		const Vec2* textureCoords = subTexture->GetTexCoords();
		const Ref<Texture2D> texture = subTexture->GetTexture();
		const float width = (float)subTexture->GetWidth();
		const float height = (float)subTexture->GetHeight();

		Mat4 renderTransform = pixelPerfect ? CorrectTransformForPixelPerfect(transform, texture) : transform;

		// TODO: Add PPU
		Mat4 scaledTransform = math::Scale(renderTransform, Vec3(flip.x * width, flip.y * height, 1.0f));

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			NextBatch();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			{
				NextBatch();
			}

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data.QuadVertexBufferPtr->Position = scaledTransform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = entityID;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuadOverlay(const Vec2& position, const float& angle, const Vec2& scale, const Ref<Texture2D>& texture, const Color& color, float tilingFactor)
	{
		RB_PROFILE_FUNCTION();

		Mat4 transform = Mat4::Identity();
		if (abs(angle) > 0.001f)
		{
			transform = math::Translate(Mat4::Identity(), { position.x, position.y, 0.0f }) * math::Rotate(Mat4::Identity(), angle, Vec3(0.0f, 0.0f, 1.0f)) * math::Scale(Mat4::Identity(), { scale.x, scale.y, 1.0f });
		}
		else
		{
			transform = math::Translate(Mat4::Identity(), { position.x, position.y, 0.0f }) * math::Scale(Mat4::Identity(), { scale.x, scale.y, 1.0f });
		}

		constexpr size_t quadVertexCount = 4;
		const Vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		// TODO: Add PPU
		Viewport viewport = Application::Get().GetViewport();
		Mat4 scaledTransform = math::Scale(transform, Vec3((float)texture->GetWidth()/viewport.width, (float)texture->GetHeight()/viewport.height, 1.0f));

		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		{
			NextBatch();
		}

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (*s_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
			{
				NextBatch();
			}

			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;
			s_Data.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			Vec3 position = scaledTransform * s_Data.QuadVertexPosition[i];
			s_Data.QuadVertexBufferPtr->Position = position;
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TextureIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr->EntityID = -1;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawLine(const Vec3& p0, Vec3& p1, const Color& color, int entityID)
	{
		s_Data.LineVertexBufferPtr->Position = p0;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexBufferPtr->Position = p1;
		s_Data.LineVertexBufferPtr->Color = color;
		s_Data.LineVertexBufferPtr->EntityID = entityID;
		s_Data.LineVertexBufferPtr++;

		s_Data.LineVertexCount += 2;
	}

	void Renderer2D::DrawRect(const Vec3& position, const Vec2& size, const Color& color, int entityID)
	{
		Vec3 p0 = Vec3(position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		Vec3 p1 = Vec3(position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z);
		Vec3 p2 = Vec3(position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z);
		Vec3 p3 = Vec3(position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z);

		DrawLine(p0, p1, color, entityID);
		DrawLine(p1, p2, color, entityID);
		DrawLine(p2, p3, color, entityID);
		DrawLine(p3, p0, color, entityID);
	}

	void Renderer2D::DrawRect(const Mat4& transform, const Color& color, int entityID)
	{
		Vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * s_Data.QuadVertexPosition[i];

		DrawLine(lineVertices[0], lineVertices[1], color, entityID);
		DrawLine(lineVertices[1], lineVertices[2], color, entityID);
		DrawLine(lineVertices[2], lineVertices[3], color, entityID);
		DrawLine(lineVertices[3], lineVertices[0], color, entityID);
	}

	void Renderer2D::DrawCircle(const Mat4& transform, const Color& color, float thickness, float fade, int entityID)
	{
		RB_PROFILE_FUNCTION();

		// TODO: implement for circles
		// if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
		// 	NextBatch();

		for (size_t i = 0; i < 4; i++)
		{
			s_Data.CircleVertexBufferPtr->WorldPosition = transform * s_Data.QuadVertexPosition[i];
			s_Data.CircleVertexBufferPtr->LocalPosition = s_Data.QuadVertexPosition[i] * 2.0f;
			s_Data.CircleVertexBufferPtr->Color = color;
			s_Data.CircleVertexBufferPtr->Thickness = thickness;
			s_Data.CircleVertexBufferPtr->Fade = fade;
			s_Data.CircleVertexBufferPtr->EntityID = entityID;
			s_Data.CircleVertexBufferPtr++;
		}

		s_Data.CircleIndexCount += 6;

		s_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawSprite(const Mat4& transform, const SpriteRendererComponent& src, int entityID)
	{
		if (src.UseSubTexture())
		{
			DrawQuad(transform, src.m_subtexture, src.GetColor(), 1.0f, entityID, src.GetFlip());
		}
		else if (src.m_texture)
		{
			DrawQuad(transform, src.m_texture, src.GetColor(), 1.0f, entityID, src.GetFlip());
		}
		else
		{
			DrawQuad(transform, src.GetColor(), entityID);
		}
	}

	void Renderer2D::DrawFrambuffer(Ref<Framebuffer> frameBuffer)
	{
		s_Data.ScreenShader->Bind();
		s_Data.ScreenVertexArray->Bind();
		frameBuffer->BindTexture();
		RenderCommand::DrawQuad();
	}

	float Renderer2D::GetLineWidth()
	{
		return s_Data.LineWidth;
	}

	void Renderer2D::SetLineWidth(float width)
	{
		s_Data.LineWidth = width;
	}

	Mat4 Renderer2D::GetViewProjectionMatrix()
	{ 
		return s_Data.ViewProjectionMatrix; 
	}

	bool Renderer2D::IsScreenPositionWithViewPort(int x, int y)
	{
		Viewport viewport = Application::Get().GetViewport();
		Vec3 fC = Vec3(x - viewport.x, y - viewport.y, 0.0f);
		Vec3 ndc = Vec3(fC.x / viewport.width, 1.0 - fC.y / viewport.height, fC.z) * 2.0f - 1.0f;

		return ndc.x >= -1.0f && ndc.x <= 1.0f && ndc.y >= -1.0f && ndc.y <= 1.0f;
	}

	Vec3 Renderer2D::ConvertScreenToWorldSpace(int x, int y)
	{
		Viewport viewport = Application::Get().GetViewport();
		Vec3 fC = Vec3(x - viewport.x, y - viewport.y, 0.0f);
		Vec3 ndc = Vec3(fC.x / viewport.width, 1.0 - fC.y / viewport.height, fC.z) * 2.0f - 1.0f;
		ndc.z = 0.0f;
		return ConvertScreenToWorldSpace(ndc);
	}

	Vec3 Renderer2D::ConvertScreenToWorldSpace(Vec3 ndc)
	{
		Mat4 viewProjection = Renderer2D::GetViewProjectionMatrix();
		Vec4 worldCoords =  viewProjection.Inverse() * Vec4(ndc, 1.0f);
		return worldCoords;
	}

	Vec3 Renderer2D::RaycastScreenPositionToWorldSpace(int x, int y, float planeDepth, const Mat4 projectionMatrix, const Mat4 viewMatrix)
	{
		Viewport viewport = Application::Get().GetViewport();
		Vec3 fC = Vec3(x - viewport.x, y - viewport.y, 0.0f);
		Vec3 ndc = Vec3(fC.x / viewport.width, 1.0 - fC.y / viewport.height, fC.z) * 2.0f - 1.0f;
		ndc.z = 1.0f;
		return RaycastScreenPositionToWorldSpace(ndc, planeDepth, projectionMatrix, viewMatrix);
	}

	Vec3 Renderer2D::RaycastScreenPositionToWorldSpace(Vec3 ndc, float planeDepth, const Mat4 projectionMatrix, const Mat4 viewMatrix)
	{
		Vec3 worldRaycast = CalculateScreenRaycast(ndc, projectionMatrix, viewMatrix);
		Vec3 cameraPosition = viewMatrix.Inverse().d();
		Vec3 projectionNormal = Vec3(0.0f, 0.0f, 1.0f);
		Vec3 projectionPlane = Vec3(0.0f, 0.0f, planeDepth);
		float t = -(cameraPosition.Dot(projectionNormal) + projectionPlane.GetMagnitude()) / (worldRaycast.Dot(projectionNormal));
		Vec3 cursorCoords = cameraPosition + worldRaycast * t;
		return cursorCoords;
	}

	Vec3 Renderer2D::CalculateScreenRaycast(Vec3 ndc, const Mat4 projectionMatrix, const Mat4 viewMatrix)
	{
		Vec4 homogeneousClipCoords = Vec4(ndc.x, ndc.y, -1.0f, 1.0f);
		Vec4 eyeCoords = projectionMatrix.Inverse() * homogeneousClipCoords;
		eyeCoords = Vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);			// Set zw to mean "forwards, and not a point"
		Vec4 worldCoords4d = viewMatrix.Inverse() * eyeCoords;
		Vec3 worldCoords = Vec3(worldCoords4d.x, worldCoords4d.y, worldCoords4d.z);
		worldCoords.Normalize();
		return worldCoords;
	}

	float RoundToNearestPixelCenter(float positionCoord)
	{
		float rounded = math::Round(positionCoord);
		float decimel = positionCoord - rounded;
		return decimel < 0.0f ? rounded - 0.5f : rounded + 0.5f;
	}

	float RoundToNearestPixelEdge(float positionCoord)
	{
		return math::Round(positionCoord);
	}

	Mat4 Renderer2D::CorrectTransformForPixelPerfect(Mat4 transform, const Ref<Texture2D>& texture)
	{
		Mat4 vCorrectedTransform = transform;

		if (texture->GetWidth() % 2 == 1)
		{
			vCorrectedTransform.cols[3].x = RoundToNearestPixelCenter(vCorrectedTransform.d().x);
		}
		else
		{
			vCorrectedTransform.cols[3].x = RoundToNearestPixelEdge(vCorrectedTransform.d().x);
		}

		if (texture->GetHeight() % 2 == 1)
		{
			vCorrectedTransform.cols[3].y = RoundToNearestPixelCenter(vCorrectedTransform.d().y);
		}
		else
		{
			vCorrectedTransform.cols[3].y = RoundToNearestPixelEdge(vCorrectedTransform.d().y);
		}

		return vCorrectedTransform;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::SetFPDStat(float dt)
	{
		s_Data.Stats.FPS = dt > 0.0f ? 1.0f/dt : 0.0f;
	}
}