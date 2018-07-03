#ifndef GUI_H
#define GUI_H

#define CONTROL_BUTTON 0x01

namespace GUI
{
	void Update();
	void CreateWindow(const char *title, int x, int y, int w, int h);
	void DestroyWindow();
	void *AddControl(const char *title, char type);
}

#endif
