#pragma once

namespace dae
{
	enum class Events : int//event 0 and 1 cannot be used as those are the collider events enter and exit
	{
		Player_Died = 10,
		Item_Fell = 11,
		Enemy_Peppered = 12,
		Game_Over = 13,
		Extra_Life_Gained,
		Reset_Pos,
		Plate_Full,
		Game_Won,
		Mr_HotDog_Died,
		Mr_Egg_Died,
		Mr_Pickle_Died,
		Drop_Enemy_1,
		Drop_Enemy_2,
		Drop_Enemy_3,
		Drop_Enemy_4,
		Drop_Enemy_5,
		Drop_Enemy_6
	};

	enum class SpriteIndices : uint32_t
	{
		Idle = 0,
		WalkLeft = 1,
		WalkRight = 2,
		WalkUp = 3,
		WalkDown = 4,
		Death = 5,
		Peppered = 6,//only for the Mr. HotDog, Mr. Egg and Mr. Pickle
		Won = 6//only for petter pepper and sally salt
	};

	enum class WalkingDirection
	{
		Left,
		Right,
		Up,
		Down
	};

	enum class EnemyState
	{
		Moving,
		Dead,
		Peppered,
		WaitingToMove
	};

	enum class GameMode : size_t
	{
		SinglePlayer = 0,
		Coop = 1,
		Versus = 2,
		MainMenu = 3
	};

	enum class SoundIndices
	{
		EnemyCrushed,
		IngredienDropped,
		LevelWon,
		PepperThrown,
		PlayerDied,
		PlayerHit
	};
}