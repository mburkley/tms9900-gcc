
#define VDP_READ_DATA_REG   (*(volatile char*)0x8800)
#define VDP_WRITE_DATA_REG  (*(volatile char*)0x8C00)
#define VDP_ADDRESS_REG     (*(volatile char*)0x8C02)

#define VDP_READ_FLAG   0x00
#define VDP_WRITE_FLAG  0x40
#define VDP_REG_FLAG    0x80


static void vdp_copy_from_sys(int index, char* src, int size)
{
  volatile char* end = src + size;
  VDP_ADDRESS_REG = index | VDP_WRITE_FLAG;
  VDP_ADDRESS_REG = (char)(index >> 8);

  while(src != end)
    VDP_WRITE_DATA_REG = *src++;
}


void main()
{
  //                     12345678901234
  vdp_copy_from_sys(0,  "HELLO WORLD!",12);
  vdp_copy_from_sys(32, "THIS IS LINE 2", 14); 
  while(1); 
}
