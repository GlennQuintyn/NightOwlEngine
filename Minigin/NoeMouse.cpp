#include "NightOwlEnginePCH.h"
#include "NoeMouse.h"

using namespace dae;

NoeMouse::NoeMouse()
	: m_CurrentMouseButtons{}
	, m_PrevMouseButtons{}
	, m_ButtonsPressedThisFrame{}
	, m_ButtonsReleasedThisFrame{}
{
}

void NoeMouse::Update()
{
	m_CurrentMouseButtons = SDL_GetMouseState(&m_MousePos.x, &m_MousePos.y);

	int buttonsChanged = m_CurrentMouseButtons ^ m_PrevMouseButtons;
	m_ButtonsPressedThisFrame = buttonsChanged & m_CurrentMouseButtons;
	m_ButtonsReleasedThisFrame = buttonsChanged & (~m_CurrentMouseButtons);

	m_PrevMouseButtons = m_CurrentMouseButtons;
}

#pragma region Mouse Buttons
bool NoeMouse::IsPressed(int mouseButton) const
{
	return m_CurrentMouseButtons & mouseButton;
}
bool NoeMouse::IsPressedThisFrame(int mouseButton) const
{
	return m_ButtonsPressedThisFrame & mouseButton;
}
bool NoeMouse::IsReleasedThisFrame(int mouseButton) const
{
	return m_ButtonsReleasedThisFrame & mouseButton;
}
#pragma endregion