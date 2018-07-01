#ifndef GUI_H
#define GUI_H

namespace GUI
{
	void Update();
	void CreateWindow(const char *title, int x, int y, int w, int h);
	void DestroyWindow(const char *title);
	void MousePress(int Button, int MouseX, int MouseY);
	void MouseRelease(int Button, int MouseX, int MouseY);
}

#endif
