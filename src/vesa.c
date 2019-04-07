#include <dpmi.h>
#include <go32.h>
#include <sys/farptr.h>

#include "vesa.h"

VESA_INFO vesa_info;
MODE_INFO mode_info;

int get_vesa_info() {
	__dpmi_regs r;
	long dosbuf;
	int c;
	
	dosbuf=__tb & 0xFFFFF;

	for (c=0; c<sizeof(VESA_INFO); c++)
		_farpokeb(_dos_ds, dosbuf+c, 0);

	dosmemput("VBE2", 4, dosbuf);

	r.x.ax = 0x4F00;
	r.x.di = dosbuf & 0xF;
	r.x.es = (dosbuf>>4) & 0xFFFF;
	__dpmi_int(0x10, &r);

	if (r.h.ah)
		return -1;

	dosmemget(dosbuf, sizeof(VESA_INFO), &vesa_info);

	if (strncmp((const char *)(vesa_info.VESASignature),"VESA", 4) != 0)
		return -1;

	return 0;
}

int get_mode_info(int mode) {
	__dpmi_regs r;
	long dosbuf;
	int c;

	dosbuf = __tb & 0xFFFFF;

	for (c=0; c<sizeof(MODE_INFO); c++)
		_farpokeb(_dos_ds, dosbuf+c, 0);

	r.x.ax = 0x4F01;
	r.x.di = dosbuf & 0xF;
	r.x.es = (dosbuf>>4) & 0xFFFF;
	r.x.cx = mode;
	__dpmi_int(0x10, &r);

	if (r.h.ah)
		return -1;

	dosmemget(dosbuf, sizeof(MODE_INFO), &mode_info);

	return 0;
}

int find_vesa_mode(int w,int h) {
	int mode_list[256];
	int number_of_modes;
	long mode_ptr;
	int c;

	if (get_vesa_info()!=0)
		return 0;

	mode_ptr=((vesa_info.VideoModePtr & 0xFFFF0000) >> 12)+(vesa_info.VideoModePtr & 0xFFFF);

	number_of_modes=0;

	while (_farpeekw(_dos_ds, mode_ptr) != 0xFFFF) {
		mode_list[number_of_modes] = _farpeekw(_dos_ds, mode_ptr);
		number_of_modes++;
		mode_ptr += 2;
	}

	for (c=0; c<number_of_modes; c++) {
		if (get_mode_info(mode_list[c]) != 0)
			continue;
		if ((mode_info.ModeAttributes & 0x19) != 0x19)
			continue;
		if ((mode_info.XResolution != w) || (mode_info.YResolution != h))
			continue;
		if (mode_info.NumberOfPlanes != 1)
			continue;
		if (mode_info.MemoryModel != 4)
			continue;
		if (mode_info.BitsPerPixel != 8)
			continue;
		return mode_list[c];
	}

	return 0; 
}

int set_vesa_mode(int mode_number) {
	__dpmi_regs r;
	
	if (!mode_number)
		return -1;

	r.x.ax = 0x4F02;
	r.x.bx = mode_number;
	__dpmi_int(0x10, &r);
	if (r.h.ah)
		return -1;

	return 0;
}

int set_vesa_mode_w_h(int w,int h) {
	__dpmi_regs r;
	int mode_number;

	mode_number = find_vesa_mode(w, h);
	if (!mode_number)
		return -1;

	r.x.ax = 0x4F02;
	r.x.bx = mode_number;
	__dpmi_int(0x10, &r);
	if (r.h.ah)
		return -1;

	return 0;
}

void set_vesa_bank(int bank_number) {
	__dpmi_regs r;
	r.x.ax = 0x4F05;
	r.x.bx = 0;
	r.x.dx = bank_number;
	__dpmi_int(0x10, &r);
}

void copy_to_video_buffer(unsigned char *memory_buffer,int screen_size) {
	int bank_size = mode_info.WinSize*1024;
	int bank_granularity = mode_info.WinGranularity*1024;
	int bank_number = 0;
	int todo = screen_size;
	int copy_size;

	while (todo > 0) {
		set_vesa_bank(bank_number);
		
		if (todo > bank_size)
			copy_size = bank_size;
		else
			copy_size = todo;

		dosmemput(memory_buffer, copy_size, 0xA0000);
		todo -= copy_size;
		memory_buffer += copy_size;
		bank_number += bank_size/bank_granularity;
	}
}

void putpixel_vesa_640x480(int x,int y,int color) {
	int address = y*640+x;
	int bank_size = mode_info.WinGranularity*1024;
	int bank_number = address/bank_size;
	int bank_offset = address%bank_size;

	set_vesa_bank(bank_number);

	_farpokeb(_dos_ds, 0xA0000+bank_offset, color);
}