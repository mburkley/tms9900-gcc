/*
 *  MGB 32-bit arithmetic functions never seem to be used.  The compiler emits
 *  sequences of 16-bit ops to perform 32-bit ops.  An exception is it does look
 *  for __ashlsi3 and __ashrsi3 when built to optimise for size (-Os) so these
 *  have been added here.
 */

/******************************************************************************
*                               __clzM2
*******************************************************************************
* Return the number of leading 0-bits in a value, starting at the most
* significant bit position.  If the value is zero, the result is undefined.
*
* Inputs : r1 - Value to test
*
* Returns: r1 - Number of zero bits
******************************************************************************/

#ifdef L_clzqi2
/*********************************************************/
/*                     __clzqi2                          */
/*********************************************************/
  def __clzqi2
__clzqi2:
  mov  r2, @-2(r10)     /* Scratch reg */
  movb r1, r2  /* Move value into position, set condition flags */
  clr  r1      /* Set minimum bit count, flags unchanged */
  jgt  do_clz  /* If val==0, return undefined, if val<0, MSB set, return 0*/
  mov  @-2(r10), r2
  b    *r11
do_clz:
  b    @__clz_nosave
#endif


#ifdef L_clzhi2
/*********************************************************/
/*                     __clzhi2                          */
/*********************************************************/
  def __clzhi2
__clzhi2:
  mov  r2, @-2(r10)     /* Scratch reg */
  mov  r1, r2  /* Move value into position, set condition flags */
  clr  r1      /* Set minimum bit count, flags unchanged */
  jgt  do_clz  /* If val==0, return undefined, if val<0, MSB set, return 0*/
  mov  @-2(r10), r2
  b *r11
do_clz:
  b    @__clz_nosave
#endif


#ifdef L_clzsi2
/*********************************************************/
/*                     __clzsi2                          */
/*********************************************************/
  def __clzsi2
__clzsi2:
  mov  r2, @-2(r10)     /* Scratch reg */
  mov  r1, r1   /* Test MSW for set bits */
  jlt  ret_0    /* Upper bit of MSW set, return zero */
  jgt  clz_msw  /* Count leading zeroes in MSW */

clz_lsw:
  li   r1, 16   /* MSW was zero, initialize count to 16 */
  mov  r2, r2   /* Test bits in LSW */
  jlt  ret_16   /* Upper bit of LSW set, return sisxteen */
  jgt  do_clz   /* Count leading zeroes of LSW */

ret_0:
  clr  r1
ret_16:
  mov  @-2(r10), r2
  b    *r11

clz_msw:
  mov  r1, r2  /* Move MSW into test position */
  clr  r1      /* Initialize count to zero */
do_clz:
  b    @__clz_nosave
#endif


#ifdef L_clz
/******************************************************************************
*                               __clz
*******************************************************************************
* Return the number of leading 0-bits in a 16-bit value, starting at the most 
* significant bit position.  If the value is zero, the result is undefined.
*
* Inputs : R1 - Minimum posible bit count
*          R2 - Value to test
*
* Returns: R1 - Number of zero bits
*******************************************************************************/
  def __clz
/*
* We will shift left until we find a set bit
*
* Example 1, zero leading bits:
*   C Val   N
*   - ----  -
*   . 1000  0
*
* Example 2, one leading bit:
*   C Val   N
*   - ----  -
*   . 0100  4
*   1 00..  2->1
*
* Example 3, two leading bits:
*   C Val   N
*   - ----  -
*   0 0010  4
*   0 10..  2
*
* If we got here, there is at least one bit set in supplied value
*/
__clz:
  mov  r2, @-2(r10)     /* Scratch reg */
__clz_nosave:
  inct r1      /* Assume two zero bits, double count implied bit */
  sla  r2, 2   /* Move next two bits into test position, set flags */
  joc  bottom  /* Upper test bit set, exit */
  jgt  __clz   /* Upper and lower test bits clear, loop */
  
  /* Clean up after loop */
  inc r1       /* If we got here, negate next instruction */
