#pragma once
#include <BaseComponent.h>

namespace dae
{
	class GameObject;

	class HotDog final : public BaseComponent
	{
	public:
		HotDog(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		HotDog(std::nullptr_t) = delete;

		~HotDog() = default;

		HotDog(const HotDog & other) = delete;
		HotDog(HotDog && other) = delete;
		HotDog& operator=(const HotDog & other) = delete;
		HotDog& operator=(HotDog && other) = delete;


	private:
		GameObject* m_pParentObject;
	};
}



