#pragma once

#include "Rhombus/ECS/System.h"
#include "Rhombus/Core/DeltaTime.h"

namespace rhombus
{
	const float GRAVITY = -16.0f * 9.8f;			// 16 pixels -> 1 meter

	class PixelPlatformerPhysicsSystem : public System
	{
	public:
		PixelPlatformerPhysicsSystem(Scene* scene) : System(scene)
		{
		}

		void Update(DeltaTime dt);

	private:
		void UpdateDynamicBody(Entity entity, DeltaTime dt);
		void Move(Entity entity, Vec2 translation);
		bool Collide(Entity entity, Vec2 position);
	};
}
