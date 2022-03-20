#pragma once

namespace dae
{
	class GameObject;

	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;
		virtual void Notify(GameObject* pObject, int event) = 0;
	};
}

