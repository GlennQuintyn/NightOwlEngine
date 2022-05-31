#pragma once
//#include <XInput.h>
#include "Singleton.h"

#include "PCController.h"
#include <vector>
#include <memory>
#include <map>

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		enum class KeyboardKey
		{
#pragma region letters
			Key_A = SDL_SCANCODE_A,
			Key_B = SDL_SCANCODE_B,
			Key_C = SDL_SCANCODE_C,
			Key_D = SDL_SCANCODE_D,
			Key_E = SDL_SCANCODE_E,
			Key_F = SDL_SCANCODE_F,
			Key_G = SDL_SCANCODE_G,
			Key_H = SDL_SCANCODE_H,
			Key_I = SDL_SCANCODE_I,
			Key_J = SDL_SCANCODE_J,
			Key_K = SDL_SCANCODE_K,
			Key_L = SDL_SCANCODE_L,
			Key_M = SDL_SCANCODE_M,
			Key_N = SDL_SCANCODE_N,
			Key_O = SDL_SCANCODE_O,
			Key_P = SDL_SCANCODE_P,
			Key_Q = SDL_SCANCODE_Q,
			Key_R = SDL_SCANCODE_R,
			Key_S = SDL_SCANCODE_S,
			Key_T = SDL_SCANCODE_T,
			Key_U = SDL_SCANCODE_U,
			Key_V = SDL_SCANCODE_V,
			Key_W = SDL_SCANCODE_W,
			Key_X = SDL_SCANCODE_X,
			Key_Y = SDL_SCANCODE_Y,
			Key_Z = SDL_SCANCODE_Z,
#pragma endregion

#pragma region Numbers and F-Keys
			Key_1 = SDL_SCANCODE_1,
			Key_2 = SDL_SCANCODE_2,
			Key_3 = SDL_SCANCODE_3,
			Key_4 = SDL_SCANCODE_4,
			Key_5 = SDL_SCANCODE_5,
			Key_6 = SDL_SCANCODE_6,
			Key_7 = SDL_SCANCODE_7,
			Key_8 = SDL_SCANCODE_8,
			Key_9 = SDL_SCANCODE_9,
			Key_0 = SDL_SCANCODE_0,
			Key_F1 = SDL_SCANCODE_F1,
			Key_F2 = SDL_SCANCODE_F2,
			Key_F3 = SDL_SCANCODE_F3,
			Key_F4 = SDL_SCANCODE_F4,
			Key_F5 = SDL_SCANCODE_F5,
			Key_F6 = SDL_SCANCODE_F6,
			Key_F7 = SDL_SCANCODE_F7,
			Key_F8 = SDL_SCANCODE_F8,
			Key_F9 = SDL_SCANCODE_F9,
			Key_F10 = SDL_SCANCODE_F10,
			Key_F11 = SDL_SCANCODE_F11,
			Key_F12 = SDL_SCANCODE_F12,
#pragma endregion

#pragma region Keypad
			Key_KPad_1 = SDL_SCANCODE_KP_1,
			Key_KPad_2 = SDL_SCANCODE_KP_2,
			Key_KPad_3 = SDL_SCANCODE_KP_3,
			Key_KPad_4 = SDL_SCANCODE_KP_4,
			Key_KPad_5 = SDL_SCANCODE_KP_5,
			Key_KPad_6 = SDL_SCANCODE_KP_6,
			Key_KPad_7 = SDL_SCANCODE_KP_7,
			Key_KPad_8 = SDL_SCANCODE_KP_8,
			Key_KPad_9 = SDL_SCANCODE_KP_9,
			Key_KPad_0 = SDL_SCANCODE_KP_0,
			key_KPad_NUMLOCK = SDL_SCANCODE_NUMLOCKCLEAR,
			Key_KPad_DIVIDE = SDL_SCANCODE_KP_DIVIDE,
			Key_KPad_MULTIPLY = SDL_SCANCODE_KP_MULTIPLY,
			Key_KPad_MINUS = SDL_SCANCODE_KP_MINUS,
			Key_KPad_PLUS = SDL_SCANCODE_KP_PLUS,
			Key_KPad_ENTER = SDL_SCANCODE_KP_ENTER,
			Key_KPad_PERIOD = SDL_SCANCODE_KP_PERIOD,
			Key_KPad_EQUALS = SDL_SCANCODE_KP_EQUALS,
#pragma endregion

#pragma region Symbols and action keys
			Key_MINUS = SDL_SCANCODE_MINUS,						// "-"
			Key_EQUALS = SDL_SCANCODE_EQUALS,					// "="
			Key_SQUARE_BRACKET_L = SDL_SCANCODE_LEFTBRACKET,	// "["
			Key_SQUARE_BRACKET_R = SDL_SCANCODE_RIGHTBRACKET,	// "]"
			Key_FORWARDSLASH = SDL_SCANCODE_SLASH,				// "/"
			Key_BACKSLASH = SDL_SCANCODE_BACKSLASH,				// "\"
			Key_SEMICOLON = SDL_SCANCODE_SEMICOLON,				// ";"
			Key_APOSTROPHE = SDL_SCANCODE_APOSTROPHE,			// "'"
			Key_GRAVE = SDL_SCANCODE_GRAVE,						// "`"
			Key_COMMA = SDL_SCANCODE_COMMA,						// ","
			Key_PERIOD = SDL_SCANCODE_PERIOD,					// "."

			Key_ENTER = SDL_SCANCODE_RETURN,
			Key_ESCAPE = SDL_SCANCODE_ESCAPE,
			Key_BACKSPACE = SDL_SCANCODE_BACKSPACE,
			Key_TAB = SDL_SCANCODE_TAB,
			Key_SPACE = SDL_SCANCODE_SPACE,

			Key_CAPSLOCK = SDL_SCANCODE_CAPSLOCK,
			Key_PRINTSCREEN = SDL_SCANCODE_PRINTSCREEN,
			Key_SCROLLLOCK = SDL_SCANCODE_SCROLLLOCK,
			Key_PAUSE = SDL_SCANCODE_PAUSE,
			Key_INSERT = SDL_SCANCODE_INSERT,

			Key_HOME = SDL_SCANCODE_HOME,
			Key_PAGEUP = SDL_SCANCODE_PAGEUP,
			Key_DELETE = SDL_SCANCODE_DELETE,
			Key_END = SDL_SCANCODE_END,
			Key_PAGEDOWN = SDL_SCANCODE_PAGEDOWN,
			Key_ARROW_RIGHT = SDL_SCANCODE_RIGHT,
			Key_ARROW_LEFT = SDL_SCANCODE_LEFT,
			Key_ARROW_DOWN = SDL_SCANCODE_DOWN,
			Key_ARROW_UP = SDL_SCANCODE_UP,

			Key_CTRL_L = SDL_SCANCODE_LCTRL,
			Key_CTRL_R = SDL_SCANCODE_RCTRL,
			Key_SHIFT_L = SDL_SCANCODE_LSHIFT,
			Key_SHIFT_R = SDL_SCANCODE_RSHIFT,
			Key_ALT_L = SDL_SCANCODE_LALT, // alt
			Key_ALT_R = SDL_SCANCODE_RALT, // alt gr
			Key_GUI_L = SDL_SCANCODE_LGUI, // windows key
			Key_GUI_R = SDL_SCANCODE_RGUI, // windows key
#pragma endregion

			//SDL_NUM_SCANCODES = 512 // not a key, just marks the number of scancodes for array bounds
		};

		enum class MouseButtons : int
		{
			Button_LEFT = 1,
			Button_MIDDLE = 2,
			Button_RIGHT = 4,
			Button_X1 = 8,
			Button_X2 = 16
		};

		enum class ButtonPressState
		{
			PressedContinuous,
			OnPressed,
			Released
		};

		InputManager();
		~InputManager();

		bool ProcessInput();

		bool IsPressed(PCController::ControllerButton controllerButton, size_t contollerIndex = 0) const;
		bool IsPressedThisFrame(PCController::ControllerButton controllerButton, size_t contollerIndex = 0) const;
		bool IsReleasedThisFrame(PCController::ControllerButton controllerButton, size_t contollerIndex = 0) const;

		bool IsPressed(KeyboardKey keyboardButton) const;
		bool IsPressedThisFrame(KeyboardKey keyboardButton) const;
		bool IsReleasedThisFrame(KeyboardKey keyboardButton) const;

		//uses a bit mask, if you want to test for more mouse buttons just continue the mask and cast to MouseButtons
		bool IsPressed(MouseButtons mouseButton) const;
		//uses a bit mask, if you want to test for more mouse buttons just continue the mask and cast to MouseButtons
		bool IsPressedThisFrame(MouseButtons mouseButton) const;
		//uses a bit mask, if you want to test for more mouse buttons just continue the mask and cast to MouseButtons
		bool IsReleasedThisFrame(MouseButtons mouseButton) const;

		const glm::ivec2& GetMousePosition() { return m_MousePos; };

		//TODO: change add controlle to works automatically aka detect if controller gets added or removed and only itterate over the ones that are still there
		//adds a new controller up to 4, when the input manager gets constructed a controller gets added automatically
		void AddController();

		template<typename CommandType>
		CommandType& AddCommand(PCController::ControllerButton controllerButton, ButtonPressState pressState);

		template<typename CommandType>
		CommandType& AddCommand(KeyboardKey keyboardKey, ButtonPressState pressState);

		template<typename CommandType>
		CommandType& AddCommand(int mouseButton, ButtonPressState pressState);


	private:
		struct ControllerButtonType
		{
			PCController::ControllerButton button;
			ButtonPressState pressState;

			std::strong_ordering operator<=>(ControllerButtonType other) const
			{
				return other.button <=> button;
			}
		};

		struct KeyboardKeyType
		{
			KeyboardKey key;
			ButtonPressState pressState;

			std::strong_ordering operator<=>(KeyboardKeyType other) const
			{
				return other.key <=> key;
			}
		};

		struct MouseButtonType
		{
			MouseButtons button;
			ButtonPressState pressState;

			std::strong_ordering operator<=>(MouseButtonType other) const
			{
				return other.button <=> button;
			}
		};

		std::map<ControllerButtonType, std::unique_ptr<Command>> m_MapOfControllerCommands;
		std::map<KeyboardKeyType, std::unique_ptr<Command>> m_MapOfKeyboardCommands;
		std::map<MouseButtonType, std::unique_ptr<Command>> m_MapOfMouseCommands;
		std::vector<std::unique_ptr<PCController>> m_pPCControllers;

		const Uint8* m_pCurrenStates;
		Uint8* m_pPrevStates;
		Uint8* m_KeysPressedThisFrame;
		Uint8* m_KeysReleasedThisFrame;

		glm::ivec2 m_MousePos;

		int m_CurrentMouseButtons;
		int m_PrevMouseButtons;
		int m_ButtonsPressedThisFrame;
		int m_ButtonsReleasedThisFrame;
	};

	template<typename CommandType>
	inline CommandType& InputManager::AddCommand(PCController::ControllerButton controllerButton, ButtonPressState pressState)
	{
		auto pCommand = new CommandType{};
		m_MapOfControllerCommands[{controllerButton, pressState}] = std::move(std::unique_ptr<Command>(pCommand));
		return *pCommand;
	}

	template<typename CommandType>
	inline CommandType& InputManager::AddCommand(KeyboardKey keyboardKey, ButtonPressState pressState)
	{
		auto pCommand = new CommandType{};
		m_MapOfKeyboardCommands[{keyboardKey, pressState}] = std::move(std::unique_ptr<Command>(pCommand));
		return *pCommand;
	}

	template<typename CommandType>
	inline CommandType& InputManager::AddCommand(int mouseButton, ButtonPressState pressState)
	{
		auto pCommand = new CommandType{};
		m_MapOfMouseCommands[{mouseButton, pressState}] = std::move(std::unique_ptr<Command>(pCommand));
		return *pCommand;
	}
}