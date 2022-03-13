#include "NightOwlEnginePCH.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>

#pragma comment(lib, "xinput.lib")

#include "PCController.h"
#include <memory>

namespace dae
{
	class PCController::PCControllerImpl
	{
	public:
		PCControllerImpl(size_t controllerIndex)
			:m_ControllerIndex(controllerIndex)
		{
			// Zeroise the state
			ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
			ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
		}

		void Update()
		{	
			// Get the state
			XInputGetState(DWORD(m_ControllerIndex), &m_CurrentState);

			auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
			m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
			m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);			

			m_PreviousState = m_CurrentState;
		}

		bool IsPressed(ControllerButton controllerButton) const { return WORD(controllerButton) & m_CurrentState.Gamepad.wButtons; }
		bool IsPressedThisFrame(ControllerButton controllerButton) const { return WORD(controllerButton) & m_ButtonsPressedThisFrame; }
		bool IsReleasedThisFrame(ControllerButton controllerButton) const { return WORD(controllerButton) & m_ButtonsReleasedThisFrame; }

	private:
		
		XINPUT_STATE m_CurrentState{};
		XINPUT_STATE m_PreviousState{};

		WORD m_ButtonsPressedThisFrame{};
		WORD m_ButtonsReleasedThisFrame{};

		size_t m_ControllerIndex{};
	};
}

using namespace dae;

PCController::PCController(size_t index)
{
	m_pImpl = new PCControllerImpl(index);
}

PCController::~PCController()
{
	delete m_pImpl;
}

void PCController::Update()
{
	m_pImpl->Update();
}

bool PCController::IsPressed(ControllerButton controllerButton) const
{
	return m_pImpl->IsPressed(controllerButton);
}

bool PCController::IsPressedThisFrame(ControllerButton controllerButton) const
{
	return m_pImpl->IsPressedThisFrame(controllerButton);;
}

bool PCController::IsReleasedThisFrame(ControllerButton controllerButton) const
{
	return m_pImpl->IsReleasedThisFrame(controllerButton);;
}