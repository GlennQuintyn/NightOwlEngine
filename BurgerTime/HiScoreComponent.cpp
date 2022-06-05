#include "BurgerTimePCH.h"
#include "HiScoreComponent.h"
#include <TextComponent.h>
#include "ScoreComponent.h"
#include <fstream>
#include <filesystem>

using namespace dae;

int HiScoreComponent::m_Score{};

HiScoreComponent::HiScoreComponent(GameObject* pParentObject)
	: m_pParentObject{ nullptr }
	, m_HiscorePath{ "../Data/Hi-Score/HISCORE.txt" }
	, m_pTextComponent{ nullptr }
	, m_pScoreComponent{ nullptr }
{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("HISCORECOMPONENT:\tPARENT OBJECT WAS NOT GIVEN!");
}

void HiScoreComponent::LateInit()
{
	if (!m_pTextComponent)
	{
		Logger::GetInstance().LogError("HISCORECOMPONENT:\tTEXTCOMPONENT WAS NOT GIVEN!");
		return;
	}
	if (!m_pScoreComponent)
	{
		Logger::GetInstance().LogError("HISCORECOMPONENT:\tSCORECOMPONENT WAS NOT GIVEN!");
		return;
	}

	int scoreFromFile{ ReadScoreFromFile() };
	if (m_Score < scoreFromFile)
	{
		m_Score = scoreFromFile;
	}

	m_pTextComponent->SetText(std::to_string(m_Score));//set intial score value
}

void HiScoreComponent::Update()
{
	if (m_pScoreComponent->GetCurrentScore() > m_Score)
	{
		m_Score = m_pScoreComponent->GetCurrentScore();
		m_pTextComponent->SetText(std::to_string(m_Score));//update score value
	}
}

void dae::HiScoreComponent::Reset()
{
	if (ReadScoreFromFile() < m_Score)
	{
		WriteScoreToFile(m_Score);
	}
}

void HiScoreComponent::SetTextComponent(TextComponent& textComponent)
{
	m_pTextComponent = &textComponent;
}

void HiScoreComponent::SetScoreComponent(ScoreComponent& scoreComponent)
{
	m_pScoreComponent = &scoreComponent;
}

int dae::HiScoreComponent::ReadScoreFromFile()
{
	if (std::filesystem::exists(m_HiscorePath))
	{
		int scoreFromFile{};
		try
		{
			std::ifstream hiscoreFile{ m_HiscorePath };
			hiscoreFile >> scoreFromFile;
			return scoreFromFile;
		}
		catch (const std::exception& e)
		{
			using namespace std::string_literals;

			Logger::GetInstance().LogError("HISCORE: FAILED TO LOAD FROM FILE"s + e.what());
			return -1;
		}
	}
	return 0;
}

void dae::HiScoreComponent::WriteScoreToFile(int score)
{
	try
	{
		std::ofstream hiscoreFile{ m_HiscorePath, std::ios::trunc };
		hiscoreFile << score;
	}
	catch (const std::exception& e)
	{
		using namespace std::string_literals;
		Logger::GetInstance().LogError("HISCORE: FAILED TO SAVE TO FILE"s + e.what());
	}
}
