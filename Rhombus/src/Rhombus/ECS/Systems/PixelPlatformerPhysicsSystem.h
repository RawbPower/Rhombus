#pragma once

#include "Rhombus/ECS/System.h"
#include "Rhombus/Core/DeltaTime.h"

namespace rhombus
{
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
	};

}
