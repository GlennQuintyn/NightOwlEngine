#pragma once
#include "BaseComponent.h"
#include "PCController.h"

namespace dae
{
	class GameObject;
	class PeterPepper final : public BaseComponent
	{
	public:
		PeterPepper(GameObject* pParentObject);
		~PeterPepper() = default;

		//make sure that component has a parent that isn't nullptr
		PeterPepper(std::nullptr_t) = delete;

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override {};

		void SetDeathButton(PCController::ControllerButton deahtButton);

		void SetObjectFellButton(PCController::ControllerButton objFellButton);
		void SetPepperEnemyButton(PCController::ControllerButton enemyPepperedButton);

		PeterPepper(const PeterPepper& other) = delete;
		PeterPepper(PeterPepper&& other) = delete;
		PeterPepper& operator=(const PeterPepper& other) = delete;
		PeterPepper& operator=(PeterPepper&& other) = delete;
	private:
		GameObject* m_pParentObject;
		PCController::ControllerButton m_DeahtButton;
		PCController::ControllerButton m_ObjFellButton;
		PCController::ControllerButton m_EnemyPepperedButton;
	};
}




