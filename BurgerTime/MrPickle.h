#pragma once
#include <BaseComponent.h>
#include "Observer.h"

namespace dae
{
	class GameObject;

	class MrPickle final : public BaseComponent, public Observer
	{
	public:
		MrPickle(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		MrPickle(std::nullptr_t) = delete;

		~MrPickle() = default;

		void Notify(GameObject* pObject, int event) override;

		void LateInit() override {};
		void Update() override {};
		void LateUpdate() override {};
		void Render() const override {};
		void Reset() override {};

		MrPickle(const MrPickle& other) = delete;
		MrPickle(MrPickle&& other) = delete;
		MrPickle& operator=(const MrPickle& other) = delete;
		MrPickle& operator=(MrPickle&& other) = delete;


	private:
		GameObject* m_pParentObject;
	};
}