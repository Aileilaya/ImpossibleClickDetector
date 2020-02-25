#include "pch.h"
#include "util.h"

namespace jass {
	static unsigned int addrGetPlayerName1, addrGetPlayerName2;
	static unsigned int addrUnitName1, addrUnitName2;
	pPlayer					Player;
	pGetPlayerController	GetPlayerController;
	pGetPlayerSlotState		GetPlayerSlotState;
	pGetPlayerAlliance		GetPlayerAlliance;
	pGetOwningPlayer		GetOwningPlayer;
	pGetUnitX				GetUnitX;
	pGetUnitY				GetUnitY;
	pPingMinimapEx			PingMinimapEx;
	pGetPlayerColor			GetPlayerColor;
	pIsUnitVisible			IsUnitVisible;
	pIsVisibleToPlayer		IsVisibleToPlayer;
	void init() {
		Player = (pPlayer)(dwGameDll + 0x1F1E70);
		GetPlayerController = (pGetPlayerController)(dwGameDll + 0x1E3CC0);
		GetPlayerSlotState = (pGetPlayerSlotState)(dwGameDll + 0x1E3FE0);
		GetPlayerAlliance = (pGetPlayerAlliance)(dwGameDll + 0x1E3C50);
		GetOwningPlayer = (pGetOwningPlayer)(dwGameDll + 0x1E3BA0);
		GetUnitX = (pGetUnitX)(dwGameDll + 0x1E66B0);
		GetUnitY = (pGetUnitY)(dwGameDll + 0x1E66F0);
		PingMinimapEx = (pPingMinimapEx)(dwGameDll + 0x1F1C30);
		GetPlayerColor = (pGetPlayerColor)(dwGameDll + 0x1E3CA0);
		IsUnitVisible = (pIsUnitVisible)(dwGameDll + 0x1E8E80);
		IsVisibleToPlayer = (pIsVisibleToPlayer)(dwGameDll + 0x1E8F50);

		addrGetPlayerName1 = dwGameDll + 0x1D03D0;
		addrGetPlayerName2 = dwGameDll + 0x24A890;
		addrUnitName1 = (dwGameDll + 0x1D1550);
		addrUnitName2 = (dwGameDll + 0x326BA0);
	}

	char* GetPlayerName(unsigned int hPlayer)
	{
		char* retaddr = (char*)"";
		__asm
		{
			mov ecx, hPlayer;
			CALL DWORD PTR DS : [addrGetPlayerName1]
			test eax, eax;
			jz NOPLAYER;
			push 1;
			mov ecx, eax;
			CALL DWORD PTR DS : [addrGetPlayerName2]
			NOPLAYER :
			mov retaddr, eax;
		}
		return retaddr;
	}

	char* GetUnitName(unsigned int hUnit)
	{
		char* retaddr = NULL;
		__asm
		{
			mov ecx, hUnit;
			CALL DWORD PTR DS : [addrUnitName1]
			test eax, eax;
			je NoUnit;
			mov ecx, [eax + 0x30];
			xor edx, edx;
			CALL DWORD PTR DS : [addrUnitName2]
			mov retaddr, eax;
		NoUnit:
		}
		return retaddr;
	}

	unsigned int GetPlayerBySlot(int slot)
	{
		unsigned int GlobalPlayerData = *(unsigned int*)(0xBE4238 + dwGameDll);
		unsigned int result = NULL;
		result = *(unsigned int*)(GlobalPlayerData + 0x58 + (slot * 4));
		return result;
	}
}