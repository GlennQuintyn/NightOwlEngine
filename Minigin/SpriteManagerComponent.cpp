#include "NightOwlEnginePCH.h"
#include "SpriteManagerComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "GameObject.h"
#include "ResourceManager.h"

//TODO: maybe change this to a FSM system instead of this ... "logic" stuff

namespace dae
{
	class SpriteManagerComponent::Sprite final
	{
	public:
		Sprite(GameObject* pParentObject, const std::string& filename, int columnCount, int rowCount, int fps, float width, float height)
			: m_pParentObject{ pParentObject }
			, m_Cols{ columnCount }
			, m_Rows{ rowCount }
			, m_CurrentFrame{ 0 }
			, m_SecsPerFrame{ 1.f / fps }
			, m_AccumulatedSec{ 0.f }
			, m_FrameWidth{ 0.f }
			, m_FrameHeight{ 0.f }
			, m_Width{ width }
			, m_Height{ height }
			, m_Loop{ false }
		{
			if (filename.empty())
			{
				Logger::GetInstance().LogError("SpriteManager: sprite filename was empty!");
				return;
			}
			m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
		}

		void LateInit()
		{
			m_FrameWidth = float(m_pTexture->GetWidth()) / m_Cols;
			m_FrameHeight = float(m_pTexture->GetHeight()) / m_Rows;
		}
		void Update()
		{
			m_AccumulatedSec += Time::GetInstance().GetDeltaT();

			//if it should not loop, then ..... TODO: rethink about this logic?
			//	- once it reached the final frame AND
			//	- one frame time has passed
			//it should stop updating the frames and stop at the last frame
			if (!m_Loop && m_CurrentFrame == ((m_Cols * m_Rows) - 1) && m_AccumulatedSec >= m_SecsPerFrame)
				return;

			if (m_AccumulatedSec >= m_SecsPerFrame)
			{
				++m_CurrentFrame;
				m_CurrentFrame %= (m_Cols * m_Rows);

				m_AccumulatedSec -= m_SecsPerFrame;
			}
		}

		void Render() const
		{
			float srcX{ m_FrameWidth * (m_CurrentFrame % m_Cols) };
			float srcY{ m_FrameHeight * (m_CurrentFrame / m_Cols) };

			if (m_pTexture != nullptr)
			{
				const auto& pos = m_pParentObject->GetWorldPosition();
				Renderer::GetInstance().RenderTexture(*m_pTexture, srcX, srcY, m_FrameWidth, m_FrameHeight, pos.x, pos.y, m_Width, m_Height);
			}
		}

		void Reset()
		{
			m_AccumulatedSec = 0.f;
			m_CurrentFrame = 0;
		}

		//so that the sprite only plays once and stops at the last frame and not the first frame
		void SetLoopState(bool loop) { m_Loop = loop; };

		~Sprite() = default;

	private:
		int m_Cols;
		int m_Rows;
		int m_CurrentFrame;

		float m_SecsPerFrame;
		float m_AccumulatedSec;

		float m_FrameWidth;
		float m_FrameHeight;
		float m_Width;
		float m_Height;

		bool m_Loop;

		GameObject* m_pParentObject;
		std::shared_ptr<Texture2D> m_pTexture{};
	};
}

dae::SpriteManagerComponent::SpriteManagerComponent(GameObject* pParentObject)
	: m_pParentObject{ pParentObject }
	, m_CurrentSpriteIndex{ 0 }
	, m_CurrentSpritePlayType{ SpritePlayType::PlayOnce }
	, m_IsMoving{ false }
	//, m_Loop{ false }
{
	m_pSpriteArray = std::make_unique<std::vector<Sprite>>();
}

void dae::SpriteManagerComponent::LateInit()
{
	auto& vectorOfSprites{ *m_pSpriteArray.get() };

	for (auto& Sprite : vectorOfSprites)
	{
		Sprite.LateInit();
	}
}

