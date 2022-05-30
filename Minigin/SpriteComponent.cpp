#include "NightOwlEnginePCH.h"
#include "SpriteComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "TimeSingleton.h"
#include "Texture2D.h"
//#include "Texture2DComponent.h"

using namespace dae;

SpriteComponent::SpriteComponent(GameObject* pParentObject)
	: m_pParentObject{ nullptr }
	, m_pTexture{ nullptr }
	, m_Cols{}
	, m_Rows{}
	, m_CurrentFrame{}
	, m_SecsPerFrame{}
	, m_AccumulatedSec{}
	, m_FrameWidth{}
	, m_FrameHeight{}
	, m_Width{}
	, m_Height{}
	, m_InitDone{ false }
{
	if (pParentObject)
		m_pParentObject = pParentObject;
	else
		Logger::GetInstance().LogWarning("SPRITECOMPONENT:\tPARENT OBJECT WAS NOT GIVEN!");
}

void SpriteComponent::LateInit()
{
	if (!m_pTexture)
	{
		Logger::GetInstance().LogError("SPRITECOMPONENT:\tTEXTURE WAS NOT GIVEN!");
		return;
	}

	if (!m_InitDone)
	{
		Logger::GetInstance().LogError("SPRITECOMPONENT:\tCOLUMNS, ROWS AND FPS WERE NOT SET!");
		return;
	}

	m_FrameWidth = float(m_pTexture->GetWidth()) / m_Cols;
	m_FrameHeight = float(m_pTexture->GetHeight()) / m_Rows;
}
void SpriteComponent::Update()
{
	m_AccumulatedSec += Time::GetInstance().GetDeltaT();

	if (m_AccumulatedSec >= m_SecsPerFrame)
	{
		++m_CurrentFrame;
		m_CurrentFrame %= (m_Cols * m_Rows);

		m_AccumulatedSec -= m_SecsPerFrame;
	}
}

void SpriteComponent::Render() const
{

	float srcX{ m_FrameWidth * (m_CurrentFrame % m_Cols) };
	float srcY{ m_FrameHeight * (m_CurrentFrame / m_Cols) };

	if (m_pTexture != nullptr)
	{
		const auto& pos = m_pParentObject->GetWorldPosition();
		Renderer::GetInstance().RenderTexture(*m_pTexture, srcX, srcY, m_FrameWidth, m_FrameHeight, pos.x, pos.y, m_Width, m_Height);
	}
}

void SpriteComponent::SetTexture(const std::string& filename)
{
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::SpriteComponent::Setup(int columns, int rows, int fps, float width, float height)
{
	m_Cols = columns;
	m_Rows = rows;
	m_SecsPerFrame = 1.f / fps;
	m_Width = width;
	m_Height = height;

	m_InitDone = true;
}
