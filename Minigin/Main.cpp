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

extern dae::Achievement_t g_Achievements[];
extern dae::SteamAchievements* g_SteamAchievements;

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
		g_SteamAchievements = new dae::SteamAchievements(g_Achievements, 4);
	}

	dae::NightOwlEngine engine;
	engine.Run();

	SteamAPI_Shutdown();

	if (g_SteamAchievements)
		delete g_SteamAchievements;

	return 0;
}