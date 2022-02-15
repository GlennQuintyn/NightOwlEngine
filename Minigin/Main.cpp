#include "NightOwlEnginePCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "NightOwlEngine.h"

int main(int, char*[])
{
	dae::NightOwlEngine engine;
	engine.Run();
    return 0;
}