bottom:
  dec r1       /* No cleared bits in last test set, decrement count */

  /* Bit count is in R1, exit */
  mov  @-2(r10), r2
  b *r11
#endif

  
/******************************************************************************
*                               __ctzM2
*******************************************************************************
* Return the number of trailing 0-bits in a 16-bit value, starting at the least
* significant bit position.  If the value is zero, the result is undefined.
*
* Inputs : R1 - Value to test
*
* Returns: R1 - Number of zero bits
******************************************************************************/

#ifdef L_ctzqi2
/*********************************************************/
/*                     __ctzqi2                          */
/*********************************************************/
  def __ctzqi2
__ctzqi2:
  clr  r2
  movb r1, r2  /* Move value to R2, clearing low bits */
  jeq  bottom  /* If all bits clear, stop now, return zero */
  li   r1, 8   /* Max possible zero bits */
  b    @__ctz  /* Examine provided value */
bottom:
  mov  @-2(r10), r2
  b *r11
#endif


#ifdef L_ctzhi2
/*********************************************************/
/*                     __ctzhi2                          */
/*********************************************************/
  def __ctzhi2
__ctzhi2:
  mov  r2, @-2(r10)     /* Scratch reg */
  mov r1, r2  /* Move value to R2, set condition flags */
  jeq bottom  /* If all bits clear, stop now, return zero */
  li  r1, 16  /* Max possible zero bits */
  b   @__ctz_nosave  /* Examine provided value */
bottom:
  mov  @-2(r10), r2
  b *r11
#endif


#ifdef L_ctzsi2
/*********************************************************/
/*                     __ctzsi2                          */
/*********************************************************/
  def __ctzsi2
__ctzsi2:
  mov  r2, @-2(r10)     /* Scratch reg */
  mov  r2, r2     /* Check low word for set bits */
  jeq  lsw_clear 

  /* There are set bits in the low word */
  li   r1, 16     /* Maximum number of possible zero bits */
  b    @__ctz_nosave     /* Examine provided value */

  /* All bits clear in the low word, no need to test them */
lsw_clear:
  mov  r1, r2     /* Move high word into test position */
  jeq  bottom     /* If all bits clear, stop now, return zero */
  li   r1, 32     /* Maximum number of possible zero bits */
  b    @__ctz_nosave     /* Examine provided value */

bottom:
  mov  @-2(r10), r2
  b *r11
#endif


#if L_ctz
/******************************************************************************
*                               __ctz
*******************************************************************************
* Return the number of trailing 0-bits in a 16-bit value, starting at the least 
* significant bit position.  If the value is zero, the result is undefined.
*
* Inputs : R1 - Maximum posible bit count
*          R2 - Value to test
*
* Returns: R1 - Number of zero bits
******************************************************************************/
  def __ctz

/*
* We will shift left until all leading set bits are shifted out of the value
*
* Exmaple 1, zero trailing bits:
*   C Val   N
*   - ----  -
*   . 0001  4
*   0 01..  2
*   1 ....  0
*
* Exmaple 2, one trailing bit:
*   C Val   N
*   - ----  -
*   . 0010  4
*   0 10..  2
*   0 ....  1
*
* Exmaple 3, two trailing bits:
*   C Val   N
*   - ----  -
*   0 0100  4
*   1 00..  2
*/

  /* Test loop, check two uppermost bits at a time */
__ctz:
  mov  r2, @-2(r10)     /* Scratch reg */
__ctz_nosave:
  dect r1      /* Assume both bits are set */
  sla  r2, 2   /* Shift two bits into test position */
  jne  __ctz_nosave   /* Still have set bits, keep looping */

  /* Correct for last two test bits */
  joc  bottom  /* If carry bit set, lower test bit was set */
  inc  r1      /* lower test bit was clear, increment bit count */
bottom:
  mov  @-2(r10), r2
  b *r11
#endif


/******************************************************************************
*                               __ffsM2
*******************************************************************************
* Return the index of the least significant set bit in a value, or zero 
* if the value is zero.  The least significant bit is index one.
*
* Inputs : R1 - Value to test
*
* Returns: R1 - Index to lowest set bit
******************************************************************************/


