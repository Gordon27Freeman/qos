global IDT_Load
extern idtp
IDT_Load:
	lidt [idtp]
	ret
