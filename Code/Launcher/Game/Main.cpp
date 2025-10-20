/**
 * @file
 * @brief Game launcher.
 */

 // CryEngine headers
#include <windows.h>
#include "IGameStartup.h"
#include "ISystem.h"
#include "include/MemUtils.h"
#include <direct.h>
#pragma comment(lib, "user32")

// tell to nVidia GPU driver that Crysis needs powerful graphics card and not the slow one (for multi-GPU laptops)
extern "C" __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

// do the same as above for AMD GPU driver
// TODO: it seems this thing has no effect on modern AMD hardware, so user has to explicitly choose faster GPU
extern "C" __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;

static void ErrorBox( const char *msg )
{
	MessageBoxA( NULL, msg, "Error", MB_OK | MB_DEFAULT_DESKTOP_ONLY );
}



void CryFindEngineRootFolder(unsigned int engineRootPathSize, char* szEngineRootPath)
{
	char exePath[MAX_PATH];
	char currentPath[MAX_PATH];
	char* lastSlash = NULL;

	// Get full path to the executable
	DWORD result = GetModuleFileNameA(NULL, exePath, sizeof(exePath));
	if (result == 0 || result == sizeof(exePath))
	{
		ErrorBox("Failed to get executable path.");
		return;
	}

	// Copy to mutable buffer
	strncpy(currentPath, exePath, sizeof(currentPath));
	currentPath[sizeof(currentPath) - 1] = '\0';

	// Strip off executable name
	lastSlash = strrchr(currentPath, '\\');
	if (lastSlash)
		*lastSlash = '\0';

	while (1)
	{
		// Check for "engine" subfolder
		char enginePath[MAX_PATH];
		sprintf_s(enginePath, sizeof(enginePath), "%s\\engine", currentPath);

		DWORD attribs = GetFileAttributesA(enginePath);
		if (attribs != INVALID_FILE_ATTRIBUTES && (attribs & FILE_ATTRIBUTE_DIRECTORY))
		{
			size_t len = strlen(currentPath);
			if (len + 1 > engineRootPathSize)
			{
				char errorMsg[512];
				sprintf_s(errorMsg, sizeof(errorMsg),
					"CryEngine root path is too long.\nMaxPathSize: %u\nPathSize: %zu\nPath: %s",
					engineRootPathSize, len + 1, currentPath);
				ErrorBox(errorMsg);
				return;
			}

			strcpy(szEngineRootPath, currentPath);
			return;
		}

		// Move up one directory
		lastSlash = strrchr(currentPath, '\\');
		if (!lastSlash || lastSlash == currentPath)
		{
			break;
		}

		*lastSlash = '\0';
	}

	ErrorBox("Unable to locate CryEngine root folder.\nMake sure the 'engine' folder exists in the root.");
}

static int RunGame( HMODULE libCryGame )
{
	IGameStartup::TEntryFunction fCreateGameStartup;

	fCreateGameStartup = (IGameStartup::TEntryFunction) GetProcAddress( libCryGame, "CreateGameStartup2" );
	if ( fCreateGameStartup == NULL )
	{
		ErrorBox( "The CryGame DLL is not valid!" );
		return 1;
	}

	IGameStartup *pGameStartup = fCreateGameStartup();
	if ( pGameStartup == NULL )
	{
		ErrorBox( "Unable to create the GameStartup interface!" );
		return 1;
	}

	const char *cmdLine = GetCommandLineA();
	const size_t cmdLineLength = strlen( cmdLine );

	SSystemInitParams params;
	memset( &params, 0, sizeof params );

	params.hInstance = GetModuleHandle( NULL );
	params.sLogFileName = "Game.log";
	params.bExecuteCommandLine = true;

	if ( cmdLineLength < sizeof params.szSystemCmdLine )
	{
		strcpy( params.szSystemCmdLine, cmdLine );
	}
	else
	{
		ErrorBox( "Command line is too long!" );
		return 1;
	}

	// move to root folder of game before initializing
	char szEngineRootDir[MAX_PATH];
	CryFindEngineRootFolder(sizeof(szEngineRootDir), szEngineRootDir);
	SetCurrentDirectoryA(szEngineRootDir);
	// init engine
	IGameRef gameRef = pGameStartup->Init( params );
	if ( gameRef == NULL )
	{
		ErrorBox( "Game initialization failed!" );
		pGameStartup->Shutdown();
		return 1;
	}

	// enter update loop
	pGameStartup->Run( NULL );

	pGameStartup->Shutdown();

	return 0;
}

int __stdcall WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	HMODULE libCryGame = LoadLibraryA( "CryGameCrysis2.dll" );
	if ( libCryGame == NULL )
	{
		ErrorBox( "Unable to load the CryGame DLL!" );
		return 1;
	}

	HMODULE libCryAction = LoadLibraryA( "CryAction.dll" );
	if ( libCryAction == NULL )
	{
		ErrorBox( "Unable to load the CryAction DLL!" );
		return 1;
	}

	uintptr_t base = reinterpret_cast<uintptr_t>(libCryAction);

	// fix crash on level changing
	WriteMem<int8_t>(base + 0xBC7C, 0xEB);
	WriteMem<int8_t>(base + 0xBC8A, 0xEB);
	WriteMem<int8_t>(base + 0xBDE0, 0xEB);
	WriteMem<int8_t>(base + 0xAF21, 0xE9);
	WriteMem<int8_t>(base + 0xAF22, 0xB2);
	WriteMem<int8_t>(base + 0xAF23, 0x00);
	WriteMem<int8_t>(base + 0xAF26, 0x90);
	WriteMem<int8_t>(base + 0xB400, 0xEB);
	WriteMem<int8_t>(base + 0xBB4A, 0xEB);
	WriteMem<int8_t>(base + 0xBFD5, 0xEB);

	HMODULE libCryNetwork = LoadLibraryA( "CryNetwork.dll" );
	if ( libCryNetwork == NULL )
	{
		ErrorBox( "Unable to load the CryNetwork DLL!" );
		return 1;
	}

	HMODULE libCrySystem = LoadLibraryA( "CrySystem.dll" );
	if ( libCrySystem == NULL )
	{
		ErrorBox( "Unable to load the CrySystem DLL!" );
		return 1;
	}

	// launch the game
	int status = RunGame( libCryGame );

	FreeLibrary( libCrySystem );
	FreeLibrary( libCryNetwork );
	FreeLibrary( libCryAction );
	FreeLibrary( libCryGame );

	return status;
}

