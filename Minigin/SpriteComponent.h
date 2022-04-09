#pragma once
#include "BaseComponent.h"

namespace dae
{
	class GameObject;
	class Texture2D;

	class SpriteComponent final : public BaseComponent
	{

	public:
		/*Sprite(const std::string& filename, int nrCols, int nrRows, float frameSec);
		~Sprite();*/

		SpriteComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		SpriteComponent(std::nullptr_t) = delete;

		~SpriteComponent() = default;

		SpriteComponent(const SpriteComponent& other) = delete;
		SpriteComponent(SpriteComponent&& other) = delete;
		SpriteComponent& operator=(const SpriteComponent& other) = delete;
		SpriteComponent& operator=(SpriteComponent&& other) = delete;

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override;

		void SetTexture(const std::string& filename);
		void Setup(int columnCount, int rowCount, int fps, float width, float height);

	private:
		std::shared_ptr<Texture2D> m_pTexture{};
		GameObject* m_pParentObject;

		int m_Cols;
		int m_Rows;
		int m_CurrentFrame;

		float m_SecsPerFrame;
		float m_AccumulatedSec;

		float m_FrameWidth;
		float m_FrameHeight;
		float m_Width;
		float m_Height;

		bool m_InitDone;
	};
}

