#pragma once
namespace dae
{
	struct Transform final
	{
		//public:
		//	Transform();

		//	void SetPosition(float x, float y);
		//	void SetRotation(float radians);
		//	void SetScale(float x, float y);
		//	void SetZDept(float z);

		//	const glm::vec2& GetPosition() const { return m_Position; }
		//	const float& GetRotation() const { return m_Rotation; }
		//	const glm::vec2& GetScale() const { return m_Scale; }
		//	const float GetZDept() const { return m_ZDept; }

		//private:

		//constructors
		Transform();
		Transform(const Transform& other);
		Transform(const glm::vec2& position, float rotation, const glm::vec2& scale, float zDept);
		Transform(float posX, float posY, float rotation, float scalX, float scalY, float zDept);

		glm::vec2 position;
		glm::vec2 scale;
		float rotation;
		float zDept;

		//operator overloading
		Transform operator+(const Transform& other)
		{
			Transform temp;
			temp.position = this->position + other.position;
			temp.scale = this->scale * other.scale;
			temp.rotation = this->rotation + other.rotation;
			temp.zDept = this->zDept + other.zDept;
			return temp;
		}

		Transform operator-(const Transform& other)
		{
			Transform temp;
			temp.position = this->position - other.position;
			temp.scale = this->scale / other.scale;
			temp.rotation = this->rotation - other.rotation;
			temp.zDept = this->zDept - other.zDept;
			return temp;
		}
	};
}
