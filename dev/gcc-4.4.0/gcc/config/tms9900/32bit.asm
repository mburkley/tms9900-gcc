

/*
 *  MGB Work in progress.  Library to perform 32-bit arithmetic and shift
 *  operations.  Moved here from tms9900.md
 */


/******************************************************************************
*                               __ashlsi3
*******************************************************************************
#ifdef L_ashlsi3
/*********************************************************/
/*                     __ashlsi3                          */
/*********************************************************/
  def __ashlsi3
__ashlsi3:
      ci   r2, 16
      jlt  __ashlsi3_lt16
      jeq  __ashlsi3_eq16
     
      /* Shift count greater than 16 */
      sla  r1, r2

      /* eq_16: Shift count equals 16 */
      mov  %4, %1
      clr  %4
      jmp  $+20

      /* lt_16: Shift count less than 16 */
      abs  %2
      jeq  $+16
      mov  %4, %3
      sla  %1, %2
      sla  %4, %2
      neg  %2
      srl  %3, %2
      soc  %3, %1
      neg  %2
      /* +34: End */
    }
    else
    {
      /* Constant shift */
      int offset = INTVAL(operands[2]);
      ops[5] = GEN_INT(16-offset);  /* Complement of shift count */
      ops[6] = GEN_INT(offset%16);  /* Modulo of shift count */
  
      if(offset < 16)
      {
        if(offset == 8)
        {
          movb %4, %1
          sb   %4, %4
          swpb %1
          swpb %4
        }
        else
        {
          mov  %4, %3
          sla  %1, %2
          sla  %4, %2
          srl  %3, %5
          soc  %3, %1
        }
      } 
      else
      {
        if(offset > 16)
        {
          sla  %4, %6
        }
        mov  %4, %1
        clr  %4
      }
    }
    return "";
  }
  [(set_attr "length" "34, 10")]
)


;; TODO MGB tidy up - looks a bit convoluted
;;-------------------------------------------------------------------
;; Arithmetic shift right

(define_insn "ashrsi3"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(ashiftrt:SI (match_operand:SI 1 "register_operand" "0,0")
		     (match_operand:HI 2 "shift_count_operand" "S,i")))
   (clobber (match_scratch:HI 3 "=&r,&r"))]
     ""
  {
    tms9900_debug_operands ("ashrsi3", operands, 3);
    rtx ops[7];
    ops[0] = operands[0];  /* Destination */
    ops[1] = operands[1];  /* Source */
    ops[2] = operands[2];  /* Shift count */
    ops[3] = operands[3];  /* Scratch reg */
    ops[4] = gen_rtx_REG (HImode, REGNO (operands[0]) + REGS_PER_WORD);  /* Low word of source */
    if(which_alternative == 0)
    {
      /* Variable shift */
      ci   %2, 16
      jlt  $+16
      jeq  $+4

      /* Shift count greater than 16 */
      sra  %1, %2

      /* eq_16: Shift count equals 16 */
      mov  %1, %4
      seto %1
      jlt  $+4
      clr  %1
      jmp  $+20

      /* lt_16: Shift count less than 16 */
      abs  %2
      jeq  $+16
      mov  %4, %3
      sra  %1, %2
      srl  %4, %2
      neg  %2
      sla  %3, %2
      soc  %3, %4
      neg  %2
      /* +38: End */
    }
    else
    {
      /* Constant shift */
      int offset = INTVAL(operands[2]);
      ops[5] = GEN_INT(16-offset);  /* Complement of shift count */
      ops[6] = GEN_INT(offset%16);  /* Modulo of shift count */
  
      if(offset < 16)
      {
        mov  %1, %3
        sra  %1, %6
        srl  %4, %6
        sla  %3, %5
        soc  %3, %4
      } 
      else
      {
        if(offset > 16)
        {
          sra  %1, %6
        }
        mov  %1, %4
        seto %1
        jlt  $+4
        clr  %1
      }
    }
    return "";
  }
  [(set_attr "length" "38, 10")]
)


