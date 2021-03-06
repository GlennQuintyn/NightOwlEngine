#pragma once
#include "BaseComponent.h"
#include <vector>

namespace dae
{
	class GameObject;

	class TrashTheCacheComponent : public BaseComponent
	{
	public:
		TrashTheCacheComponent(GameObject* pParentObject);
		~TrashTheCacheComponent() = default;

		void LateInit() override {};
		void Update() override;
		void LateUpdate() override {};
		void Render() const override;
		void Reset() override {};

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

		GameObject* m_pParentObject;

		int m_SampleSize{ 10 };

		bool m_PlotButton1Pressed{};
		bool m_PlotButton2Pressed{};
		bool m_PlotButton3Pressed{};
	};
}