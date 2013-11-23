#pragma once

#include "GlutWindow.h"

/**
* For singleton - Qt way.
*/
class GlutApp
{
public:
	GlutApp(int argc, _TCHAR* argv[], const char* appName = nullptr);
	~GlutApp(void);

	void exec();

	GlutWindow getMainWindow() const;

private:
	static GlutApp* self;

	int mainWindowId;
};

