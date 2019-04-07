/*
                GRAPHICS                                TEXT

15-bit   7-bit    Resolution   Colors   15-bit   7-bit    Columns   Rows
mode     mode                           mode     mode
number   number                         number   number
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
100h     -        640x400      256      108h     -        80        60
101h     -        640x480      256
                                        109h     -        132       25
102h     6Ah      800x600      16       10Ah     -        132       43
103h     -        800x600      256      10Bh     -        132       50
                                        10Ch     -        132       60
104h     -        1024x768     16
105h     -        1024x768     256

106h     -        1280x1024    16
107h     -        1280x1024    256

VESA Super VGA Standard VS911022-8

10Dh     -        320x200      32K   (1:5:5:5)
10Eh     -        320x200      64K   (5:6:5)
10Fh     -        320x200      16.8M (8:8:8)
110h     -        640x480      32K   (1:5:5:5)
111h     -        640x480      64K   (5:6:5)
112h     -        640x480      16.8M (8:8:8)
113h     -        800x600      32K   (1:5:5:5)
114h     -        800x600      64K   (5:6:5)
115h     -        800x600      16.8M (8:8:8)
116h     -        1024x768     32K   (1:5:5:5)
117h     -        1024x768     64K   (5:6:5)
118h     -        1024x768     16.8M (8:8:8)
119h     -        1280x1024    32K   (1:5:5:5)
11Ah     -        1280x1024    64K   (5:6:5)
11Bh     -        1280x1024    16.8M (8:8:8)
*/

typedef struct VESA_INFO { 
	unsigned char  VESASignature[4]     __attribute__ ((packed));
	unsigned short VESAVersion          __attribute__ ((packed));
	unsigned long  OEMStringPtr         __attribute__ ((packed));
	unsigned char  Capabilities[4]      __attribute__ ((packed));
	unsigned long  VideoModePtr         __attribute__ ((packed));
	unsigned short TotalMemory          __attribute__ ((packed));
	unsigned short OemSoftwareRev       __attribute__ ((packed));
	unsigned long  OemVendorNamePtr     __attribute__ ((packed));
	unsigned long  OemProductNamePtr    __attribute__ ((packed));
	unsigned long  OemProductRevPtr     __attribute__ ((packed));
	unsigned char  Reserved[222]        __attribute__ ((packed));
	unsigned char  OemData[256]         __attribute__ ((packed));
} VESA_INFO;

typedef struct MODE_INFO {
	unsigned short ModeAttributes       __attribute__ ((packed));
	unsigned char  WinAAttributes       __attribute__ ((packed));
	unsigned char  WinBAttributes       __attribute__ ((packed));
	unsigned short WinGranularity       __attribute__ ((packed));
	unsigned short WinSize              __attribute__ ((packed));
	unsigned short WinASegment          __attribute__ ((packed));
	unsigned short WinBSegment          __attribute__ ((packed));
	unsigned long  WinFuncPtr           __attribute__ ((packed));
	unsigned short BytesPerScanLine     __attribute__ ((packed));
	unsigned short XResolution          __attribute__ ((packed));
	unsigned short YResolution          __attribute__ ((packed));
	unsigned char  XCharSize            __attribute__ ((packed));
	unsigned char  YCharSize            __attribute__ ((packed));
	unsigned char  NumberOfPlanes       __attribute__ ((packed));
	unsigned char  BitsPerPixel         __attribute__ ((packed));
	unsigned char  NumberOfBanks        __attribute__ ((packed));
	unsigned char  MemoryModel          __attribute__ ((packed));
	unsigned char  BankSize             __attribute__ ((packed));
	unsigned char  NumberOfImagePages   __attribute__ ((packed));
	unsigned char  Reserved_page        __attribute__ ((packed));
	unsigned char  RedMaskSize          __attribute__ ((packed));
	unsigned char  RedMaskPos           __attribute__ ((packed));
	unsigned char  GreenMaskSize        __attribute__ ((packed));
	unsigned char  GreenMaskPos         __attribute__ ((packed));
	unsigned char  BlueMaskSize         __attribute__ ((packed));
	unsigned char  BlueMaskPos          __attribute__ ((packed));
	unsigned char  ReservedMaskSize     __attribute__ ((packed));
	unsigned char  ReservedMaskPos      __attribute__ ((packed));
	unsigned char  DirectColorModeInfo  __attribute__ ((packed));
	unsigned long  PhysBasePtr          __attribute__ ((packed));
	unsigned long  OffScreenMemOffset   __attribute__ ((packed));
	unsigned short OffScreenMemSize     __attribute__ ((packed));
	unsigned char  Reserved[206]        __attribute__ ((packed));
} MODE_INFO;


int get_vesa_info();
int get_mode_info(int mode);
int find_vesa_mode(int w,int h);
int set_vesa_mode(int mode_number);
int set_vesa_mode_w_h(int w,int h);
void set_vesa_bank(int bank_number);
void copy_to_video_buffer(unsigned char *memory_buffer,int screen_size);

void putpixel_vesa_640x480(int x,int y,int color);