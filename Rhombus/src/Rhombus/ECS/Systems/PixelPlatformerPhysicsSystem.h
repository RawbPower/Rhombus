#pragma once

#include "Rhombus/ECS/System.h"
#include "Rhombus/Core/DeltaTime.h"

namespace rhombus
{
	const float GRAVITY = -16.0f * 9.8f;			// 16 pixels -> 1 meter
	const DeltaTime STEP_TIME = 1.0f / 240.0f;		// 240 Hz physics update (might need to lower as we go)

	class PixelPlatformerPhysicsSystem : public System
	{
	public:
		PixelPlatformerPhysicsSystem(Scene* scene) : System(scene)
		{
		}

		void Update(DeltaTime dt);

	private:
		void Integrate(DeltaTime dt);
		void IntegrateDynamicBody(Entity entity, DeltaTime dt);
		void Move(Entity entity, Vec2 translation);
		bool Collide(Entity entity, Vec2 position);

		float m_accumulator = 0.0f;
	};
}
