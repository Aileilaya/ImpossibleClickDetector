#pragma once
#pragma comment(lib,"Version.lib")
#include "pch.h"

#ifndef TOOLS_H
#define TOOLS_H
extern unsigned int dwGameDll;
static inline bool IsInGame() {
	return *(unsigned int*)(dwGameDll + 0xBE5F78) == 4 && *(unsigned int*)(dwGameDll + 0xBE5F7C) == 4;
};

static inline void DisplayText(const char* szText, float fDuration = 30.0f) {
	if (IsInGame())
	{
		void* GameGlobalUI = (void*)(0xBE6350 + dwGameDll);
		unsigned int PrintTextCall = dwGameDll + 0x3577B0;
		__asm
		{
			PUSH	0xFFFFFFFF;
			PUSH	fDuration;
			PUSH	szText;
			MOV		ECX, [GameGlobalUI];
			MOV		ECX, [ECX];
			MOV		EAX, PrintTextCall;
			CALL	EAX;
		}
	}
};

static inline unsigned int ObjectToHandle(void* pObj) {
	if (!pObj) return NULL;
	unsigned int result = NULL;
	unsigned int GameGlobalObject = dwGameDll + 0xBE4238;
	unsigned int GameHandleFromObject = dwGameDll + 0x2651D0;
	unsigned int GameStateGet = dwGameDll + 0x1C3200;
	__asm {
		mov ecx, GameGlobalObject
		mov ecx, [ecx]
		call GameStateGet
		mov ecx, eax
		push 0
		push pObj
		call GameHandleFromObject
		mov result, eax
	}
	return result;
};

static inline unsigned int WarcraftVersion() {
	DWORD dwHandle = NULL;
	DWORD dwLen = GetFileVersionInfoSize(L"Game.dll", &dwHandle);
	char* lpData(new char[dwLen]);
	GetFileVersionInfo(L"Game.dll", dwHandle, dwLen, lpData);
	LPBYTE lpBuffer = NULL;
	UINT   uLen = NULL;
	VerQueryValue(lpData, L"\\", (LPVOID*)&lpBuffer, &uLen);
	VS_FIXEDFILEINFO* Version = (VS_FIXEDFILEINFO*)lpBuffer;
	DWORD ret = Version->dwFileVersionLS;
	delete[]lpData;
	return LOWORD(ret);
};
#endif

union Float
{
	float fl;
	unsigned int dw;
};

namespace jass {
	typedef unsigned int(__cdecl* pPlayer)				(unsigned int slot);
	extern pPlayer Player;
	typedef int(__cdecl* pGetPlayerController)(unsigned int whichPlayer);
	extern pGetPlayerController GetPlayerController;
	typedef int(__cdecl* pGetPlayerSlotState)(unsigned int whichPlayer);
	extern pGetPlayerSlotState GetPlayerSlotState;
	typedef bool(__cdecl* pGetPlayerAlliance)		(unsigned int hPlayer1, unsigned int hPlayer2, unsigned int type);
	extern pGetPlayerAlliance	GetPlayerAlliance;
	typedef unsigned int(__cdecl* pGetOwningPlayer)		(unsigned int unit);
	extern pGetOwningPlayer GetOwningPlayer;
	typedef Float(__cdecl* pGetUnitX)				(unsigned int hUnit);
	extern pGetUnitX	GetUnitX;
	typedef Float(__cdecl* pGetUnitY)				(unsigned int hUnit);
	extern pGetUnitY	GetUnitY;
	typedef void(__cdecl* pPingMinimapEx)			(float* x, float* y, const float* duration, int red, int green, int blue, bool extraEffects);
	extern pPingMinimapEx	PingMinimapEx;
	typedef unsigned int(__cdecl* pGetPlayerColor)		(unsigned int hPlayer);
	extern pGetPlayerColor GetPlayerColor;
	typedef bool(__cdecl* pIsUnitVisible)			(unsigned int hUnit, unsigned int hPlayer);
	extern pIsUnitVisible IsUnitVisible;
	typedef bool(__cdecl* pIsVisibleToPlayer)(float* x, float* y, unsigned int whichPlayer);
	extern pIsVisibleToPlayer IsVisibleToPlayer;

	char* GetPlayerName(unsigned int hPlayer);
	char* GetUnitName(unsigned int hUnit);
	unsigned int GetPlayerBySlot(int slot);
	void init();
}
