#include "BurgerTimePCH.h"
#include "MovementComponent.h"
#include "Observer.h"
#include "GameObject.h"
#include "LadderComponent.h"
#include "WalkingPlatformComponent.h"

namespace dae
{
	enum class ColliderIndices
	{
		ColliderLeft = 0,
		ColliderRight = 1,
		ColliderUp = 2,
		ColliderDown = 3
	};

	class MovementComponent::HitBoxObserver final : public dae::Observer
	{
	public:
		HitBoxObserver(bool isLadderObserver, bool& direction)
			: m_IsLadderObserver{ isLadderObserver }
			, m_CanGoDirection{ direction }
			, m_pLastTouchedColliderObj{ nullptr }
		{}

		~HitBoxObserver() = default;

		void Notify(GameObject* pObject, int event) override
		{
			if (m_IsLadderObserver)
			{
				if (auto pLaddercmp = pObject->GetComponent<LadderComponent>())
				{
					// if the event is a 0 that means the collider entered a ladder and if its 0 it exited one
					switch (event)
					{
					case 0:
						m_CanGoDirection = true;
						m_pLastTouchedColliderObj = pObject;
						break;
					case 1:
						m_CanGoDirection = false;
						m_pLastTouchedColliderObj = nullptr;
						break;
					default:
						Logger::GetInstance().LogError("MOVEMENTCOMPONENT: invalid switch state reached!");
						break;
					}
				}
			}
			else
			{
				if (auto pPlatformcmp = pObject->GetComponent<WalkingPlatformComponent>())
				{
					// if the event is a 0 that means the collider entered a platform and if its 0 it exited one
					switch (event)
					{
					case 0:
						m_CanGoDirection = true;
						m_pLastTouchedColliderObj = pObject;
						break;
					case 1:
						m_CanGoDirection = false;
						m_pLastTouchedColliderObj = nullptr;
						break;
					default:
						Logger::GetInstance().LogError("MOVEMENTCOMPONENT: invalid switch state reached!");
						break;
					}
				}
			}
		}

		GameObject* GetLastTouchedColliderObj()
		{
			return m_pLastTouchedColliderObj;
		};

	private:
		const bool m_IsLadderObserver;
		bool& m_CanGoDirection;
		GameObject* m_pLastTouchedColliderObj;
	};
}

dae::MovementComponent::MovementComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_LastDirection{}
	, m_ColliderLeft{ nullptr }
	, m_ColliderRight{ nullptr }
	, m_ColliderUp{ nullptr }
	, m_ColliderDown{ nullptr }
	, m_CanGoLeft{ false }
	, m_CanGoRight{ false }
	, m_CanGoUp{ false }
	, m_CanGoDown{ false }
	, m_Enabled{ false }//start out false by default so that object doesn't move when scene isn't active (has to be set false at end of scene and true lateinit of scene)
{
	//hardcoded hitboxes structure of the charachter that needs them
	auto& colliderLogicObj = m_pParentObject->CreateAddChild("ColliderLogic");
	auto& colliderLObj = colliderLogicObj.CreateAddChild("Lcollider");
	m_ColliderLeft = &colliderLObj.AddComponent<RectColliderComponent>();
	m_ColliderLeft->Init({ -3,42,3,3 }, -1, false, { 255, 0, 0, 128 });

	auto& colliderRObj = colliderLogicObj.CreateAddChild("Rcollider");
	m_ColliderRight = &colliderRObj.AddComponent<RectColliderComponent>();
	m_ColliderRight->Init({ 47,42,3,3 }, -1, false, { 0, 255, 0, 128 });

	auto& colliderUPObj = colliderLogicObj.CreateAddChild("UPcollider");
	m_ColliderUp = &colliderUPObj.AddComponent<RectColliderComponent>();
	m_ColliderUp->Init({ 20,/*-3*/27,5,3 }, -1, false, { 0, 0, 255, 128 });

	auto& colliderDOWNObj = colliderLogicObj.CreateAddChild("DOWNcollider");
	m_ColliderDown = &colliderDOWNObj.AddComponent<RectColliderComponent>();
	m_ColliderDown->Init({ 20,48,5,3 }, -1, false, { 0, 255, 255, 128 });

	m_pImpl = std::unique_ptr<std::array<HitBoxObserver, 4>>(
		new std::array<HitBoxObserver, 4>{
		HitBoxObserver(false, m_CanGoLeft),
			HitBoxObserver(false, m_CanGoRight),
			HitBoxObserver(true, m_CanGoUp),
			HitBoxObserver(true, m_CanGoDown) }
	);

	//initializing observer system
	//auto& subjectL = colliderLcmpt.GetSubject();
	auto& subjectL = m_ColliderLeft->GetSubject();
	subjectL.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderLeft)));
	//auto& subjectR = colliderRcmpt.GetSubject();
	auto& subjectR = m_ColliderRight->GetSubject();
	subjectR.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderRight)));
	//auto& subjectUp = colliderUPcmpt.GetSubject();
	auto& subjectUp = m_ColliderUp->GetSubject();
	subjectUp.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderUp)));
	//auto& subjectDown = colliderDowncmpt.GetSubject();
	auto& subjectDown = m_ColliderDown->GetSubject();
	subjectDown.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderDown)));
}

dae::MovementComponent::~MovementComponent()
{
}

dae::GameObject* dae::MovementComponent::GetTouchingPlatformLeft()
{
	if (m_Enabled)
	{
		m_LastDirection = WalkingDirection::Left;
		return m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderLeft)).GetLastTouchedColliderObj();
	}
	else
		return nullptr;
}

dae::GameObject* dae::MovementComponent::GetTouchingPlatformRight()
{
	if (m_Enabled)
	{
		m_LastDirection = WalkingDirection::Right;
		return m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderRight)).GetLastTouchedColliderObj();
	}
	else
		return nullptr;
}

dae::GameObject* dae::MovementComponent::GetTouchingLadderUp()
{
	if (m_Enabled)
	{
		m_LastDirection = WalkingDirection::Up;
		return m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderUp)).GetLastTouchedColliderObj();
	}
	else
		return nullptr;
}

dae::GameObject* dae::MovementComponent::GetTouchingLadderDown()
{
	if (m_Enabled)
	{
		m_LastDirection = WalkingDirection::Down;
		return m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderDown)).GetLastTouchedColliderObj();
	}
	else
		return nullptr;
}

void dae::MovementComponent::LateInit()
{
	if (auto pCollider = m_pParentObject->GetComponent<RectColliderComponent>())
	{
		int sceneId = pCollider->GetSceneId();

		m_ColliderLeft->SetSceneId(sceneId);
		m_ColliderRight->SetSceneId(sceneId);
		m_ColliderUp->SetSceneId(sceneId);
		m_ColliderDown->SetSceneId(sceneId);
	}
}