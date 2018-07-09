#include <gui.h>
#include <controls.h>
#include <string.h>
#include <mm.h>

int wx = 200, wy = 200;
struct TextBox *txt, *txt2;
struct CheckBox *chk;

void CreateWindowFunc()
{
	char *title = (char *)Memory::Alloc(1024);
	strcpy(title, txt2->text);
	window newwin = GUI::CreateWindow(title, wx, wy, 200, 100, chk->checked);
	if (!newwin) return;
	struct Label *lbl = (struct Label *)GUI::AddControl(newwin, CONTROL_LABEL);
	lbl->text = (char *)Memory::Alloc(1024);
	strcpy(lbl->text, txt->text);
	wx += 5;
	wy += 5;
	if(wx > 400) wx = 200;
	if(wy > 400) wy = 200;
}

extern "C" void kmain()
{
	window win1 = GUI::CreateWindow("QuantumOS", 80, 60, 172, 100, 0);
	
	chk = (struct CheckBox *)GUI::AddControl(win1, CONTROL_CHECKBOX);
	chk->y = 72;
	chk->label = "Resizable";
	chk->checked = 1;

	struct Label *lbl = (struct Label *)GUI::AddControl(win1, CONTROL_LABEL);
	lbl->text = "Title:";

	struct Label *lbl2 = (struct Label *)GUI::AddControl(win1, CONTROL_LABEL);
	lbl2->text = "Label:";
	lbl2->y = 24;

	txt = (struct TextBox *)GUI::AddControl(win1, CONTROL_TEXTBOX);
	txt->y = 24;
	txt->x = 54;
	txt->w = 112;
	Memory::Free(txt->text);
	txt->text = (char *)Memory::Alloc(65536);
	strcpy(txt->text, "Hello, World!");
	txt->length = strlen(txt->text);

	txt2 = (struct TextBox *)GUI::AddControl(win1, CONTROL_TEXTBOX);
	txt2->y = 0;
	txt2->x = 54;
	txt2->w = 112;
	Memory::Free(txt2->text);
	txt2->text = (char *)Memory::Alloc(65536);
	strcpy(txt2->text, "New Window");
	txt2->length = strlen(txt2->text);

	struct Button *btn = (struct Button *)GUI::AddControl(win1, CONTROL_BUTTON);
	btn->label = "Create Window";
	btn->w = 166;
	btn->y = 48;
	btn->onClick = CreateWindowFunc;

	while (1) GUI::Update();
}
