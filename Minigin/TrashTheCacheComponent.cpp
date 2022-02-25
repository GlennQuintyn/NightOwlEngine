#include "NightOwlEnginePCH.h"
#include "TrashTheCacheComponent.h"
#include <chrono>

#include "../imgui-1.87/backends/imgui_impl_sdl.h"
#include "../imgui-1.87/backends/imgui_impl_opengl2.h"

struct Transform
{
	float matrix[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
};

class GameObject3D
{
public:
	Transform m_transform{};
	int ID{ 16 };

	operator int() const
	{
		return ID;
	}
};

class GameObject3DPTR
{
public:
	Transform* m_transform{};
	int ID{ 16 };

	operator int() const
	{
		return ID;
	}
};

template<typename T>
inline void dae::TrashTheCacheComponent::TrashTheCache(std::vector<float>& graphData)
{
	int uselessOperatorVariable{ 1 };
	std::vector<T> arr{};
	arr.resize(10'000'000);
	int maxStepsize{ 2048 };
	//the max stepsize is 2048 so in 
	graphData.resize(int(std::log2f(float(maxStepsize))));

	for (size_t sample = 0; sample < m_SampleSize; sample++)
	{
		int sampleIndex{};
		for (size_t stepsize{ 1 }; stepsize < maxStepsize; stepsize *= 2)
		{
			auto start = std::chrono::high_resolution_clock::now();
			for (size_t j = 0; j < arr.size(); j += stepsize)
			{
				uselessOperatorVariable ^= arr[j];
			}

			auto end = std::chrono::high_resolution_clock::now();

			//add new timestamp and then average them out
			graphData[sampleIndex] += std::chrono::duration<float>(end - start).count();
			graphData[sampleIndex] *= .5f;

			sampleIndex++;
		}
	}

	//simple func to use the varible so that the operations in the loop don't get removed for optimizations
	putchar(uselessOperatorVariable);
}

void dae::TrashTheCacheComponent::Update()
{
	ImGui::Begin("Trash The cache!");

	int prevSampleSize{ m_SampleSize };

	ImGui::InputInt("Sample size", &m_SampleSize);

	if (m_SampleSize != prevSampleSize)
	{
		m_PlotButton1Pressed = false;
		m_PlotButton2Pressed = false;
		m_PlotButton3Pressed = false;
	}

	if (!m_PlotButton1Pressed)
	{
		m_PlotButton1Pressed = ImGui::Button("Calc and plot int cach timings");
		if (m_PlotButton1Pressed)
		{
			ImGui::Text("Calculating...");
			TrashTheCache<int>(m_GraphData1);
		}
	}
	else
	{
		ImGui::Text("plot int cach timings");
		ImGui::PlotLines("int array", m_GraphData1.data(), int(m_GraphData1.size()), 0, nullptr, FLT_MAX, FLT_MAX, { 420,150 });
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	if (!m_PlotButton2Pressed)
	{
		m_PlotButton2Pressed = ImGui::Button("Calc and plot object unoptimized cach timings");

		if (m_PlotButton2Pressed)
		{
			ImGui::Text("Calculating...");
			TrashTheCache<GameObject3D>(m_GraphData2);
		}
	}
	else
	{
		ImGui::Text("plot object cach timings");
		ImGui::PlotLines("object array", m_GraphData2.data(), int(m_GraphData2.size()), 0, nullptr, FLT_MAX, FLT_MAX, { 420,150 });
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	if (!m_PlotButton3Pressed)
	{
		m_PlotButton3Pressed = ImGui::Button("Calc and plot object optimized cach timings");

		if (m_PlotButton3Pressed)
		{
			ImGui::Text("Calculating...");
			TrashTheCache<GameObject3DPTR>(m_GraphData3);
		}
	}
	else
	{
		ImGui::Text("plot object with ptr cach timings");
		ImGui::PlotLines("object array with ptr", m_GraphData3.data(), int(m_GraphData3.size()), 0, nullptr, FLT_MAX, FLT_MAX, { 420,150 });
	}

	ImGui::End();
}

void dae::TrashTheCacheComponent::Render() const
{
}
