unsigned char inb(unsigned short port)
{
	unsigned char data;
	asm volatile("inb %1, %0" : "=a" (data) : "dN" (port));
	return data;
}

void outb(unsigned short port, unsigned char data)
{
	asm volatile("outb %1, %0" : : "dN" (port), "a" (data));
}
