#pragma once
#include <BaseComponent.h>
#include "Observer.h"

namespace dae
{
	class GameObject;

	class MrEgg final : public BaseComponent, public Observer
	{
	public:
		MrEgg(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		MrEgg(std::nullptr_t) = delete;

		~MrEgg() = default;

		void Notify(GameObject* pObject, int event) override;

		void LateInit() override {};
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override {};

		MrEgg(const MrEgg& other) = delete;
		MrEgg(MrEgg&& other) = delete;
		MrEgg& operator=(const MrEgg& other) = delete;
		MrEgg& operator=(MrEgg&& other) = delete;


	private:
		GameObject* m_pParentObject;
	};
}
