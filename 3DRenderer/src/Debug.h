#pragma once


namespace Debug
{
	inline void cls(void){
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD());
	}

	void console_cursor(bool flag);
}
