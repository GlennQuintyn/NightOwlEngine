#include "BurgerTimePCH.h"
#include "IngredientComponent.h"
#include "GameObject.h"
#include "WalkingPlatformComponent.h"
#include "PlateComponent.h"
#include "RectColliderComponent.h"
#include "PeterPepper.h"

namespace dae
{
	enum class ColliderIndices
	{
		ColliderLeft = 0,
		ColliderCenter = 1,
		ColliderRight = 2,
	};

	class IngredientComponent::HitBoxObserver final : public dae::Observer
	{
	public:
		HitBoxObserver(bool& hitState)
			: m_HasBeenHit{ hitState }
		{}

		~HitBoxObserver() = default;

		void Notify(GameObject* pObject, int event) override
		{
			if (auto pPetercmp = pObject->GetComponent<PeterPepper>())
			{
				// if the event is a 0 that means the collider entered a ladder and if its 0 it exited one
				if (event == 0)
					m_HasBeenHit = true;
			}
		}
	private:
		bool& m_HasBeenHit;
	};
}

dae::IngredientComponent::IngredientComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_pPlatformObject{ nullptr }
	, m_FallSpeed{ 150.f }
	, m_State{ IngridientState::Falling }
{
	//hardcoded hitboxes structure of the charachter that needs them
	auto& colliderLogicObj = m_pParentObject->CreateAddChild("ColliderLogic");
	auto& colliderLObj = colliderLogicObj.CreateAddChild("colliderLObj");
	auto& colliderLcmpt = colliderLObj.AddComponent<RectColliderComponent>();
	colliderLcmpt.Init({ 15,0,2,22 }, 0, true, { 255, 0, 0, 255 });
	auto& colliderCenterObj = colliderLogicObj.CreateAddChild("colliderCenterObj");
	auto& colliderCentercmpt = colliderCenterObj.AddComponent<RectColliderComponent>();
	colliderCentercmpt.Init({ 43,0,2,22 }, 1, true, { 0, 255, 0, 255 });
	auto& colliderRObj = colliderLogicObj.CreateAddChild("colliderRObj");
	auto& colliderRcmpt = colliderRObj.AddComponent<RectColliderComponent>();
	colliderRcmpt.Init({ 72,0,2,22 }, 2, true, { 0, 0, 255, 255 });

	m_pImpl = std::unique_ptr<std::array<HitBoxObserver, 3>>(
		new std::array<HitBoxObserver, 3>{
		HitBoxObserver(m_ColliderLeftHit),
			HitBoxObserver(m_ColliderCenterHit),
			HitBoxObserver(m_ColliderRightHit) }
	);

	//initializing observer system
	auto& subjectL = colliderLcmpt.GetSubject();
	subjectL.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderLeft)));
	auto& subjectCenter = colliderCentercmpt.GetSubject();
	subjectCenter.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderCenter)));
	auto& subjectR = colliderRcmpt.GetSubject();
	subjectR.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderRight)));
}

void dae::IngredientComponent::Update()
{
	switch (m_State)
	{
	case dae::IngredientComponent::IngridientState::Falling:
	{
		auto& pos = m_pParentObject->GetWorldPosition();

		m_pParentObject->SetLocalPosition(pos.x, pos.y + (m_FallSpeed * Time::GetInstance().GetDeltaT()));

		if (m_pPlatformObject)
		{
			//auto& pos = m_pParentObject->GetWorldPosition();
			auto& posCollider = m_pPlatformObject->GetWorldPosition();
			auto pCollider = m_pPlatformObject->GetComponent<RectColliderComponent>();

			//if the ingredient is below the the platform it should stop falling and loc into the correct position
			if (posCollider.y - pCollider->GetRectangle().h * 2.f <= pos.y)
			{
				m_pParentObject->SetLocalPosition(pos.x, posCollider.y - pCollider->GetRectangle().h * 2.f);

				m_pPlatformObject = nullptr;
				m_State = IngridientState::OnPlatform;
				m_ColliderLeftHit = false;
				m_ColliderCenterHit = false;
				m_ColliderRightHit = false;
			}
		}
		break;
	}
	case dae::IngredientComponent::IngridientState::OnPlatform:
	{
		if (m_ColliderLeftHit && m_ColliderCenterHit && m_ColliderRightHit)
			m_State = IngridientState::Falling;
		break;
	}
	default:
		break;
	}
}

void dae::IngredientComponent::Notify(GameObject* pObject, int event)
{
	if (auto pPlatform = pObject->GetComponent<WalkingPlatformComponent>())
	{
		if (event == 0)
			m_pPlatformObject = pObject;
	}
	else if (pObject->GetComponent<IngredientComponent>())
	{
		if (event == 0 && m_State != IngridientState::OnPlate)
			m_State = IngridientState::Falling;
	}
	else if (pObject->GetComponent<PlateComponent>())
	{
		if (event == 0)
			m_State = IngridientState::OnPlate;
	}
}

bool dae::IngredientComponent::IsFalling() const
{
	return m_State == IngridientState::Falling;
}
