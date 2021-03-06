#include "NightOwlEnginePCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "NightOwlEngine.h"

#pragma warning(push)
#pragma warning( disable : 26812 )
#pragma warning( disable : 4996 )
#include <steam_api.h>
#pragma warning (pop)

#include "SteamAchievements.h"

int main(int, char* [])
{
	if (!SteamAPI_Init())
	{
		std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
		return 1;
	}
	else
	{
		std::cout << "Successfully initialized steam." << std::endl;
		dae::SteamAchievements::g_SteamAchievements = new dae::SteamAchievements(dae::SteamAchievements::g_Achievements, 4);
	}

	dae::NightOwlEngine engine{ 640,480 };
	engine.Run();

	SteamAPI_Shutdown();

	if (dae::SteamAchievements::g_SteamAchievements)
		delete dae::SteamAchievements::g_SteamAchievements;

	return 0;
}