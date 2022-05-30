#pragma once

namespace dae
{
	class BaseComponent
	{
	public:
		BaseComponent() = default;
		virtual ~BaseComponent() = default;

		virtual void LateInit() = 0;
		virtual void Update() = 0;
		virtual void LateUpdate() = 0;
		virtual void Render() const = 0;

		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;
	private:

	};
}
