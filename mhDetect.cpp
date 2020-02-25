#include "pch.h"
#include "mhDetect.h"
#include <stdio.h>
#include <random>

const auto SELECTINFOSIZE = 12;
static mh selectInfo[SELECTINFOSIZE];
std::mt19937 g(GetTickCount64());

void mhDetect::init()
{
	HWND hWnd = FindWindowW(L"Warcraft III", L"Warcraft III");
	int i = 10;
	bool f = false;
	while (--i > 0) {
		if (SetTimer(hWnd, g(), 300, (TIMERPROC)detect) != 0) {
			f = true;
			break;
		}
	}
	if (f == false) {
		MessageBoxW(NULL, L"initialize failed", L"Warning", MB_ICONSTOP | MB_APPLMODAL | MB_TOPMOST);
	}
	memset(selectInfo, 0, sizeof(selectInfo));
}

void __stdcall mhDetect::detect()
{
	static bool isPlayerSet = false;
	if (IsInGame()) {
		if (false == isPlayerSet) {
			for (int i = 0; i < 12; i++) {
				selectInfo[i].player = jass::Player(i);
			}
			isPlayerSet = true;
		}
		static char printBuffer[256];

		for (int i = 0; i < 12; i++)
		{
			unsigned int playerHandle = selectInfo[i].player;
			if (playerHandle == NULL
				|| jass::GetPlayerController(playerHandle) != 0
				|| jass::GetPlayerSlotState(playerHandle) != 1
				)
				continue;
			unsigned int selectedUnit = getPlayerSelectedUnit(i);
			if (selectedUnit != NULL) {
				if (selectedUnit == selectInfo[i].unit) {
					if (selectInfo[i].showMsgCount > 3) continue;
				}
				else {
					selectInfo[i].unit = selectedUnit;
					selectInfo[i].count = 0;
					selectInfo[i].showMsgCount = 0;
				}
				selectInfo[i].count++;
				if (jass::GetPlayerAlliance(playerHandle, jass::GetOwningPlayer(selectedUnit), 5)) continue;//ALLIANCE_SHARED_VISION == 5
				float x = jass::GetUnitX(selectedUnit).fl;
				float y = jass::GetUnitY(selectedUnit).fl;
				int state = isUnitVisible(selectedUnit, playerHandle, x, y);
				if (state > 0 && selectInfo[i].count < 10) {
					selectInfo[i].showMsgCount++;
					float duration = 4.0f;
					jass::PingMinimapEx(&x, &y, &duration, 255, 0, 0, true);
					if (state == 4) {
						_snprintf_s(printBuffer, _TRUNCATE, "%s%s|r selected invisibled %s%s|r [Count : %d]",
							GetPlayerColorString(i), jass::GetPlayerName(playerHandle),
							GetPlayerColorString(jass::GetPlayerColor(jass::GetOwningPlayer(selectedUnit))), jass::GetUnitName(selectedUnit),
							selectInfo[i].count);
					}
					else if (state == 6) {
						_snprintf_s(printBuffer, _TRUNCATE, "%s%s|r selected fogged %s%s|r [Count : %d]",
							GetPlayerColorString(i), jass::GetPlayerName(playerHandle),
							GetPlayerColorString(jass::GetPlayerColor(jass::GetOwningPlayer(selectedUnit))), jass::GetUnitName(selectedUnit),
							selectInfo[i].count);
					}
					DisplayText(printBuffer);
				}
			}
		}
	}
	else {
		isPlayerSet = false;
	}
}

int mhDetect::isUnitVisible(unsigned int unit, unsigned int player, float x, float y)
{
	bool isUnitVisible = jass::IsUnitVisible(unit, player);
	bool isLocVisible = jass::IsVisibleToPlayer(&x, &y, player);
	int ret = 0;
	if (!isUnitVisible) {
		ret += 4;
		if (!isLocVisible) ret += 2;
	}
	return ret;
}

unsigned int mhDetect::getPlayerSelectedUnit(int slot)
{
	unsigned int unitHandle = NULL;
	if (slot < 12 && slot >= 0) {
		unsigned int player = jass::GetPlayerBySlot(slot);
		if (player) {
			unsigned int playerData = *(unsigned int*)(player + 0x34);
			unsigned int unit2 = *(unsigned int*)(playerData + 0x1a4);
			int unitCount1 = *(int*)(playerData + 0x10);
			if (unitCount1 == 1 && unit2 != NULL) {
				unitHandle = ObjectToHandle((void*)unit2);
			}
		}
	}
	return unitHandle;
}

const char* GetPlayerColorString(int c)
{
	const char* ret;
	switch (c) {
	case PLAYER_COLOR_RED:
		ret = "|cffFF0202";
		break;
	case PLAYER_COLOR_BLUE:
		ret = "|cff0041FF";
		break;
	case PLAYER_COLOR_CYAN:
		ret = "|cff1BE5B8";
		break;
	case PLAYER_COLOR_PURPLE:
		ret = "|cff530080";
		break;
	case PLAYER_COLOR_YELLOW:
		ret = "|cffFFFC00";
		break;
	case PLAYER_COLOR_ORANGE:
		ret = "|cffFE890D";
		break;
	case PLAYER_COLOR_GREEN:
		ret = "|cff1FBF00";
		break;
	case PLAYER_COLOR_PINK:
		ret = "|cffE45AAF";
		break;
	case PLAYER_COLOR_LIGHT_GRAY:
		ret = "|cff949596";
		break;
	case PLAYER_COLOR_LIGHT_BLUE:
		ret = "|cff7DBEF1";
		break;
	case PLAYER_COLOR_AQUA:
		ret = "|cff0F6145";
		break;
	case PLAYER_COLOR_BROWN:
		ret = "|cff4D2903";
		break;
	default:
		ret = "|cffFFFFFF";
		break;
	}
	return ret;
}
