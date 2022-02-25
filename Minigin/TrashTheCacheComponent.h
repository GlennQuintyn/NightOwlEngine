#pragma once
#include "BaseComponent.h"
#include <vector>

//#include "Transform.h"

namespace dae
{
	class TrashTheCacheComponent : public BaseComponent
	{
	public:
		TrashTheCacheComponent() = default;
		~TrashTheCacheComponent() = default;

		virtual void Update() override;
		virtual void Render() const override;

		//void SetTexture(const std::string& filename);
		TrashTheCacheComponent(const TrashTheCacheComponent& other) = delete;
		TrashTheCacheComponent(TrashTheCacheComponent&& other) = delete;
		TrashTheCacheComponent& operator=(const TrashTheCacheComponent& other) = delete;
		TrashTheCacheComponent& operator=(TrashTheCacheComponent&& other) = delete;
	private:

		template<typename T>
		void TrashTheCache(std::vector<float>& graphData);

		std::vector<float> m_GraphData1{};
		std::vector<float> m_GraphData2{};
		std::vector<float> m_GraphData3{};

		int m_SampleSize{ 10 };

		bool m_PlotButton1Pressed{};
		bool m_PlotButton2Pressed{};
		bool m_PlotButton3Pressed{};
	};

	/*template<typename T>
	inline void TrashTheCacheComponent::TrashTheCache(std::vector<float>& graphData)
	{
	}*/
}

/*
#include "BaseComponent.h"

namespace dae
{
	class Font;
	class Texture2D;

	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent();
		~FPSComponent() = default;

		//todo: change fps mode to a seperate component and add precision of float to as option and maybe add code to make fps more or less volatile
		void SetFont(const std::shared_ptr<Font>& font) { m_Font = font; };
		void SetPosition(float x, float y) { m_Transform.SetPosition(x, y, 0.0f); };
		void SetTextColor(const SDL_Color& color) { m_TextColor = color; };
		void SetTextColor(unsigned char r, unsigned char g, unsigned char b) { m_TextColor = SDL_Color{ r, g, b }; };

		virtual void Update() override;
		virtual void Render() const override;

		//void SetTexture(const std::string& filename);
		FPSComponent(const FPSComponent& other) = delete;
		FPSComponent(FPSComponent&& other) = delete;
		FPSComponent& operator=(const FPSComponent& other) = delete;
		FPSComponent& operator=(FPSComponent&& other) = delete;
	private:*/