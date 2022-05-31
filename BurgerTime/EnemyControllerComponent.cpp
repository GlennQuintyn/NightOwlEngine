#include "BurgerTimePCH.h"
#include "EnemyControllerComponent.h"
#include "GameObject.h"
#include "MovementComponent.h"
#include "RectColliderComponent.h"
#include "SpriteManagerComponent.h"
#include "Enums.h"

const float g_PixelEpsilon{ 1.5f };

dae::EnemyControllerComponent::EnemyControllerComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_pMovementCmpt{ nullptr }
	, m_pSpriteManagerCmpt{ nullptr }
	, m_pPlayer1{ nullptr }
	, m_pPlayer2{ nullptr }
	, m_State{ MovementState::Horizontal }
	, m_MovmentSpeed{ 75.f, 40.f }//x: horizontal, y: vertical
{
}

void dae::EnemyControllerComponent::LateInit()
{
	if (!m_pPlayer1)
	{
		Logger::GetInstance().LogError("EnemyControllerComponent: No player was given!");
	}

	m_pMovementCmpt = m_pParentObject->GetComponent<MovementComponent>();
	if (!m_pMovementCmpt)
	{
		Logger::GetInstance().LogError("EnemyControllerComponent: No MovementComponent was found!");
	}

	m_pSpriteManagerCmpt = m_pParentObject->GetComponent<SpriteManagerComponent>();
	if (!m_pSpriteManagerCmpt)
	{
		Logger::GetInstance().LogError("EnemyControllerComponent: No SpriteManagerComponent was found!");
	}

	auto pCollider = m_pParentObject->GetComponent<RectColliderComponent>();
	if (!pCollider)
	{
		Logger::GetInstance().LogError("EnemyControllerComponent: No RectColliderComponent was found!");
		return;
	}

	m_Size.x = pCollider->GetRectangle().w;
	m_Size.y = pCollider->GetRectangle().h;
}

void dae::EnemyControllerComponent::Update()
{
	//TODO: check player count and goto closest
	//if on lader keep direction
	//check if player is above, below, left or right

	//switch (m_State)
	//{
	//case dae::EnemyControllerComponent::MovementState::Horizontal:
	//	Logger::GetInstance().LogInfo("Horizontal");
	//	break;
	//case dae::EnemyControllerComponent::MovementState::UpLadder:
	//	Logger::GetInstance().LogInfo("UpLadder");
	//	break;
	//case dae::EnemyControllerComponent::MovementState::DownLadder:
	//	Logger::GetInstance().LogInfo("DownLadder");
	//	break;
	//case dae::EnemyControllerComponent::MovementState::NoMovement:
	//	Logger::GetInstance().LogInfo("NoMovement");
	//	break;
	//default:
	//	break;
	//}

	//no Movement AI when dead)
	if (m_State == MovementState::NoMovement)
		return;

	auto& pos = m_pParentObject->GetWorldPosition();

	//first check if there is a platform left or right to break the continous ladder movement
	if (m_pMovementCmpt->GetTouchingPlatformLeft() || m_pMovementCmpt->GetTouchingPlatformRight())
	{
		Logger::GetInstance().LogInfo("Horizontal");
		m_State = MovementState::Horizontal;
	}
	//if the enemy is still going up/down a ladder then they should keep going that direction until they reached the end of the ladder.
	if (m_State == MovementState::UpLadder)
	{
		if (m_pMovementCmpt->CanGoUp())
			MoveUp(pos);
		else
			m_State = MovementState::Horizontal;
		return;
	}
	else if (m_State == MovementState::DownLadder)
	{
		if (m_pMovementCmpt->CanGoDown())
			MoveDown(pos);
		else
			m_State = MovementState::Horizontal;
		return;
	}

	auto& posPlayer1 = m_pPlayer1->GetWorldPosition();
	glm::vec2 goal;

	//check which player is closer (if there is a 2nd player to begin with)
	if (m_pPlayer2)
	{
		auto& posPlayer2 = m_pPlayer1->GetWorldPosition();

		if (glm::distance(pos, posPlayer1) <= glm::distance(pos, posPlayer2))
			goal = posPlayer1;
		else
			goal = posPlayer2;
	}
	else
		goal = posPlayer1;

	//if (std::abs(goal.y - pos.y) <= std::abs(goal.x - pos.x))
	//{
	//}

	//go up if possible
	if (std::abs(goal.y - pos.y) >= g_PixelEpsilon && goal.y <= pos.y && m_pMovementCmpt->CanGoUp())
		//if (goal.y <= pos.y && m_pMovementCmpt->CanGoUp())
	{
		MoveUp(pos);
		m_State = MovementState::UpLadder;
	}
	//go down if possible
	else if (std::abs(goal.y - pos.y) >= g_PixelEpsilon && goal.y >= pos.y && m_pMovementCmpt->CanGoDown())
		//else if (goal.y >= pos.y && m_pMovementCmpt->CanGoDown())
	{
		MoveDown(pos);
		m_State = MovementState::DownLadder;
	}
	//go left
	else if (m_State == MovementState::Horizontal && goal.x < pos.x && m_pMovementCmpt->CanGoLeft())
	{
		MoveLeft(pos);
	}
	//go right
	else if (m_State == MovementState::Horizontal && goal.x > pos.x && m_pMovementCmpt->CanGoRight())
	{
		MoveRigth(pos);
	}
}

