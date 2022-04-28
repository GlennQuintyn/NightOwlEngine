#pragma once
//#include <Texture2D.h>
//disabling all warnings that come from this math library
#pragma warning(push)
#pragma warning( disable : 26495 )
#pragma warning( disable : 26812 )
#pragma warning( disable : 26819 )
#pragma warning( disable : 4201 )
#include <glm/glm.hpp>
//#include <SDL.h>
#pragma warning (pop)

#include <memory>

#include "BaseComponent.h"
#include "Observer.h"

namespace dae
{
	class GameObject;
	class TextComponent;
	class Texture2D;

	class LivesComponent final : public BaseComponent, public Observer
	{
	public:
		LivesComponent(GameObject* pParentObject);

		//make sure that component has a parent that isn't nullptr
		LivesComponent(std::nullptr_t) = delete;

		~LivesComponent() = default;

		//negative value for either x or y means that the orriginal image size will be used, if not then the given custom size will be used instead
		void SetTexture(const std::string& filename, float sizeX = -1.f, float sizeY = -1.f);
		void SetSize(const glm::vec2& size) { m_IconSize = size; };
		void SetSize(const float x, const float y) {
			m_IconSize.x = x; m_IconSize.y = y;
		};

		void Notify(GameObject* pObject, int event) override;

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override;

		LivesComponent(const LivesComponent& other) = delete;
		LivesComponent(LivesComponent&& other) = delete;
		LivesComponent& operator=(const LivesComponent& other) = delete;
		LivesComponent& operator=(LivesComponent&& other) = delete;
	private:
		std::shared_ptr<Texture2D> m_pTexture;
		glm::vec2 m_IconSize;
		GameObject* m_pParentObject;
		int m_CurrentLives;
	};
}
