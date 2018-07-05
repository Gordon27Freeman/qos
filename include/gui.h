#ifndef GUI_H
#define GUI_H

#define CONTROL_LABEL 0x01
#define CONTROL_BUTTON 0x02
#define CONTROL_TEXTBOX 0x03

typedef unsigned int window;

namespace GUI
{
	void Update();
	window CreateWindow(const char *title, int x, int y, int w, int h);
	void DestroyWindow();
	void *AddControl(window win, char type);
}

#endif
