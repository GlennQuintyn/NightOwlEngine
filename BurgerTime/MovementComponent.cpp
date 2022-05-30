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
					m_pLastTouchedColliderObj = pObject;

					// if the event is a 0 that means the collider entered a ladder and if its 0 it exited one
					switch (event)
					{
					case 0:
						m_CanGoDirection = true;
						break;
					case 1:
						m_CanGoDirection = false;
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
					m_pLastTouchedColliderObj = pObject;

					// if the event is a 0 that means the collider entered a platform and if its 0 it exited one
					switch (event)
					{
					case 0:
						m_CanGoDirection = true;
						break;
					case 1:
						m_CanGoDirection = false;
						break;
					default:
						Logger::GetInstance().LogError("MOVEMENTCOMPONENT: invalid switch state reached!");
						break;
					}
				}
			}
		}

		GameObject* GetLastTouchedColliderObj() { return m_pLastTouchedColliderObj; };

	private:
		const bool m_IsLadderObserver;
		bool& m_CanGoDirection;
		GameObject* m_pLastTouchedColliderObj;
	};
}


dae::MovementComponent::MovementComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_CanGoLeft{ false }
	, m_CanGoRight{ false }
	, m_CanGoUp{ false }
	, m_CanGoDown{ false }
	, m_Enabled{ true }
{
	//hardcoded hitboxes structure of the charachter that needs them
	auto& colliderLogicObj = m_pParentObject->CreateAddChild("ColliderLogic");
	auto& colliderLObj = colliderLogicObj.CreateAddChild("Lcollider");
	auto& colliderLcmpt = colliderLObj.AddComponent<RectColliderComponent>();
	colliderLcmpt.Init({ -3,42,3,3 }, 0, true, { 255, 0, 0, 128 });
	auto& colliderRObj = colliderLogicObj.CreateAddChild("Rcollider");
	auto& colliderRcmpt = colliderRObj.AddComponent<RectColliderComponent>();
	colliderRcmpt.Init({ 47,42,3,3 }, 1, true, { 0, 255, 0, 128 });
	auto& colliderUPObj = colliderLogicObj.CreateAddChild("UPcollider");
	auto& colliderUPcmpt = colliderUPObj.AddComponent<RectColliderComponent>();
	colliderUPcmpt.Init({ 21,-3,3,3 }, 2, true, { 0, 0, 255, 128 });
	auto& colliderDOWNObj = colliderLogicObj.CreateAddChild("DOWNcollider");
	auto& colliderDowncmpt = colliderDOWNObj.AddComponent<RectColliderComponent>();
	colliderDowncmpt.Init({ 21,48,3,3 }, 3, true, { 0, 255, 255, 128 });

	m_pImpl = std::unique_ptr<std::array<HitBoxObserver, 4>>(
		new std::array<HitBoxObserver, 4>{
		HitBoxObserver(false, m_CanGoLeft),
			HitBoxObserver(false, m_CanGoRight),
			HitBoxObserver(true, m_CanGoUp),
			HitBoxObserver(true, m_CanGoDown) }
	);

	//initializing observer system
	auto& subjectL = colliderLcmpt.GetSubject();
	subjectL.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderLeft)));
	auto& subjectR = colliderRcmpt.GetSubject();
	subjectR.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderRight)));
	auto& subjectUp = colliderUPcmpt.GetSubject();
	subjectUp.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderUp)));
	auto& subjectDown = colliderDowncmpt.GetSubject();
	subjectDown.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderDown)));
}

dae::MovementComponent::~MovementComponent()
{
}

dae::GameObject* dae::MovementComponent::GetTouchingPlatformLeft()
{
	if (m_Enabled)
		return m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderLeft)).GetLastTouchedColliderObj();
	else
		return nullptr;
}

dae::GameObject* dae::MovementComponent::GetTouchingPlatformRight()
{
	if (m_Enabled)
		return m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderRight)).GetLastTouchedColliderObj();
	else
		return nullptr;
}

dae::GameObject* dae::MovementComponent::GetTouchingLadderUp()
{
	if (m_Enabled)
		return m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderUp)).GetLastTouchedColliderObj();
	else
		return nullptr;
}

dae::GameObject* dae::MovementComponent::GetTouchingLadderDown()
{
	if (m_Enabled)
		return m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderDown)).GetLastTouchedColliderObj();
	else
		return nullptr;
}