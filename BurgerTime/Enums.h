#pragma once

namespace dae
{
	enum class Events : int
	{
		Player_Died = 10,
		Item_Fell = 11,
		Enemy_Peppered = 12
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

	enum class WalkingDirection
	{
		Left,
		Right,
		Up,
		Down
	};
}