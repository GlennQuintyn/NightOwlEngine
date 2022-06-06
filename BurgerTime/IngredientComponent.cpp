#include "BurgerTimePCH.h"
#include "IngredientComponent.h"
#include <GameObject.h>
#include <RectColliderComponent.h>

#include "WalkingPlatformComponent.h"
#include "PlateComponent.h"
#include "PeterPepper.h"
#include "Enums.h"
#include "MrHotDog.h"
#include "MrEgg.h"
#include "MrPickle.h"

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
	, m_Subject{}
	, m_SpawnPos{}
	, m_FallSpeed{ 150.f }
	//, m_FallSpeed{ 50.f }
	, m_State{ IngridientState::Falling }
	, m_EnemyOnIngredientCount{}
	, m_ColliderLeftHit{ false }
	, m_ColliderCenterHit{ false }
	, m_ColliderRightHit{ false }
	, m_FallExtraLevel{ false }
	, m_HasSettled{ false }
{
	//hardcoded hitboxes structure of the charachter that needs them
	auto& colliderLogicObj = m_pParentObject->CreateAddChild("ColliderLogic");
	auto& colliderLObj = colliderLogicObj.CreateAddChild("colliderLObj");
	m_ColliderLeft = &colliderLObj.AddComponent<RectColliderComponent>();
	m_ColliderLeft->Init({ 15,0,2,10 }, 0, false, { 255, 0, 0, 255 });
	auto& colliderCenterObj = colliderLogicObj.CreateAddChild("colliderCenterObj");
	m_ColliderCenter = &colliderCenterObj.AddComponent<RectColliderComponent>();
	m_ColliderCenter->Init({ 43,0,2,10 }, 1, false, { 0, 255, 0, 255 });
	auto& colliderRObj = colliderLogicObj.CreateAddChild("colliderRObj");
	m_ColliderRight = &colliderRObj.AddComponent<RectColliderComponent>();
	m_ColliderRight->Init({ 72,0,2,10 }, 2, false, { 0, 0, 255, 255 });

	m_pImpl = std::unique_ptr<std::array<HitBoxObserver, 3>>(
		new std::array<HitBoxObserver, 3>{
		HitBoxObserver(m_ColliderLeftHit),
			HitBoxObserver(m_ColliderCenterHit),
			HitBoxObserver(m_ColliderRightHit) }
	);

	//initializing observer system
	//auto& subjectL = colliderLcmpt.GetSubject();
	auto& subjectL = m_ColliderLeft->GetSubject();
	subjectL.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderLeft)));
	auto& subjectCenter = m_ColliderCenter->GetSubject();
	subjectCenter.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderCenter)));
	auto& subjectR = m_ColliderRight->GetSubject();
	subjectR.AddObserver(m_pImpl->at(static_cast<size_t>(ColliderIndices::ColliderRight)));
}