//void dae::SpriteManagerComponent::Update()
//{
//	//if the sprite is not looping then just update it and let the sprite play out
//	//if the PlaySprite gets called again for a new sprite then the current not looping sprite will get interrupted
//	if (!m_Loop)
//	{
//		m_pSpriteArray->at(m_CurrentSpriteIndex).Update();
//		return;
//	}
//
//	//if in the current frame there was no movement then it should play the idle frame
//	if (!m_IsMoving)
//	{
//		m_CurrentSpriteIndex = 0;
//		m_pSpriteArray->at(m_CurrentSpriteIndex).Reset();
//	}
//
//	m_pSpriteArray->at(m_CurrentSpriteIndex).Update();
//
//	//reset the moving flag after each update
//	m_IsMoving = false;
//}

void dae::SpriteManagerComponent::Update()
{
	//if the sprite is not looping then just update it and let the sprite play out
	//if the PlaySprite gets called again for a new sprite then the current not looping sprite will get interrupted

	switch (m_CurrentSpritePlayType)
	{
	case dae::SpriteManagerComponent::SpritePlayType::PlayOnce://the looping bool in the sprite class takes care of the this
	case dae::SpriteManagerComponent::SpritePlayType::Looping:
		m_pSpriteArray->at(m_CurrentSpriteIndex).Update();
		break;
	case dae::SpriteManagerComponent::SpritePlayType::LoopUntilNoMovement:
		//if in the current frame there was no movement then it should play the idle frame
		if (!m_IsMoving)
		{
			m_CurrentSpriteIndex = 0;
			m_pSpriteArray->at(m_CurrentSpriteIndex).Reset();
		}
		else
			m_pSpriteArray->at(m_CurrentSpriteIndex).Update();
		break;
	default:
		Logger::GetInstance().LogError("SPRITEMANAGERCOMPONENT: invalid switch state reached!");
		break;
	}

	//reset the moving flag after each update
	m_IsMoving = false;
}

void dae::SpriteManagerComponent::Render() const
{
	m_pSpriteArray->at(m_CurrentSpriteIndex).Render();
}

//void dae::SpriteManagerComponent::PlaySprite(uint32_t index, bool loop)
//{
//	m_IsMoving = true && loop;//not looping sprites shouldn't move 
//	m_Loop = loop;
//
//	//if its the same sprite don't do anything
//	if (index == m_CurrentSpriteIndex)
//		return;
//
//	if (index < m_pSpriteArray->size())
//	{
//		m_CurrentSpriteIndex = index;
//		m_pSpriteArray->at(m_CurrentSpriteIndex).Reset();
//		m_pSpriteArray->at(m_CurrentSpriteIndex).SetLoopState(loop);
//	}
//	else
//		Logger::GetInstance().LogWarning("SPRITEMANAGER: index was out of bounds");
//}

//index of the sprite, playtype: PlayOnce, Looping and LoopUntilNoMovement
void dae::SpriteManagerComponent::PlaySprite(uint32_t index, SpritePlayType playType)
{
	//the LoopUntilNoMovement is for when a sprite should keep looping until no more input was given so that i would go to an idle frame/sprite animation
	m_CurrentSpritePlayType = playType;
	if (m_CurrentSpritePlayType == SpritePlayType::LoopUntilNoMovement)
		m_IsMoving = true;

	//if its the same sprite don't do anything
	if (m_CurrentSpritePlayType != SpritePlayType::PlayOnce && index == m_CurrentSpriteIndex)
		return;

	if (index < m_pSpriteArray->size())
	{
		m_CurrentSpriteIndex = index;
		m_pSpriteArray->at(m_CurrentSpriteIndex).Reset();

		if (m_CurrentSpritePlayType == SpritePlayType::Looping || m_CurrentSpritePlayType == SpritePlayType::LoopUntilNoMovement)
			m_pSpriteArray->at(m_CurrentSpriteIndex).SetLoopState(true);
		else
			m_pSpriteArray->at(m_CurrentSpriteIndex).SetLoopState(false);
	}
	else
		Logger::GetInstance().LogWarning("SPRITEMANAGER: index was out of bounds");
}

void dae::SpriteManagerComponent::AddSprite(const std::string& filename, int columnCount, int rowCount, int fps, float width, float height)
{
	m_pSpriteArray->emplace_back(m_pParentObject, filename, columnCount, rowCount, fps, width, height);
}