;;-------------------------------------
(define_insn "lshrsi3"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(lshiftrt:SI (match_operand:SI 1 "register_operand" "0,0")
		     (match_operand:HI 2 "shift_count_operand" "S,i")))
   (clobber (match_scratch:HI 3 "=&r,&r"))]
     ""
  {
    tms9900_debug_operands ("lshrsi3", operands, 3);
    rtx ops[7];
    ops[0] = operands[0];  /* Destination */
    ops[1] = operands[1];  /* Source */
    ops[2] = operands[2];  /* Shift count */
    ops[3] = operands[3];  /* Scratch reg */
    ops[4] = gen_rtx_REG (HImode, REGNO (operands[0]) + REGS_PER_WORD);  /* Low word of source */
    if(which_alternative == 0)
    {
      /* Variable shift */
      ci   %2, 16
      jlt  $+12
      jeq  $+4

      /* Shift count greater than 16 */
      srl  %1, %2

      /* eq_16: Shift count equals 16 */
      mov  %1, %4
      clr  %1
      jmp  $+20

      /* lt_16: Shift count less than 16 */
      abs  %2
      jeq  $+16
      mov  %1, %3
      srl  %1, %2
      srl  %4, %2
      neg  %2
      sla  %3, %2
      soc  %3, %4
      neg  %2
      /* +34: End */
    }
    else
    {
      /* Constant shift */
      int offset = INTVAL(operands[2]);
      ops[5] = GEN_INT(16-offset);  /* Complement of shift count */
      ops[6] = GEN_INT(offset%16);  /* Modulo of shift count */
  
      if(offset < 16)
      {
        if(offset == 8)
        {
          swpb %1
          swpb %4
          movb %1, %4
          sb   %1, %1
        }
        else
        {
          mov  %1, %3
          srl  %1, %6
          srl  %4, %6
          sla  %3, %5
          soc  %3, %4
        }
      } 
      else
      {
        if(offset > 16)
        {
          srl  %1, %6
        }
        mov  %1, %4
        clr  %1
      }
    }
    return "";
  }
  [(set_attr "length" "34, 10")]
)

;; TODO convert to expand
;;-------------------------------------
(define_insn "addsi3"
  [(set (match_operand:SI 0 "nonimmediate_operand"          "=r,r,r,r,r,r, R,R,R,R,R,R, Q,Q,Q,Q,Q,Q")
	(plus:SI (match_operand:SI 1 "nonimmediate_operand" "%0,0,0,0,0,0, 0,0,0,0,0,0, 0,0,0,0,0,0")
		 (match_operand:SI 2 "general_operand"       "r,R,Q,I,J,K, r,R,Q,I,J,K, r,R,Q,I,J,K")))
   (clobber (match_scratch:HI 3                             "=r,r,r,r,r,r, r,r,r,r,r,r, r,r,r,r,r,r"))]
  ""
  { 
    tms9900_debug_operands ("addsi3", operands, 3);
    rtx low_words[4];
    rtx offset[3];
    low_words[3] = operands[3];

    if(REG_P (operands[0]))
      low_words[0] = gen_rtx_REG (HImode, REGNO (operands[0]) + REGS_PER_WORD);
    else
      low_words[0] = adjust_address (operands[0], HImode, 2);

    if(which_alternative >= 12)
      offset[0] = GEN_INT(6);
    else
      offset[0] = GEN_INT(4);
  
    if(! CONSTANT_P(operands[2]))
    {
      /* Adding two variables */
      if(REG_P (operands[2]))
        low_words[2] = gen_rtx_REG (HImode, REGNO (operands[2]) + REGS_PER_WORD);
      else
        low_words[2] = adjust_address (operands[2], HImode, 2);

      a    %2, %0
      a    %2, %0
      jnc  $+%0
      inc  %0
      return("");
    }
    else
    {
      /* Adding a constant */
      int low_const  =  INTVAL(operands[2])        & 0xffff;
      int high_const = (INTVAL(operands[2]) >> 16) & 0xffff;
      low_words[2] = GEN_INT(low_const);
      operands[2]  = GEN_INT(high_const);

      if(low_const != 0)
      {
        switch(low_const)
        {
          case 0xFFFE: output_asm_insn("dect %0",     low_words); break;
          case 0xFFFF: output_asm_insn("dec  %0",     low_words); break;
          case      1: output_asm_insn("inc  %0",     low_words); break;
          case      2: output_asm_insn("inct %0",     low_words); break;
          default:
            if(REG_P(operands[0]))
              ai   %0, %2
            else
            {
              li   %3, %2
              a    %3, %0
            }
        }
        /* DEC and DECT use inverted carry flags */
        /*
        if(low_const == 0xFFFF || low_const == 0xFFFE)
          output_asm_insn("joc  $+%0",   offset);
        else
        */
          output_asm_insn("jnc  $+%0",   offset);

        /* Handle carry to high word */
        output_asm_insn("inc  %0",     operands);
      }
      if(high_const != 0)
      {
        switch(high_const)
        {
          case 0xFFFE: output_asm_insn("dect %0",     operands); break;
          case 0xFFFF: output_asm_insn("dec  %0",     operands); break;
          case      1: output_asm_insn("inc  %0",     operands); break;
          case      2: output_asm_insn("inct %0",     operands); break;
          default:
            if(REG_P(operands[0]))
              ai   %0, %2
            else
            {
              li   %3, %2
              a    %3, %0
            }
        }
      }
    }

    return("");
  }
  [(set_attr "length" "8,10,14,4,8,12, 10,12,16,6,12,16,  14,16,18,8,16,20")])

