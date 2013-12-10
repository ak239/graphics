#include "GL.h"
#include "Bar.h"


Bar::Bar(const string& name) : bar_(TwNewBar(name.c_str()))
{
}


Bar::~Bar(void)
{
	TwDeleteBar(bar_);
	for (auto it = vars_.begin(); it != vars_.end(); ++it)
		delete it->second;
}


void Bar::addButton(const string& name, TwButtonCallback cb)
{
	TwAddButton(bar_, name.c_str(), cb, nullptr, "");
}
