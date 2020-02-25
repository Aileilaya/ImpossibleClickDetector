#pragma once
enum color {
	PLAYER_COLOR_RED = 0, 
	PLAYER_COLOR_BLUE,
	PLAYER_COLOR_CYAN,
	PLAYER_COLOR_PURPLE,
	PLAYER_COLOR_YELLOW,
	PLAYER_COLOR_ORANGE,
	PLAYER_COLOR_GREEN,
	PLAYER_COLOR_PINK,
	PLAYER_COLOR_LIGHT_GRAY,
	PLAYER_COLOR_LIGHT_BLUE,
	PLAYER_COLOR_AQUA,
	PLAYER_COLOR_BROWN
};

struct mh {
	//bool			isBusy = false;
	unsigned int		player = 0;
	unsigned int		unit = 0;
	unsigned int	count = 0;
	unsigned int	showMsgCount = 0;
};
const char* GetPlayerColorString(int c);
namespace mhDetect {
	void init();
	void __stdcall detect();
	int isUnitVisible(unsigned int unit, unsigned int player, float x, float y);
	unsigned int getPlayerSelectedUnit(int slot);
}