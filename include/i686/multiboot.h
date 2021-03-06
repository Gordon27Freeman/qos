#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#define MULTIBOOT_HEADER_MAGIC 0x1BADB002
#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

typedef struct
{
	unsigned int size;
	unsigned int base_addr;
	unsigned int base_addr_high;
	unsigned int length;
	unsigned int length_high;
	unsigned int type;
} mmap_field;

typedef struct module
{
	unsigned int start;
	unsigned int end;
	char * name;
	int reserved;
} module;

typedef struct
{
	unsigned int flags;
	unsigned int mem_lower;
	unsigned int mem_upper;
	unsigned int boot_device;
	char *cmdline;
	unsigned int mods_count;
	module *mods_addr;
	unsigned int syms[4];
	unsigned int mmap_length;
	mmap_field *mmap_addr;
	unsigned int drives_length;
	unsigned int *drives_addr;
	unsigned int config_table;
	char *bootloader_name;
	unsigned int apm_table;
	unsigned int vbe_control_info;
	unsigned int vbe_mode_info;
	unsigned short vbe_mode;
	unsigned short vbe_interface_seg;
	unsigned short vbe_interface_off;
	unsigned short vbe_interface_len;
	unsigned long long framebuffer_addr;
	unsigned int framebuffer_pitch;
	unsigned int framebuffer_width;
	unsigned int framebuffer_height;
	unsigned char framebuffer_bpp;
	unsigned char framebuffer_type;
	unsigned char color_info[6];
} multiboot_header;

#endif
