#pragma once

namespace dae
{
	enum class Events
	{
		PeterPepper_Died = 0,
		Item_Fell = 1,
		Enemy_Peppered = 2
	};

	enum class SpriteIndices : uint32_t
	{
		Idle = 0,
		WalkLeft = 1,
		WalkRight = 2,
		WalkUp = 3,
		WalkDown = 4,
		Death = 5,
		Peppered = 6
	};
}