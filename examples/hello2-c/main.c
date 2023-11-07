/* 
Define macros to access VDP registers

These macros can be used as if they were variables. 
This makes things easier to read and we don't have a lot of
typecasts troughout the code.

  Read data register located at address >8800   
  Write data register located at address >8C00   
  Address register located at address >8C02
*/  
#define VDP_READ_DATA_REG   (*(volatile unsigned char*)0x8800)
#define VDP_WRITE_DATA_REG  (*(volatile unsigned char*)0x8C00)
#define VDP_ADDRESS_REG     (*(volatile unsigned char*)0x8C02)

/* Location of the screen in VDP memory.
   This is the based on the VDP configuration set by the TI firmware  */
#define VDP_SCREEN_ADDRESS  0


/*==========================================================================
 *           vdp_copy_from_sys
 *==========================================================================
 * Description: Copy data from system memory to VDP memory
 *
 * Parameters : index - Starting index into VDP memory
 *              src   - Address of source data in system memory
 *              size  - Number of bytes to copy
 *
 * Returns    : Nothing
 */
static void vdp_copy_from_sys(unsigned int index, char* src, int size)
{
  /* Find the address of the end of the source data */
  volatile char* end = src + size;

  /* Set the address in VDP to which we will copy.
     We must do this in two parts.

     The first part: Write the low byte of the VDP address
     and include a flag informing VDP we will soon be writing data */
  VDP_ADDRESS_REG = index;

  /* The second part: Write the high byte of the VDP address.
     After this, the VDP will be ready to have data written into 
     it's memory */
  VDP_ADDRESS_REG = (index >> 8) & 0xFF;

  /* Copy data to VDP memory via VDP data write register 
     Keep copying until we hit the end of the source data */
  while(src < end)
    VDP_WRITE_DATA_REG = *src++;
}


/*==========================================================================
 *                            display_at
 *==========================================================================
 * Description: Display a text string at a specified location on the screen
 *
 * Parameters : row    - Screen row for display
 *              column - Screen column for display
 *              text   - Text to display
 *
 * Returns    : Nothing
 */
static void display_at(unsigned int row, unsigned int column, char* text)
{
  int size;          /* Size of text string to display */
  int offset;        /* Offset into screen buffer */
  char *ptr = text;  /* Pointer to a character in "text" */

  /* Find length of display text 
     C strings are ended by a zero byte, so look for that */
  size = 0;
  while(text[size] != 0) size++;
  
  /* Convert row and column to screen offset. 
     There are 32 columns per row */
  offset = row * 32 + column;

  /* Copy text to the screen, which lies in VDP memory */
  vdp_copy_from_sys(offset + VDP_SCREEN_ADDRESS, text, size);
}


/*==========================================================================
 *                                 main
 *==========================================================================
 * Description: Entry point for program
 *
 * Parameters : None
 *
 * Returns    : Nothing
 */
void main()
{
  /* Display flashing text at the center of the screen
     We will do this by displaying text, then spaces, then text again.
     To keep things simple, we will use the character set defined by the
     TI firmware. Unfortunately, it only defines symbols and uppercase
     letters. Keep that in mind if you change the displayed text

     This loop will never exit. Since the interrupts have been disabled
     in the setup code, the console must be reset to exit the loop */
  while(1)
  {
    display_at(12, 10, "HELLO WORLD!");
    display_at(12, 10, "            ");
  }
}
