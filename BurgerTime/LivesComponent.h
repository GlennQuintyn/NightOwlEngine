#pragma once
#include <BaseComponent.h>
#include <Observer.h>
#include <Subject.h>

namespace dae
{
	class GameObject;
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

		Subject& GetSubject() { return m_Subject; }

		void Notify(GameObject* pObject, int event) override;

		void LateInit() override;
		void Update() override;
		void LateUpdate() override {};
		void Render() const override;
		void Reset() override {};

		LivesComponent(const LivesComponent& other) = delete;
		LivesComponent(LivesComponent&& other) = delete;
		LivesComponent& operator=(const LivesComponent& other) = delete;
		LivesComponent& operator=(LivesComponent&& other) = delete;
	private:
		Subject m_Subject;
		std::shared_ptr<Texture2D> m_pTexture;
		glm::vec2 m_IconSize;
		GameObject* m_pParentObject;
		static int m_CurrentLives;
	};
}
