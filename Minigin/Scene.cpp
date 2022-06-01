#include "NightOwlEnginePCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "SceneManager.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string_view name) : m_Name(name) {}

GameObject& Scene::CreateObject(const std::string_view name/*const std::unique_ptr<GameObject>& object*/)
{
	m_Objects.push_back(std::make_unique<GameObject>(name, this));
	return *m_Objects.back();
	//return *m_Objects.emplace_back(name, this);
}

void dae::Scene::TakeOwnership(std::unique_ptr<GameObject> pObject)
{
	m_Objects.push_back(std::move(pObject));
}

std::unique_ptr<GameObject> dae::Scene::ReleaseOwnership(GameObject& pObject)
{
	auto it = std::find_if(m_Objects.begin(), m_Objects.end(), [&pObject](const std::unique_ptr<GameObject>& pUniqueObject)
		{
			return pUniqueObject.get() == &pObject;
		});

	if (it != m_Objects.end())
	{
		std::unique_ptr<GameObject> object = std::move(*it);
		m_Objects.erase(it);
		return object;
	}

	std::stringstream errormsg{};
	errormsg << "GAMEOBJECT:\tOBJECT WITH NAME " << pObject.GetName() << " COULD NOT BE REALEASED!";
	Logger::GetInstance().LogWarning(errormsg.view());
	return {};
}

void dae::Scene::LateInit()
{
	for (auto& object : m_Objects)
	{
		object->LateInit();
	}
}

void Scene::Update()
{
	for (auto& object : m_Objects)
	{
		object->Update();
	}
}

void dae::Scene::FixedUpdate(float deltaT)
{
	for (auto& object : m_Objects)
	{
		object->FixedUpdate(deltaT);
	}
}

void dae::Scene::LateUpdate()
{
	for (const auto& object : m_Objects)
	{
		object->LateUpdate();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

void dae::Scene::Reset()
{
	for (auto& object : m_Objects)
	{
		object->Reset();
	}
}