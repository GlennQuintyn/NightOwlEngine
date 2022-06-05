#include "NightOwlEnginePCH.h"
#include "InputManager.h"

#pragma warning(push)
#pragma warning( disable : 33010 )
#pragma warning( disable : 6387 )
#include "../imgui-1.87/backends/imgui_impl_sdl.h"
#pragma warning (pop)

using namespace dae;

InputManager::InputManager()
	: m_pCurrenStates{ SDL_GetKeyboardState(nullptr) }
	, m_pPrevStates{ new Uint8[SDL_NUM_SCANCODES] }
	, m_KeysPressedThisFrame{ new Uint8[SDL_NUM_SCANCODES] }
	, m_KeysReleasedThisFrame{ new Uint8[SDL_NUM_SCANCODES] }

	, m_CurrentMouseButtons{}
	, m_PrevMouseButtons{}
	, m_ButtonsPressedThisFrame{}
	, m_ButtonsReleasedThisFrame{}
{
	AddController();

	ZeroMemory(m_pPrevStates, sizeof(Uint8)* SDL_NUM_SCANCODES);
	ZeroMemory(m_KeysPressedThisFrame, sizeof(Uint8)* SDL_NUM_SCANCODES);
	ZeroMemory(m_KeysReleasedThisFrame, sizeof(Uint8)* SDL_NUM_SCANCODES);
}

dae::InputManager::~InputManager()
{
	delete[] m_pPrevStates;
	delete[] m_KeysPressedThisFrame;
	delete[] m_KeysReleasedThisFrame;
}

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

bool InputManager::ProcessInput()
{
	//byte copy all items from the SDL_GetKeyboardState(m_pKeyBoardStates) to current or smt
	//DO that xor thingy and just make it like the controller

	for (size_t index = 0; index < SDL_NUM_SCANCODES; index++)
	{
		char keysChanged = m_pCurrenStates[index] ^ m_pPrevStates[index];
		m_KeysPressedThisFrame[index] = keysChanged & m_pCurrenStates[index];
		m_KeysReleasedThisFrame[index] = keysChanged & (~m_pCurrenStates[index]);

		m_pPrevStates[index] = m_pCurrenStates[index];
	}

	m_CurrentMouseButtons = SDL_GetMouseState(&m_MousePos.x, &m_MousePos.y);

	int buttonsChanged = m_CurrentMouseButtons ^ m_PrevMouseButtons;
	m_ButtonsPressedThisFrame = buttonsChanged & m_CurrentMouseButtons;
	m_ButtonsReleasedThisFrame = buttonsChanged & (~m_CurrentMouseButtons);

	m_PrevMouseButtons = m_CurrentMouseButtons;

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
bool dae::InputManager::IsPressed(KeyboardKey keyboardButton) const
{
	return m_pCurrenStates[int(keyboardButton)];
}

bool dae::InputManager::IsPressedThisFrame(KeyboardKey keyboardButton) const
{
	return m_KeysPressedThisFrame[int(keyboardButton)];
}

bool dae::InputManager::IsReleasedThisFrame(KeyboardKey keyboardButton) const
{
	return m_KeysReleasedThisFrame[int(keyboardButton)];
}
#pragma endregion

#pragma region Mouse Buttons
bool dae::InputManager::IsPressed(int mouseButton) const
{
	return m_CurrentMouseButtons & mouseButton;
}
bool dae::InputManager::IsPressedThisFrame(int mouseButton) const
{
	return m_ButtonsPressedThisFrame & mouseButton;
}
bool dae::InputManager::IsReleasedThisFrame(int mouseButton) const
{
	return m_ButtonsReleasedThisFrame & mouseButton;
}
#pragma endregion