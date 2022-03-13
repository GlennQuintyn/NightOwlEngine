#pragma once
//#include <XInput.h>
#include "Singleton.h"

#include "PCController.h"
#include <vector>
#include <memory>
#include <map>

namespace dae
{
	/*enum class ControllerButton
	{
		ButtonA,
		ButtonB,
		ButtonX,
		ButtonY
	};*/

	/*class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();
		bool IsPressed(ControllerButton button) const;
	private:
		XINPUT_STATE m_CurrentState{};
	};*/

	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();

		bool ProcessInput();
		bool IsPressed(PCController::ControllerButton controllerButton, size_t contollerIndex = 0) const;
		bool IsPressedThisFrame(PCController::ControllerButton controllerButton, size_t contollerIndex = 0) const;
		bool IsReleasedThisFrame(PCController::ControllerButton controllerButton, size_t contollerIndex = 0) const;

		void AddController();

		template<typename CommandType>
		CommandType& AddCommand(PCController::ControllerButton controllerButton, PCController::ButtonPressState pressState);
	private:
		struct ControllerButtonType
		{
			PCController::ControllerButton button;
			PCController::ButtonPressState pressState;

			std::strong_ordering operator<=>(ControllerButtonType other) const
			{
				return other.button <=> button;
			}
		};

		std::map<ControllerButtonType, std::unique_ptr<Command>> m_MapOfCommands;
		std::vector<std::unique_ptr<PCController>> m_pPCControllers;
	};

	template<typename CommandType>
	inline CommandType& InputManager::AddCommand(PCController::ControllerButton controllerButton, PCController::ButtonPressState pressState)
	{
		auto pCommand = new CommandType{};
		m_MapOfCommands[{controllerButton, pressState}] = std::move(std::unique_ptr<Command>(pCommand));
		return *pCommand;
	}
}