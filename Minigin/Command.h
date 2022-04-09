#pragma once
#include <iostream>

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;

		virtual void Execute() = 0;

	protected:
		Command() = default;
	};











	//user
	//class FireCommand final : public Command
	//{
	//	void Execute() override { std::cout << "Fire\n";/*BLACK BOX THAT USER FLESHES OUT*/ };
	//	//int* m_pPlayer;//example code

	//	//void SetPlayer(int* p) { m_pPlayer = p };
	//};

	//class DuckCommand final : public Command
	//{
	//	void Execute() override { std::cout << "Duck\n"; };
	//};

	//class JumpCommand final : public Command
	//{
	//	void Execute() override { std::cout << "Jump\n"; };
	//};

	//class FartCommand final : public Command
	//{
	//	void Execute() override { std::cout << "Fart\n"; };
	//};
}