#pragma once
#include "BaseComponent.h"

namespace dae
{
	class GameObject;

	class SpriteManagerComponent final : public BaseComponent
	{
	public:
		SpriteManagerComponent(GameObject* pParentObject);
		~SpriteManagerComponent() = default;

		//make sure that component has a parent that isn't nullptr
		SpriteManagerComponent(std::nullptr_t) = delete;

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override;

		void PlaySprite(uint32_t index, bool loop = true);
		void AddSprite(const std::string& filename, int columnCount, int rowCount, int fps, float width, float height);

		//		void SetTexture(const std::string& filename);
		//		void Setup(int columnCount, int rowCount, int fps, float width, float height);
		SpriteManagerComponent(const SpriteManagerComponent& other) = delete;
		SpriteManagerComponent(SpriteManagerComponent&& other) = delete;
		SpriteManagerComponent& operator=(const SpriteManagerComponent& other) = delete;
		SpriteManagerComponent& operator=(SpriteManagerComponent&& other) = delete;

	private:
		GameObject* m_pParentObject;

		uint32_t m_CurrentSprite;

		class Sprite;
		std::unique_ptr<std::vector<Sprite>> m_pSpriteArray;
		bool m_IsMoving;
	};
}