#ifdef L_ffsqi2
/*********************************************************/
/*                     __ffsqi2                          */
/*********************************************************/
  def __ffsqi2
__ffsqi2:
  dect r10
  mov  r11,*r10
  movb r1, r1     /* Check for zero value */
  jeq  done       /* If so, exit now */
  bl   @__ctzqi2  /* Count trailing zero bits */
  inc  r1         /* Least sig. bit is in position trailing_count +1 */
  jmp  ffsqi2_ret
done:
  clr  r1         /* Return zero value */
ffsqi2_ret:
  mov  *r10+, r11
  b    *r11
#endif


#ifdef L_ffshi2
/*********************************************************/
/*                     __ffshi2                          */
/*********************************************************/
  def __ffshi2
__ffshi2:
  dect r10
  mov  r11,*r10
  mov r1, r1      /* Check for zero value */
  jeq done        /* If so, exit now */
  bl  @__ctzhi2   /* Count trailing zero bits */
  inc r1          /* Least sig. bit is in position trailing_count +1 */
done:
  mov  *r10+, r11
  b    *r11
#endif


#ifdef L_ffssi2
/*********************************************************/
/*                     __ffssi2                          */
/*********************************************************/
  def __ffssi2
__ffssi2:
  dect r10
  mov  r11,*r10
  mov r1, r0
  soc r2, r0      /* Check for zero value */
  jeq done        /* If so, exit now */
  bl  @__ctzsi2   /* Count trailing zero bits */
  inc r1          /* Least sig. bit is in position trailing_count +1 */
done:
  mov  *r10+, r11
  b    *r11
  ; def __modsi3 ; ???
#endif


#ifdef L_parity
/******************************************************************************
*                               __parityM2
*******************************************************************************
* Return the value zero if the number of bits set in the given value is even,
* and the value one otherwise.
*
* Inputs : R1 - Value to test
*
* Returns: R1 - Number of zero bits
******************************************************************************/

/* Test an 8-bit value */
  def __parityqi2
__parityqi2:
  dect r10
  mov  r3,*r10  ; save r3
  seto r3
  jmp byte1

/* Test a 16-bit value */
  def __parityhi2
__parityhi2:
  dect r10
  mov  r3,*r10  ; save r3
  clr r3
  jmp byte2

/* Test a 32-bit value */
  def __paritysi2
__paritysi2:
  dect r10
  mov  r3,*r10  ; save r3
  clr  r3
  movb r2, r2
  jop pre_byte3
  inv r3
pre_byte3:
  swpb r2
byte3:
  movb r2, r2
  jop byte2
  inv r3
byte2:
  movb r1, r1
  jop pre_byte1
  inv r3
pre_byte1:
  swpb r1
byte1:
  movb r1, r1
  jop done
  inv r3
done:
  neg r3
  mov r3, r1
  mov  *r10+, r3
  bl *r11
#endif


/******************************************************************************
*                               __popcountM2
*******************************************************************************
* Return the number of set bits in a value
*
* Inputs : R1 - Value to test
*
* Returns: R1 - Number of set bits
******************************************************************************/


#ifdef L_popcountqi2
/*********************************************************/
/*                   __popcountqi2                       */
/*********************************************************/
  def __popcountqi2
__popcountqi2:
  dect r10
  mov  r2,*r10  ; save r2
  dect r10
  mov  r11,*r10  ; save r11
  clr  r2           /* Clear lower unused bits */
  movb r1, r2       /* Move value into test position */
  clr  r1           /* Clear bit count */
  bl   @__popcount  /* Find set bit count */
  mov  *r10+, r11
  mov  *r10+, r3
  b    *r11
#endif


#ifdef L_popcounthi2
/*********************************************************/
/*                   __popcounthi2                       */
/*********************************************************/
  def __popcounthi2
