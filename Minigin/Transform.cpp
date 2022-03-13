#include "NightOwlEnginePCH.h"
#include "Transform.h"

using namespace dae;

Transform::Transform()
	: position{}
	, scale{ 1.f, 1.f }
	, rotation{}
	, zDept{}
{
}

Transform::Transform(const Transform& other)
	: position{ other.position }
	, scale{ other.scale }
	, rotation{ other.rotation }
	, zDept{ other.zDept }
{
}

Transform::Transform(const glm::vec2& position, float rotation, const glm::vec2& scale, float zDept)
	: position{ position }
	, scale{ scale }
	, rotation{ rotation }
	, zDept{ zDept }
{
}

Transform::Transform(float posX, float posY, float rotation, float scalX, float scalY, float zDept)
	: position{ posX, posY }
	, scale{ scalX, scalY }
	, rotation{ rotation }
	, zDept{ zDept }
{
}

//
//void dae::Transform::SetPosition(const float x, const float y)
//{
//	m_Position.x = x;
//	m_Position.y = y;
//}
//
//void dae::Transform::SetRotation(float radians)
//{
//	m_Rotation = radians;
//}
//
//void dae::Transform::SetScale(float x, float y)
//{
//	m_Scale.x = x;
//	m_Scale.y = y;
//}
//
//void dae::Transform::SetZDept(float z)
//{
//	m_ZDept = z;
//}
