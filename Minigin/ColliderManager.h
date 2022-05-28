#pragma once
#include "Singleton.h"
#include "RectColliderComponent.h"

namespace dae
{
	class ColliderManager final : public Singleton<ColliderManager>
	{
	public:
		~ColliderManager() = default;

		void RegisterRectCollider(RectColliderComponent* pCollider);

		void Update();

		ColliderManager(const ColliderManager& other) = delete;
		ColliderManager(ColliderManager&& other) = delete;
		ColliderManager& operator=(const ColliderManager& other) = delete;
		ColliderManager& operator=(ColliderManager&& other) = delete;

	private:
		friend class Singleton<ColliderManager>;
		ColliderManager() = default;

		std::vector<RectColliderComponent*> m_pRectColliderComponents;
	};
}