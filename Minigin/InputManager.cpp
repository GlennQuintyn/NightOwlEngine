#include "NightOwlEnginePCH.h"
#include "InputManager.h"

#pragma warning(push)
#pragma warning( disable : 33010 )
#pragma warning( disable : 6387 )
#include "../imgui-1.87/backends/imgui_impl_sdl.h"
#pragma warning (pop)

using namespace dae;

InputManager::InputManager()
	: m_pKeyboard{ std::make_unique<NoeKeyboard>() }
	, m_pMouse{ std::make_unique<NoeMouse>() }
{
	//TODO: remove this
	AddController();
}

//this shouldn't be needed and it should just automatically add/remove controllers
//TODO: Change to auto add and remove controllers, Check eliteeninge from matheu
void InputManager::AddController()
{
	if (m_pPCControllers.size() > 4)
	{
		//print error of there being to many controller aka max 4
		Logger::GetInstance().LogWarning("INPUTMANAGER:\tONLY ALLOWED A MAXIMUM OF 4 PC CONTROLLERS");
		return;
	}

	m_pPCControllers.push_back(std::make_unique<PCController>(m_pPCControllers.size()));
}

void dae::InputManager::CheckConnectedControllers()
{
}

bool InputManager::ProcessInput()
{
	m_pKeyboard->Update();
	m_pMouse->Update();

	//first check how many controllers are newly connected or disconnected
	CheckConnectedControllers();

	for (auto& controller : m_pPCControllers)
		controller->Update();

	//controller command code
	for (auto& pCommandPair : m_MapOfControllerCommands)
	{
		switch (pCommandPair.first.pressState)
		{
		case ButtonPressState::PressedContinuous:
			if (IsPressed(pCommandPair.first.button, pCommandPair.first.index))
				pCommandPair.second->Execute();
			break;
		case ButtonPressState::OnPressed:
			if (IsPressedThisFrame(pCommandPair.first.button, pCommandPair.first.index))
				pCommandPair.second->Execute();
			break;
		case ButtonPressState::Released:
			if (IsReleasedThisFrame(pCommandPair.first.button, pCommandPair.first.index))
				pCommandPair.second->Execute();
			break;
		}
	}

	//keyboard command code
	for (auto& keyboardCommand : m_MapOfKeyboardCommands)
	{
		switch (keyboardCommand.first.pressState)
		{
		case ButtonPressState::PressedContinuous:
			if (IsPressed(keyboardCommand.first.key))
				keyboardCommand.second->Execute();
			break;
		case ButtonPressState::OnPressed:
			if (IsPressedThisFrame(keyboardCommand.first.key))
				keyboardCommand.second->Execute();
			break;
		case ButtonPressState::Released:
			if (IsReleasedThisFrame(keyboardCommand.first.key))
				keyboardCommand.second->Execute();
			break;
		}
	}

	//mouse command code
	for (auto& mouseCommand : m_MapOfMouseCommands)
	{
		switch (mouseCommand.first.pressState)
		{
		case ButtonPressState::PressedContinuous:
			if (IsPressed(mouseCommand.first.button))
				mouseCommand.second->Execute();
			break;
		case ButtonPressState::OnPressed:
			if (IsPressedThisFrame(mouseCommand.first.button))
				mouseCommand.second->Execute();
			break;
		case ButtonPressState::Released:
			if (IsReleasedThisFrame(mouseCommand.first.button))
				mouseCommand.second->Execute();
			break;
		}
	}

	//SDL keyboard input
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		ImGui_ImplSDL2_ProcessEvent(&e);
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		//if (e.type == SDL_KEYDOWN)
		//{
		//	//e.key.keysym
		//}
		//if (e.type == SDL_MOUSEBUTTONDOWN)
		//{
		//}
	}

	return true;
}

#pragma region Controller Buttons
bool InputManager::IsPressed(PCController::ControllerButton controllerButton, size_t contollerIndex) const
{
	//if there are no controllers or the index is bigger than 3 (max 4 controllers so indices go from 0->3)
	if (contollerIndex > 3 || contollerIndex >= m_pPCControllers.size())
	{
		//Logger::GetInstance().LogWarning("INPUTMANAGER:\tGiven controller index was out of bounds");
		return false;
	}
	return m_pPCControllers[contollerIndex]->IsPressed(controllerButton);
}

bool InputManager::IsPressedThisFrame(PCController::ControllerButton controllerButton, size_t contollerIndex) const
{
	//if there are no controllers or the index is bigger than 3 (max 4 controllers so indices go from 0->3)
	if (contollerIndex > 3 || contollerIndex >= m_pPCControllers.size())
	{
		//Logger::GetInstance().LogWarning("INPUTMANAGER:\tGiven controller index was out of bounds");
		return false;
	}
	return m_pPCControllers[contollerIndex]->IsPressedThisFrame(controllerButton);
}

bool InputManager::IsReleasedThisFrame(PCController::ControllerButton controllerButton, size_t contollerIndex) const
{
	//if there are no controllers or the index is bigger than 3 (max 4 controllers so indices go from 0->3)
	if (contollerIndex > 3 || contollerIndex >= m_pPCControllers.size())
	{
		//Logger::GetInstance().LogWarning("INPUTMANAGER:\tGiven controller index was out of bounds");
		return false;
	}
	return m_pPCControllers[contollerIndex]->IsReleasedThisFrame(controllerButton);
}
#pragma endregion

#pragma region Keyboard Keys
bool dae::InputManager::IsPressed(NoeKeyboard::KeyboardKey keyboardButton) const
{
	return m_pKeyboard->IsPressed(keyboardButton);
	//return m_pCurrenStates[int(keyboardButton)];
}

bool dae::InputManager::IsPressedThisFrame(NoeKeyboard::KeyboardKey keyboardButton) const
{
	return m_pKeyboard->IsPressedThisFrame(keyboardButton);
	//return m_KeysPressedThisFrame[int(keyboardButton)];
}

bool dae::InputManager::IsReleasedThisFrame(NoeKeyboard::KeyboardKey keyboardButton) const
{
	return m_pKeyboard->IsReleasedThisFrame(keyboardButton);
	//return m_KeysReleasedThisFrame[int(keyboardButton)];
}
#pragma endregion

#pragma region Mouse Buttons
bool dae::InputManager::IsPressed(int mouseButton) const
{
	return m_pMouse->IsPressed(mouseButton);
	//return m_CurrentMouseButtons & mouseButton;
}
bool dae::InputManager::IsPressedThisFrame(int mouseButton) const
{
	return m_pMouse->IsPressedThisFrame(mouseButton);
	//return m_ButtonsPressedThisFrame & mouseButton;
}
bool dae::InputManager::IsReleasedThisFrame(int mouseButton) const
{
	return m_pMouse->IsReleasedThisFrame(mouseButton);
	//return m_ButtonsReleasedThisFrame & mouseButton;
}
#pragma endregion