__popcounthi2:
  dect r10
  mov  r2,*r10  ; save r2
  dect r10
  mov  r11,*r10  ; save r11
  mov  r1, r2       /* Move value into test position */
  clr  r1           /* Clear bit count */
  bl   @__popcount  /* Find set bit count */
  mov  *r10+, r11
  mov  *r10+, r3
  b    *r11
#endif


#ifdef L_popcountsi2
/*********************************************************/
/*                   __popcountsi2                       */
/*********************************************************/
  def __popcountsi2
__popcountsi2:
  dect r10
  mov  r3,*r10  ; save r3
  dect r10
  mov  r11,*r10  ; save r11
  mov  r1, r3       /* Move MSW to safe position */
  clr  r1           /* Clear bit count */
  bl   @__popcount  /* Find LSW set bit count */
  mov  r3, r2       /* Move MSW into test position */
  bl   @__popcount  /* Find MSW set bit count */
  mov  *r10+, r11
  mov  *r10+, r3
  b    *r11
#endif


#ifdef L_popcount
/******************************************************************************
*                               __popcount
*******************************************************************************
* Return the number of set bits in a 16-bit value
*
* Inputs : R1 - Current bit count
*          R2 - Value to test
*
* Returns: R1 - Number of set bits
******************************************************************************/
  def __popcount
__popcount:
  mov  r2,r2   /* Check for zero value */  
  jeq  done    /* If zero, exit now */
top:
  inc  r1      /* Increment bit count */
  mov  r2, r0  /* \                           */
  neg  r0      /* | Equvilent to r2 &= (r2-1) */
  szc  r0, r2  /* /                           */
  jne  top     /* Keep looping until all bits counted */
done:
  b    *r11    /* Return to caller */
#endif


#ifdef L_divmodsi3
/******************************************************************************
*                               __divmodsi3
*******************************************************************************
* Calculate the signed quotient and modulus of the two values provided
*
* Inputs:  [R1,R2] - 32-bit numerator
*          [R3,R4] - 32-bit denominator
*           R5     - Address for modulus
*
* Returns: [R1,R2] - 32-bit quotient
*
* TODO - is this a library function?  Why pointer in r5?  Disabled for now
******************************************************************************/
  def __divmodsi3
__divmodsi3:
  /* Save return register and r6 */
  dect r10
  mov  r11,*r10
  dect r10
  mov  r5,*r10

  bl   @__divmodstart

  /* Caclulate result */
calc:
  bl   @__udivmod32

  /* Negate modulus if needed */
  mov  r5, r0
  jlt  savemod
  inv  r3
  neg  r4
  jnc  savemod
  inc  r3
  
  /* Save modulus */
savemod:
  mov  *r10, r0
  mov  r3, *r0+
  mov  r4, *r0

  /* Restore from stack */
  mov  *r10+, r5
  mov  *r10+, r11

  /* Complete operatons */
  b    @__divmodend
#endif


#ifdef L_divsi3
/******************************************************************************
*                               __divsi3
*******************************************************************************
* Calculate the signed quotient of the two values provided
*
* Inputs:  [R1,R2] - 32-bit numerator
*          [R3,R4] - 32-bit denominator
*
* Returns: [R1,R2] - 32-bit quotient
******************************************************************************/
  def __divsi3
__divsi3:
  /* Save return register */
  dect r10
  mov  r11,*r10  ; save r11
  dect r10
  mov  r5,*r10  ; save r5
  bl   @__divmodstart

  /* Caclulate result */
calc:
  bl   @__udivmod32

  /* Restore from stack */
  mov  *r10+, r5
  mov  *r10+, r11

  b    @__divmodend
#endif


#ifdef L_modsi3
/******************************************************************************
*                               __modsi3
*******************************************************************************
* Calculate the signed modulus of the two values provided
*
* Inputs:  [R1,R2] - 32-bit numerator
*          [R3,R4] - 32-bit denominator
*
* Returns: [R1,R2] - 32-bit modulus
******************************************************************************/
  def __modsi3
__modsi3:
  /* Save return register */
  dect r10
  mov  r11,*r10  ; save r11
  dect r10
  mov  r5,*r10  ; save r5
  bl   @__divmodstart

  /* Caclulate result */
