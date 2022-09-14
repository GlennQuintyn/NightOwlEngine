#include "NightOwlEnginePCH.h"
#include "NoeKeyboard.h"

using namespace dae;

dae::NoeKeyboard::NoeKeyboard()
	: m_pCurrenStates{ SDL_GetKeyboardState(nullptr) }
	, m_pPrevStates{ new Uint8[SDL_NUM_SCANCODES] }
	, m_KeysPressedThisFrame{ new Uint8[SDL_NUM_SCANCODES] }
	, m_KeysReleasedThisFrame{ new Uint8[SDL_NUM_SCANCODES] }
{
	ZeroMemory(m_pPrevStates, sizeof(Uint8) * SDL_NUM_SCANCODES);
	ZeroMemory(m_KeysPressedThisFrame, sizeof(Uint8) * SDL_NUM_SCANCODES);
	ZeroMemory(m_KeysReleasedThisFrame, sizeof(Uint8) * SDL_NUM_SCANCODES);
}

dae::NoeKeyboard::~NoeKeyboard()
{
	delete[] m_pPrevStates;
	delete[] m_KeysPressedThisFrame;
	delete[] m_KeysReleasedThisFrame;
}

void NoeKeyboard::Update()
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
}

#pragma region Keyboard Keys
bool NoeKeyboard::IsPressed(KeyboardKey keyboardButton) const
{
	return m_pCurrenStates[int(keyboardButton)];
}

bool NoeKeyboard::IsPressedThisFrame(KeyboardKey keyboardButton) const
{
	return m_KeysPressedThisFrame[int(keyboardButton)];
}

bool NoeKeyboard::IsReleasedThisFrame(KeyboardKey keyboardButton) const
{
	return m_KeysReleasedThisFrame[int(keyboardButton)];
}
#pragma endregion