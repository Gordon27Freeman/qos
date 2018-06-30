#ifndef IDT_H
#define IDT_H

namespace IDT
{
	void SetGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
	void Init();
}

#endif
