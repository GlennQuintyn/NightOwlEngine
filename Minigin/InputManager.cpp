#include "NightOwlEnginePCH.h"
#include "InputManager.h"

#pragma warning(push)
#pragma warning( disable : 33010 )
#pragma warning( disable : 6387 )
#include "../imgui-1.87/backends/imgui_impl_sdl.h"
#pragma warning (pop)

using namespace dae;

InputManager::InputManager()
{
	AddController();
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
	//controller input code
	for (size_t index = 0; index < m_pPCControllers.size(); index++)
	{
		m_pPCControllers[index]->Update();

		//m_MapOfCommands.at(ControllerButton(m_CurrentState.Gamepad.wButtons))->Execute();
		for (auto& pCommandPair : m_MapOfCommands)
		{
			switch (pCommandPair.first.pressState)
			{
			case PCController::PCController::ButtonPressState::PressedContinuous:
				//if (WORD(pCommandPair.first.button) & m_CurrentState.Gamepad.wButtons)
				if (IsPressed(pCommandPair.first.button, index))
					pCommandPair.second->Execute();
				break;
			case PCController::ButtonPressState::OnPressed:
				//if (WORD(pCommandPair.first.button) & m_ButtonsPressedThisFrame)
				if (IsPressedThisFrame(pCommandPair.first.button, index))
					pCommandPair.second->Execute();
				break;
			case PCController::ButtonPressState::Released:
				//if (WORD(pCommandPair.first.button) & m_ButtonsReleasedThisFrame)
				if (IsReleasedThisFrame(pCommandPair.first.button, index))
					pCommandPair.second->Execute();
				break;
			}
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
		if (e.type == SDL_KEYDOWN)
		{

		}
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{

		}
	}

	return true;
}

bool InputManager::IsPressed(PCController::ControllerButton controllerButton, size_t contollerIndex) const
{
	return m_pPCControllers[contollerIndex]->IsPressed(controllerButton);
}

bool InputManager::IsPressedThisFrame(PCController::ControllerButton controllerButton, size_t contollerIndex) const
{
	return m_pPCControllers[contollerIndex]->IsPressedThisFrame(controllerButton);
}

bool InputManager::IsReleasedThisFrame(PCController::ControllerButton controllerButton, size_t contollerIndex) const
{
	return m_pPCControllers[contollerIndex]->IsReleasedThisFrame(controllerButton);
}