calc:
  bl   @__udivmod32
  mov  r3, r1
  mov  r4, r2

  /* Restore from stack */
  mov  *r10+, r5
  mov  *r10+, r11

  b    @__divmodend
#endif


#ifdef L_divmod_common
/******************************************************************************
*                               __divmodstart
*******************************************************************************
* Common code for the start of all signed division and modulo calculations
*
* Inputs:  [R1,R2] - Signed 32-bit numerator
*          [R3,R4] - Signed 32-bit denominator
*
* Returns: [R1,R2] - Positive 32-bit quotient
*          [R3,R4] - Positive 32-bit denominator
*          [R5]    - Sign of result
******************************************************************************/
  def __divmodstart
__divmodstart:
  /* Make numerator positive */
  mov  r1, r5
  jlt  negnum
  jmp  testden
negnum:
  inv  r1
  neg  r2
  jnc  testden
  inc  r1

  /* Make denominator positive */
testden:
  xor  r3, r5
  mov  r3, r3
  jlt  negden
  jmp  done
negden:
  inv  r3
  neg  r4
  jnc  done
  inc  r3
done:
  b    *r11


/******************************************************************************
*                               __divmodend
*******************************************************************************
* Common code for the end of all signed division and modulo calculations
*
* Inputs:  [R1,R2] - Positive 32-bit quotient
*          [R3,R4] - Positive 32-bit denominator
*          [R5]    - Sign of result
*          
* Returns: [R1,R2] - Signed 32-bit result
******************************************************************************/
  def __divmodend
__divmodend:

  /* Do we need to negate the result? */
  mov  r5, r0
  jlt  makeneg  
  b    *r11        # Nope, exit now

  /* Negate result and return */
makeneg:  
  inv  r1
  neg  r2
  jnc  jmp1
  inc  r1
jmp1:
  b    *r11
#endif


#ifdef L_udivmodsi3
/******************************************************************************
*                             __udivmodsi3
*******************************************************************************
* Calculate the unsigned quotient and remainder of the two values provided
*
* Inputs:  [R1,R2] - 32-bit numerator
*          [R3,R4] - 32-bit denominator
*           R5     - Address to place 32-bit remainder
*
* Returns: [R1,R2] - 32-bit quotient
******************************************************************************/
  def __udivmodsi3
__udivmodsi3:
  /* Save pointer and return address */
  dect r10
  mov  r11,*r10  ; save r11
  dect r10
  mov  r5,*r10  ; save r5

  /* Do some math */
  bl   @__udivmodsi
  
  /* Save remainder */
  mov  *r10, r0
  mov  r3, *r0+
  mov  r4, *r0

  mov  *r10+, r5
  mov  *r10+, r11
  b    *r11
#endif


#ifdef L_udivsi3
/******************************************************************************
*                               __udivsi3
*******************************************************************************
* Calculate the unsigned quotient of the two values provided
*
* Inputs:  [R1,R2] - 32-bit numerator
*          [R3,R4] - 32-bit denominator
*
* Returns: [R1,R2] - 32-bit quotient
******************************************************************************/
  def __udivsi3
__udivsi3:
  /* Fall through to next routine */


/******************************************************************************
*                               __udivmod32
*******************************************************************************
* Calculate the unsigned quotient and remainder of the two values provided.
* This is used by all the 32-bit division and modulus functions.
*
* Inputs:  [R1,R2] - 32-bit numerator
*          [R3,R4] - 32-bit denominator
*
* Returns: [R1,R2] - 32-bit quotient
*          [R3,R4] - 32-bit remainder
******************************************************************************/
  def __udivmod32
__udivmod32:
  ai   r10, -10
  mov  r10, r0
  mov  r5, *r0+
  mov  r6, *r0+
  mov  r7, *r0+
  mov  r8, *r0+
  mov  r12, *r0+

  /* Check size of denominator */
  mov  r3, r3      /* Is the upper word of denominator used? */
  jne  den32       /* If so, jump to 32-bit code */

  /* Handle 16-bit denominator */
  /* Zero extend numerator */
  clr  r0
  mov  r1, r1
  jeq  num16       /* If numerator is only 16 bits, skip the first DIV */

  /* Perform calculation */
  div  r4, r0
