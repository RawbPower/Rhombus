#pragma once
// Header file only intended to be included by client applications

//#include <stdio.h>
#include "Rhombus/Core/Application.h"
#include "Rhombus/Core/Layer.h"
#include "Rhombus/Core/Log.h"

#include "Rhombus/Core/DeltaTime.h"
#include "Rhombus/Core/Color.h"
#include "Rhombus/Core/Colors.h"

#include "Rhombus/Core/Input.h"
#include "Rhombus/Core/MouseButtonCodes.h"
#include "Rhombus/Core/KeyCodes.h"
#include "Rhombus/Renderer/OrthographicCameraController.h"

#include "Rhombus/ImGui/ImGuiLayer.h"

#include "Rhombus/Scenes/Scene.h"
#include "Rhombus/Scenes/Entity.h"
#include "Rhombus/Scenes/ScriptableEntity.h"

#include "Rhombus/ECS/Components/Component.h"
#include "Rhombus/ECS/Components/Area2DComponent.h"
#include "Rhombus/ECS/Components/CameraComponent.h"
#include "Rhombus/ECS/Components/CircleRendererComponent.h"
#include "Rhombus/ECS/Components/Collider2DComponent.h"
#include "Rhombus/ECS/Components/Rigidbody2DComponent.h"
#include "Rhombus/ECS/Components/ScriptComponent.h"
#include "Rhombus/ECS/Components/SpriteRendererComponent.h"
#include "Rhombus/ECS/Components/TransformComponent.h"
#include "Rhombus/ECS/Components/TweenComponent.h"

#include "Rhombus/Math/Quat.h"
#include "Rhombus/Math/Matrix.h"
#include "Rhombus/Math/Vector.h"

#include "Rhombus/Project/Project.h"

// ---Renderer--------------------
#include "Rhombus/Renderer/Renderer.h"
#include "Rhombus/Renderer/Renderer2D.h"
#include "Rhombus/Renderer/RenderCommand.h"

#include "Rhombus/Renderer/Buffer.h"
#include "Rhombus/Renderer/Shader.h"
#include "Rhombus/Renderer/Framebuffer.h"
#include "Rhombus/Renderer/Texture.h"
#include "Rhombus/Renderer/SubTexture2D.h"
#include "Rhombus/Renderer/VertexArray.h"

#include "Rhombus/Renderer/OrthographicCamera.h"
//--------------------------------

// ---Animation-------------------
#include "Rhombus/Animation/Tween.h"
#include "Rhombus/Animation/EasingFunctions.h"
//--------------------------------

using namespace rhombus;