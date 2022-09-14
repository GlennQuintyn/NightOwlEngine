#pragma once
#include "Singleton.h"

#include "PCController.h"
#include "NoeKeyboard.h"
#include "NoeMouse.h"
#include "Command.h"

#include <vector>
#include <memory>
#include <map>

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class ButtonPressState
		{
			PressedContinuous,
			OnPressed,
			Released
		};

		InputManager();
		~InputManager() = default;

		bool ProcessInput();

		bool IsPressed(PCController::ControllerButton controllerButton, size_t contollerIndex = 0) const;
		bool IsPressedThisFrame(PCController::ControllerButton controllerButton, size_t contollerIndex = 0) const;
		bool IsReleasedThisFrame(PCController::ControllerButton controllerButton, size_t contollerIndex = 0) const;

		bool IsPressed(NoeKeyboard::KeyboardKey keyboardButton) const;
		bool IsPressedThisFrame(NoeKeyboard::KeyboardKey keyboardButton) const;
		bool IsReleasedThisFrame(NoeKeyboard::KeyboardKey keyboardButton) const;

		//uses a bit mask, if you want to test for more mouse buttons just continue the mask and cast to MouseButtons
		bool IsPressed(int mouseButton) const;
		//uses a bit mask, if you want to test for more mouse buttons just continue the mask and cast to MouseButtons
		bool IsPressedThisFrame(int mouseButton) const;
		//uses a bit mask, if you want to test for more mouse buttons just continue the mask and cast to MouseButtons
		bool IsReleasedThisFrame(int mouseButton) const;

		const glm::ivec2& GetMousePosition() { return m_pMouse->GetMousePosition(); };

		//TODO: change add controlle to works automatically aka detect if controller gets added or removed and only itterate over the ones that are still there
		//adds a new controller up to 4, when the input manager gets constructed a controller gets added automatically
		void AddController();

		void CheckConnectedControllers();

		//TODO: Add RemoveCommands function to remove or even disable certain commands

		template<typename CommandType>
		CommandType& AddCommand(PCController::ControllerButton controllerButton, ButtonPressState pressState, int controllerIndex = 0);

		template<typename CommandType>
		CommandType& AddCommand(NoeKeyboard::KeyboardKey keyboardKey, ButtonPressState pressState);

		//because not all mouses have the same buttons the enum is limited to the most used ones, you need to static_cast<int>() your MouseButtons or give the index of the button you want to add as a command
		template<typename CommandType>
		CommandType& AddCommand(int mouseButton, ButtonPressState pressState);


	private:
		struct ControllerButtonType
		{
			PCController::ControllerButton button;
			ButtonPressState pressState;
			int index;
		};

		struct KeyboardKeyType
		{
			NoeKeyboard::KeyboardKey key;
			ButtonPressState pressState;
		};

		struct MouseButtonType
		{
			int button;
			ButtonPressState pressState;
		};

		std::vector<std::pair<ControllerButtonType, std::unique_ptr<Command>>> m_MapOfControllerCommands;
		std::vector<std::pair<KeyboardKeyType, std::unique_ptr<Command>>> m_MapOfKeyboardCommands;
		std::vector<std::pair<MouseButtonType, std::unique_ptr<Command>>> m_MapOfMouseCommands;

		std::vector<std::unique_ptr<PCController>> m_pPCControllers;
		std::unique_ptr<NoeKeyboard> m_pKeyboard;
		std::unique_ptr<NoeMouse> m_pMouse;
	};

	//default controller index 0
	template<typename CommandType>
	inline CommandType& InputManager::AddCommand(PCController::ControllerButton controllerButton, ButtonPressState pressState, int controllerIndex)
	{
		auto pCommand = new CommandType{};
		//m_MapOfControllerCommands[{controllerButton, pressState}] = std::move(std::unique_ptr<Command>(pCommand));
		m_MapOfControllerCommands.emplace_back(std::make_pair(ControllerButtonType(controllerButton, pressState, controllerIndex), std::move(std::unique_ptr<Command>(pCommand))));
		return *pCommand;
	}

	template<typename CommandType>
	inline CommandType& InputManager::AddCommand(NoeKeyboard::KeyboardKey keyboardKey, ButtonPressState pressState)
	{
		auto pCommand = new CommandType{};
		//m_MapOfKeyboardCommands[{keyboardKey, pressState}] = std::move(std::unique_ptr<Command>(pCommand));
		m_MapOfKeyboardCommands.emplace_back(std::make_pair(KeyboardKeyType(keyboardKey, pressState), std::move(std::unique_ptr<Command>(pCommand))));
		return *pCommand;
	}

	template<typename CommandType>
	inline CommandType& InputManager::AddCommand(int mouseButton, ButtonPressState pressState)
	{
		auto pCommand = new CommandType{};
		//m_MapOfMouseCommands[{mouseButton, pressState}] = std::move(std::unique_ptr<Command>(pCommand));
		m_MapOfMouseCommands.emplace_back(std::make_pair(MouseButtonType(mouseButton, pressState), std::move(std::unique_ptr<Command>(pCommand))));
		return *pCommand;
	}
}