num16:
  div  r4, r1

  /* Move results into return position */
  mov  r2, r4      /* LSW of remainder */
                   /* MSW of remainder still zero */
  mov  r1, r2      /* LSW of result */
  mov  r0, r1      /* MSW of result */
  jmp  jmp4

den32:
  /*
  * Handle 32-bit denominator
  *
  *      A1*N + A1
  * Q = ---------
  *      B1*N + B2
  *
  * N = 0x10000
  *
  * Divide top and bottom by 2*B1 to make the denominator a 16-bit quantity
  *
  *        A1*N + A2
  *       ---------
  *          2*B1
  * Q = ---------------
  *        N     B2
  *       --- + ---
  *        2    2*B1
  *
  * This will result in some rounding error which must be corrected for.
  * We will call the approximate value P, and the rounding error E. 
  *
  * Q - P = E
  *
  * Analysis of the error shows that E must be either 0 or -1. We will calculate
  * the remainder to determine E and so correct P.
  *
  * R = (A1*N + A2) - P*(B1*N + B2)
  *
  * If R is negative, P is too large by one and we will apply the correction.
  * Otherwise, P is correct and we can use the value directly.
  */

  /* Move arguments into test position */
  mov  r1, r0      /* [r0,r12] <- [A1,A2] = numerator */
  mov  r2, r12

  mov  r3, r7      /* [r7,r8] <- [B1,B2] = deominator */
  mov  r4, r8

  /* Calculate V = (N/2 + B2/(2*B1)) */
  clr  r1          /* [r1,r2] <- [C1,C2] = B2/2 */
  mov  r8, r2
  srl  r2, 1

  div  r7, r1      /* r1 <- v = [C1,C2]/B1 */
  ai   r1, 0x8000  /* v += 0x8000 */

  /* Calculate U = (A1*N + A2)/(2*B1) */
  mov  r0, r2      /* [r2,r3] <- [U1,U2] = [A1,A2] */
  mov  r12, r3

  srl  r2, 1       /* [U1,U2] = [A1,A2]/2 */
  srl  r3, 1
  jnc  jmp1
  ori  r3, 0x8000
jmp1:

  clr  r4          /* [U1,U2] = [U1,U2]/B1 */
  mov  r2, r5
  div  r7, r4
  mov  r3, r6
  div  r7, r5
  mov  r4, r2
  mov  r5, r3

  div  r1, r2      /* r2 <- P = [U1,U2]/V */

  /* Calculate remainder  [m1,m2]=[a1,a2]-[b1,b2]*p */
  mov  r7, r3      /* [r3,r4] <- [U1,U2] = B1*p */
  mpy  r2, r3

  mov  r8, r5      /* [r5,r6] <- [D1,D2] = B2*P */
  mpy  r2, r5

  a    r4, r5      /* [D1,D2] += [U2,0] */

  mov  r0, r3      /* [M1,M2] = [A1,A2] */
  mov  r12, r4

  s    r5, r3      /* [M1,M2] -= [D1,D2] */
  s    r6, r4
  joc  jmp2
  dec  r3
jmp2:

  /*
  * Results now in return position, prepare for exit
  *   [r1,r2] <- quotient
  *   [r3,r4] <- remainder
  */
  clr  r1          /* Set upper word of quotient, we know it will be zero */
  mov  r3, r3      /* if(remainder >= 0) P is correct */
  jlt  jmp3
  jmp  jmp4
jmp3:

  /* Correct result for rounding error */
  dec  r2          /* P -= 1 */

  /* Correct remainder for rounding error */
  a    r7, r3      /* [M1,M2] += [B1,B2] */
  a    r8, r4
  jnc  jmp4
  inc  r3
jmp4:

  /* Return value */
  mov  *r10+, r5
  mov  *r10+, r6
  mov  *r10+, r7
  mov  *r10+, r8
  mov  *r10+, r12
  b    *r11