void dae::EnemyControllerComponent::MoveLeft(const glm::vec2& pos)
{
	auto pPlatform = m_pMovementCmpt->GetTouchingPlatformLeft();
	if (!pPlatform)
		return;

	auto& PlatformPos = pPlatform->GetWorldPosition();
	auto pPlarformCollider = pPlatform->GetComponent<RectColliderComponent>();

	m_pParentObject->SetLocalPosition(pos.x - (m_MovmentSpeed.x * Time::GetInstance().GetDeltaT()), PlatformPos.y + (pPlarformCollider->GetRectangle().h) - m_Size.y);

	if (m_pSpriteManagerCmpt)
		m_pSpriteManagerCmpt->PlaySprite(static_cast<uint32_t>(SpriteIndices::WalkLeft));
}

void dae::EnemyControllerComponent::MoveRigth(const glm::vec2& pos)
{
	auto pPlatform = m_pMovementCmpt->GetTouchingPlatformRight();
	if (!pPlatform)
		return;

	auto& PlatformPos = pPlatform->GetWorldPosition();
	auto pPlarformCollider = pPlatform->GetComponent<RectColliderComponent>();

	m_pParentObject->SetLocalPosition(pos.x + (m_MovmentSpeed.x * Time::GetInstance().GetDeltaT()), PlatformPos.y + (pPlarformCollider->GetRectangle().h) - m_Size.y);

	if (m_pSpriteManagerCmpt)
		m_pSpriteManagerCmpt->PlaySprite(static_cast<uint32_t>(SpriteIndices::WalkRight));

}

void dae::EnemyControllerComponent::MoveUp(const glm::vec2& pos)
{
	auto pLadder = m_pMovementCmpt->GetTouchingLadderUp();
	if (!pLadder)
		return;

	auto& ladderPos = pLadder->GetWorldPosition();
	auto pLadderCollider = pLadder->GetComponent<RectColliderComponent>();

	m_pParentObject->SetLocalPosition(ladderPos.x + (pLadderCollider->GetRectangle().w / 2.f) - (m_Size.x / 2.f), pos.y - (m_MovmentSpeed.y * Time::GetInstance().GetDeltaT()));

	if (m_pSpriteManagerCmpt)
		m_pSpriteManagerCmpt->PlaySprite(static_cast<uint32_t>(SpriteIndices::WalkUp));
}

void dae::EnemyControllerComponent::MoveDown(const glm::vec2& pos)
{
	auto pLadder = m_pMovementCmpt->GetTouchingLadderDown();
	if (!pLadder)
		return;

	auto& ladderPos = pLadder->GetWorldPosition();
	auto pLadderCollider = pLadder->GetComponent<RectColliderComponent>();

	m_pParentObject->SetLocalPosition(ladderPos.x + (pLadderCollider->GetRectangle().w / 2.f) - (m_Size.x / 2.f), pos.y + (m_MovmentSpeed.y * Time::GetInstance().GetDeltaT()));

	if (m_pSpriteManagerCmpt)
		m_pSpriteManagerCmpt->PlaySprite(static_cast<uint32_t>(SpriteIndices::WalkDown));
}
