#pragma once
namespace dae
{
	class NoeMouse final
	{
	public:

		enum class MouseButtons : int
		{
			Button_LEFT = 1,
			Button_MIDDLE = 2,
			Button_RIGHT = 4,
			Button_X1 = 8,
			Button_X2 = 16
		};

		NoeMouse();
		~NoeMouse() = default;

		void Update();

		const glm::ivec2& GetMousePosition() { return m_MousePos; };

		//uses a bit mask, if you want to test for more mouse buttons just continue the mask and cast to MouseButtons
		bool IsPressed(int mouseButton) const;
		//uses a bit mask, if you want to test for more mouse buttons just continue the mask and cast to MouseButtons
		bool IsPressedThisFrame(int mouseButton) const;
		//uses a bit mask, if you want to test for more mouse buttons just continue the mask and cast to MouseButtons
		bool IsReleasedThisFrame(int mouseButton) const;

	private:
		glm::ivec2 m_MousePos;

		int m_CurrentMouseButtons;
		int m_PrevMouseButtons;
		int m_ButtonsPressedThisFrame;
		int m_ButtonsReleasedThisFrame;
	};
}
