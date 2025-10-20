// Copyright (C) 2001-2008 Crytek GmbH

#pragma once

struct ISystem;
struct ISystemUserCallback;
struct ILog;
struct ILogCallback;
struct IValidator;
struct IOutputPrintSink;


struct SSystemInitParams
{
  void *hInstance;
  void *hWnd;
  ILog *pLog;
  ILogCallback *pLogCallback;
  ISystemUserCallback *pUserCallback;
  const char *sLogFileName;
  IValidator *pValidator;
  IOutputPrintSink *pPrintSync;
  char szSystemCmdLine[2048];
  char szUserPath[256];
  char szBinariesDir[256];
  bool bEditor;
  bool bPreview;
  bool bTestMode;
  bool bDedicatedServer;
  bool bExecuteCommandLine;
  bool bUIFramework;
  bool bSkipFont;
  bool bSkipRenderer;
  bool bSkipConsole;
  bool bSkipNetwork;
  bool bMinimal;
  bool bSkipInput;
  bool bTesting;
  bool bNoRandom;
  bool bShaderCacheGen;
  ISystem *pSystem;
  void *pCheckFunc;
  void *pProtectedFunctions[10];
  void *pCvarsDefault;
};