void dae::IngredientComponent::LateInit()
{
	if (auto pCollider = m_pParentObject->GetComponent<RectColliderComponent>())
	{
		int sceneId = pCollider->GetSceneId();

		m_ColliderLeft->SetSceneId(sceneId);
		m_ColliderCenter->SetSceneId(sceneId);
		m_ColliderRight->SetSceneId(sceneId);
	}
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
			if (posCollider.y - pCollider->GetRectangle().h * 4.f <= pos.y)
			{
				//when the ingredients spawn in or get reset they first fall into their starting platform,
				//this falsely triggers the Item_Fell event, after the first time they have settled
				if (m_HasSettled)
					m_Subject.Notify(m_pParentObject, static_cast<int>(Events::Item_Fell));
				else
					m_HasSettled = true;

				m_EnemyOnIngredientCount = 0;//reset counter each time it falls on a platform
				m_pPlatformObject = nullptr;


				//if it needs to fall an extra leve it should just pass by the first platform it hits
				if (!m_FallExtraLevel)
				{
					m_pParentObject->SetLocalPosition(pos.x, posCollider.y - pCollider->GetRectangle().h * 4.f);
					m_ColliderLeftHit = false;
					m_ColliderCenterHit = false;
					m_ColliderRightHit = false;
					m_State = IngridientState::OnPlatform;
				}
				else
				{
					//m_State
					m_FallExtraLevel = false;
				}
			}
		}
		break;
	}
	case dae::IngredientComponent::IngridientState::OnPlatform:
	{
		if (m_ColliderLeftHit && m_ColliderCenterHit && m_ColliderRightHit)
		{
			//when it goes into the falling state,
			//check how many enemies are on it and then send correct point event
			switch (m_EnemyOnIngredientCount)
			{
			case 1:
				m_Subject.Notify(m_pParentObject, static_cast<int>(Events::Drop_Enemy_1));
				break;
			case 2:
				m_Subject.Notify(m_pParentObject, static_cast<int>(Events::Drop_Enemy_2));
				break;
			case 3:
				m_Subject.Notify(m_pParentObject, static_cast<int>(Events::Drop_Enemy_3));
				break;
			case 4:
				m_Subject.Notify(m_pParentObject, static_cast<int>(Events::Drop_Enemy_4));
				break;
			case 5:
				m_Subject.Notify(m_pParentObject, static_cast<int>(Events::Drop_Enemy_5));
				break;
			case 6:
				m_Subject.Notify(m_pParentObject, static_cast<int>(Events::Drop_Enemy_6));
				break;
			}

			if (m_EnemyOnIngredientCount > 0)
			{
				m_FallExtraLevel = true;
			}

			m_State = IngridientState::Falling;
		}
		break;
	}
	}
}

void dae::IngredientComponent::Reset()
{
	m_HasSettled = false;
	m_EnemyOnIngredientCount = 0;
	m_State = IngridientState::Falling;
	m_pParentObject->SetLocalPosition(m_SpawnPos);

	m_ColliderLeftHit = false;
	m_ColliderCenterHit = false;
	m_ColliderRightHit = false;
}

void dae::IngredientComponent::Notify(GameObject* pObject, int event)
{
	if (event == 0)
	{
		if (auto pPlatform = pObject->GetComponent<WalkingPlatformComponent>())
		{
			m_pPlatformObject = pObject;
		}
		//for when it hits another ingriedient (normall ingredient falls ontop of it) hits it it should start falling
		else if (pObject->GetComponent<IngredientComponent>())
		{
			if (m_State != IngridientState::OnPlate)
			{
				if (m_FallExtraLevel)
				{
					m_State = IngridientState::Wait;
				}
				else
				{
					m_State = IngridientState::Falling;
				}
			}
		}
		else if (pObject->GetComponent<PlateComponent>())
		{
			m_State = IngridientState::OnPlate;
			//falling on a plate also counts as giving points
			m_Subject.Notify(m_pParentObject, static_cast<int>(Events::Item_Fell));
			//reset counter when it falls on a plate
			m_EnemyOnIngredientCount = 0;
		}
		//if it is one of the enemies enters increase the counter
		else if (pObject->GetComponent<MrHotDog>() || pObject->GetComponent<MrEgg>() || pObject->GetComponent<MrPickle>())
		{
			++m_EnemyOnIngredientCount;
		}
	}
	else if (event == 1)
	{
		//if it is one of the enemies leavesd decrease the counter
		if (pObject->GetComponent<MrHotDog>() || pObject->GetComponent<MrEgg>() || pObject->GetComponent<MrPickle>())
		{
			--m_EnemyOnIngredientCount;
		}
		else if (m_State == IngridientState::Wait && pObject->GetComponent<IngredientComponent>())
		{
			m_State = IngridientState::Falling;
		}
	}
}

void dae::IngredientComponent::SetSpawnLocation(float x, float y)
{
	m_SpawnPos.x = x;
	m_SpawnPos.y = y;
}

bool dae::IngredientComponent::IsFalling() const
{
	return m_State == IngridientState::Falling;
}
