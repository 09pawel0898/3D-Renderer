#include "pch.h"
#include "App.h"
#include "Math/vector.h"
#include "Debug.h"


int main(void)
{
	Debug::console_cursor(false);
	App app;
	app.run();
	exit(EXIT_SUCCESS);
}
