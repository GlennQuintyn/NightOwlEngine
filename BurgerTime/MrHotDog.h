#pragma once
#include <BaseComponent.h>
#include "Observer.h"

namespace dae
{
	class GameObject;

	class MrHotDog final : public BaseComponent, public Observer
	{
	public:
		MrHotDog(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		MrHotDog(std::nullptr_t) = delete;

		~MrHotDog() = default;

		void Notify(GameObject* pObject, int event) override;

		void LateInit() override {};
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override {};

		MrHotDog(const MrHotDog & other) = delete;
		MrHotDog(MrHotDog && other) = delete;
		MrHotDog& operator=(const MrHotDog & other) = delete;
		MrHotDog& operator=(MrHotDog && other) = delete;


	private:
		GameObject* m_pParentObject;
	};
}
