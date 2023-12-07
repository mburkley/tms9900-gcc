/******************************************************************************

*
*  NOT USED.  Was before I foundi LIB2FUNCS_EXCLUDE 

*                               __addM3M2
*******************************************************************************
* Return the number of leading 0-bits in a value, starting at the most
* significant bit position.  If the value is zero, the result is undefined.
*
// double addition -- method normally included in GCC lib
* Inputs : r1 - Pointer to src
*          r2 - Pointer to dst
*
* Returns: r1 - Pointer to res
******************************************************************************/

#ifdef L_adddf3
/*********************************************************/
/*                     __adddf3                          */
/*********************************************************/
  def __adddf3

#define FAC 0x834A
#define ARG 0x835C

  li r3,8
  li r4, FAC
__adddf3_cpy_src:
  movb *r1+,*r4+
  dec r3
  jne __adddf3_cpy_src
  li r3,8
  li r4, ARG
__adddf3_cpy_dst:
  movb *r1+,*r4+
  dec r3
  jne __adddf3_cpy_dst
  lwpi >83E0
  bl @>0D80
  lwpi >8300

  li r3,8
  li r4, FAC
__adddf3_cpy_res:
  movb *r4+,*r1+
  dec r3
  jne __adddf3_cpy_res
  b    *r11
#endif

#ifdef L_muldi3
/*********************************************************/
/*                     __muldi3                          */
/*********************************************************/
  def __muldi3
  ; TODO
  b    *r11
#endif