;;-------------------------------------
(define_insn "subsi3"
  [(set (match_operand:SI 0 "nonimmediate_operand"          "=r,r,r,r,r,r,R,R,R,R,R,R,Q,Q,Q,Q,Q,Q")
	(minus:SI (match_operand:SI 1 "nonimmediate_operand" "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0")
		  (match_operand:SI 2 "general_operand"      "r,R,Q,I,J,K,r,R,Q,I,J,K,r,R,Q,I,J,K")))]
  ""
  {
    tms9900_debug_operands ("subsi3", operands, 3);
    rtx lateoperands[3];
    rtx offset[3];

    lateoperands[0] = operands[0];
    if(REG_P(operands[0]))
    {
      operands[0] = gen_rtx_REG(HImode, REGNO (operands[0]) + REGS_PER_WORD);
      offset[0] = GEN_INT(4);
    }
    else
    {
      operands[0] = adjust_address(operands[0], HImode, 2);
      offset[0] = GEN_INT(6);
    }
  
    if(! CONSTANT_P(operands[2]))
    {
      lateoperands[2] = operands[2];

      if(REG_P(operands[2]))
        operands[2] = gen_rtx_REG(HImode, REGNO (operands[2]) + REGS_PER_WORD);
      else
        operands[2] = adjust_address(operands[2], HImode, 2);

      s    %2, %0
      s    %2, %0
      joc  $+%0
      dec  %0
      return("");
    }
    /* EMW - This never seems to be called... */
    lateoperands[2] = GEN_INT((INTVAL(operands[2]) >> 16) & 0xffff);
    operands[2] = GEN_INT(INTVAL(operands[2]) & 0xffff);
  
    if(INTVAL(operands[2]))
    { 
      ai   %0, %2
      jnc  $+%0
      dec  %0
    }
    if(INTVAL(lateoperands[2]))
    {
      operands[2] = GEN_INT(-INTVAL(operands[2]));
      ai   %0, %2
    }
    return("");
  }
  [(set_attr "length" "8,10,12,4,8,12, 12,14,16,4,12,16,  16,18,18,6,12,18")])

;; TODO change this to be an expand
;;-------------------------------------
(define_insn "mulsi3"
  [(set (match_operand:SI 0 "register_operand" "=r,r,r")
        (mult:SI (match_operand:SI 1 "register_operand" "0,0,0")
                 (match_operand:SI 2 "nonimmediate_operand"  "r,R,Q")))
   (clobber (match_scratch:HI 3 "=r,r,r"))
   (clobber (match_scratch:HI 4 "=r,r,r"))]
  ""
  {
    tms9900_debug_operands ("mulsi3", operands, 3);
    rtx args[6];

    args[0] = operands[0];  // Destination R
    args[1] = gen_lowpart_SUBREG(HImode, operands[0]);

    args[2] = operands[2];  // Source G
    if(REG_P(operands[2]))
    {
      args[3] = gen_lowpart_SUBREG(HImode, operands[2]);
    }
    else
    {
      args[3] = adjust_address(operands[2], HImode, 2);
    }

    args[4] = operands[3];  // Temp
    args[5] = operands[4];  // Hold

    /*
    Since we only have a 16-bit multiply, we need to expand
    this math.

    (R0*N+R1)*(G0*N+G1) = R0*G0*N*N + R0*G1*N + R1*G0*N + R1*G1

    We can omit the R0*G0 term since it won't fit into 32 bits
    */

    mov  %1, %5
    mpy  %3, %0
    mov  %1, %4
    mov  %5, %0
    mpy  %2, %0
    a    %1, %4
    mov  %5, %0
    mpy  %3, %0
    a    %4, %0
    return("");
  }
  [(set_attr "length" "18,22,26")])
; TODO expand into 2 x neghi2
;;-------------------------------------

(define_insn "negsi2"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=rR,Q")
	(neg:SI (match_operand:SI 1 "nonimmediate_operand" "0,0")))]
  ""
  {
    tms9900_debug_operands ("negsi2", operands, 2);
    rtx word[2];
    rtx offset[1];

    /* word 0 is most significant */
    word[0] = operands[0];
    if (REG_P (operands[0]))
    {
      word[1] = gen_rtx_REG(HImode, REGNO(operands[0]) + REGS_PER_WORD);
      offset[0] = GEN_INT(4);
    }
    else
    {
      word[1] = adjust_address(operands[0], HImode, 2);
      offset[0] = GEN_INT(6);
    }
    inv  %0
    neg  %1
    jnc  $+%0
    inc  %0
    return("");
  }
  [(set_attr "length" "8,14")])

