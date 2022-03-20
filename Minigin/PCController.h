#pragma once
#include <memory>
#include "Command.h"

namespace dae
{
	class PCController final
	{
	public:
		PCController(size_t index = 0);
		~PCController();

		enum class ControllerButton
		{
			Button_DPAD_UP = 0x0001,
			Button_DPAD_DOWN = 0x0002,
			Button_DPAD_LEFT = 0x0004,
			Button_DPAD_RIGHT = 0x0008,
			Button_START = 0x0010,
			Button_BACK = 0x0020,
			Button_L_THUMB = 0x0040,
			Button_R_THUMB = 0x0080,
			Button_L_SHOULDER = 0x0100,
			Button_R_SHOULDER = 0x0200,
			Button_A = 0x1000,
			Button_B = 0x2000,
			Button_X = 0x4000,
			Button_Y = 0x8000,

			//Playstation layout buttons(have the same value), because my pc controller has a PS layout but software wise is just a xbox controller
			Button_Cross = 0x1000,
			Button_Circle = 0x2000,
			Button_Square = 0x4000,
			Button_Triangle = 0x8000
		};

		enum class ButtonPressState
		{
			PressedContinuous,
			OnPressed,
			Released
		};

		void Update();

		bool IsPressed(ControllerButton controllerButton) const;
		bool IsPressedThisFrame(ControllerButton controllerButton) const;
		bool IsReleasedThisFrame(ControllerButton controllerButton) const;

	private:
		class PCControllerImpl;
		PCControllerImpl* m_pImpl;
	};
}