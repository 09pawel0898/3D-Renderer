#include "pch.h"
#include "App.h"
#include "Utility.h"

int main(void)
{
	Utility::console_cursor(false);
	App app;
	app.run();
	exit(EXIT_SUCCESS);
}