#endif


#ifdef L_umodsi3
/******************************************************************************
*                               __umodsi3
*******************************************************************************
* Calculate the unsigned modulus of the two values provided
*
* Inputs:  [R1,R2] - 32-bit numerator
*          [R3,R4] - 32-bit denominator
*
* Returns: [R1,R2] - 32-bit modulus
******************************************************************************/
  def __umodsi3
__umodsi3:
  dect r10
  mov  r11,*r10  ; save r11
  bl   @__udivmod32
  mov  r3, r1
  mov  r4, r2
  mov  *r10+, r11
  b    *r11
#endif


#ifdef L_ashlsi3
/******************************************************************************
*                               __ashlsi3
*******************************************************************************
* Arithmetic left shift of 32 bit value
*
* Inputs:  [R1,R2] - 32-bit value
*          [R3]    - Shift count
*
* Returns: [R1,R2] - 32-bit result
******************************************************************************/
  def __ashlsi3
__ashlsi3:
  mov  r15, @-2(r10)     /* Scratch reg. No need to modify SP, not used and no siblings */

  /* Variable shift */
  mov  r3, r0
  ci   r0, 16
  jlt  ashllt16
  jeq  ashleq16
     
  /* Shift count greater than 16 */
  sla  r2, 0 

  /* eq_16: Shift count equals 16 */
ashleq16:
  mov  r2, r1
  clr  r2
  jmp  ashldone

  /* lt_16: Shift count less than 16 */
ashllt16:      
  abs  r0
  jeq  ashldone
  mov  r2, r15
  sla  r1, 0
  sla  r2, 0
  neg  r0
  srl  r15, 0
  soc  r15, r1

ashldone:
  mov  @-2(r10), r15
  b    *r11
#endif


#ifdef L_ashrsi3
/******************************************************************************
*                               __ashrsi3
*******************************************************************************
* Arithmetic left shift of 32 bit value
*
* Inputs:  [R1,R2] - 32-bit value
*          [R3]    - Shift count
*
* Returns: [R1,R2] - 32-bit result
******************************************************************************/
  def __ashrsi3
__ashrsi3:
  mov  r15, @-2(r10)     /* Scratch reg */

  /* Variable shift */
  mov  r3, r0
  ci   r0, 16
  jlt  ashrlt16
  jeq  ashreq16

  /* Shift count greater than 16 */
  sra  r1, 0

ashreq16:
  /* eq_16: Shift count equals 16 */
  mov  r1, r2
  seto r1
  jlt  ashrdone
  clr  r1
  jmp  ashrdone

ashrlt16:
  /* lt_16: Shift count less than 16 */
  abs  r0
  jeq  ashrdone
  mov  r2, r15
  sra  r1, 0
  srl  r2, 0
  neg  r0
  sla  r15, 0
  soc  r15, r2

ashrdone:
  mov  @-2(r10), r15
  b    *r11
#endif


#ifdef L_lshrsi3
/******************************************************************************
*                               __lshrsi3
*******************************************************************************
* Arithmetic left shift of 32 bit value
*
* Inputs:  [R1,R2] - 32-bit value
*          [R3]    - Shift count
*
* Returns: [R1,R2] - 32-bit result
******************************************************************************/
  def __lshrsi3
__lshrsi3:
  mov  r15, @-2(r10)     /* Scratch reg */

  /* Variable shift */
  mov  r3, r0
  ci   r0, 16
  jlt  lshrlt16
  jeq  lshreq16

  /* Shift count greater than 16 */
  srl  r1, 0

lshreq16:
  /* eq_16: Shift count equals 16 */
  mov  r1, r2
  clr  r1
  jmp  lshrdone

lshrlt16:
  /* lt_16: Shift count less than 16 */
  abs  r0
  jeq  lshrdone
  mov  r2, r15
  srl  r1, 0
  srl  r2, 0
  neg  r0
  sla  r15, 0
  soc  r15, r2

lshrdone:
  mov  @-2(r10), r15
  b    *r11
#endif


