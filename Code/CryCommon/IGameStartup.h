// Copyright (C) 2001-2004 Crytek GmbH

#pragma once

#include "IGameRef.h"

struct SSystemInitParams;

struct IGameStartup
{
        virtual ~IGameStartup(){}
        // Summary
        //   Entry function to the game
        // Returns
        //   a new instance of the game startup
        // Description
        //   Entry function used to create a new instance of the game
        typedef IGameStartup *(*TEntryFunction)();

        // Description:
        //              Initialize the game and/or any MOD, and get the IGameMod interface.
        //              The shutdown method, must be called independent of this method's return value.
        // Arguments:
        //              startupParams - Pointer to SSystemInitParams structure containing system initialization setup!
        // Return Value:
        //              Pointer to a IGameMod interface, or 0 if something went wrong with initialization.
        virtual IGameRef Init(SSystemInitParams &startupParams) = 0;

        // Description:
        //              Shuts down the game and any loaded MOD and delete itself.
        virtual void Shutdown() = 0;

        // Description:
        //              Updates the game.
        // Arguments:
        //              haveFocus - Boolean describing if the game has the input focus or not.
        // Return Value:
        //              0 to terminate the game (i.e. when quitting), non-zero to continue
        virtual int Update(bool haveFocus, unsigned int updateFlags) = 0;

        // Description:
        //              Returns a restart level and thus triggers a restart. 
        // Return Value:
        //              NULL to quit the game, a levelname to restart with that level
        virtual bool GetRestartLevel(char** levelName) = 0;

        // Description:
        //              Returns whether a patch needs installing
        //      Return Value:
        //      path+filename if patch to install, null otherwise
        virtual const char* GetPatch() const = 0;
        //              Retrieves the next mod to use in case the engine got a restart request.
        // Return Value:
        //              true will indicates that a mod is requested
        virtual bool GetRestartMod(char* pModName, int nameLenMax) = 0;

        // Description:
        //              Initiate and control the game loop!
        // Arguments:
        //    autoStartLevelName - name of the level to jump into, or NULL for normal behaviour
        // Return Value:
        //              0 when the game terminated normally, non-zero otherwise
        virtual int Run( const char * autoStartLevelName ) = 0;
};

