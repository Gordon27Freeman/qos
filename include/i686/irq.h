#ifndef IRQ_H
#define IRQ_H

#include <regs.h>

namespace IRQ
{
	void InstallHandler(int irq, void (*handler)(struct regs *r));
	void UninstallHandler(int irq);
	void Init();
}

#endif
