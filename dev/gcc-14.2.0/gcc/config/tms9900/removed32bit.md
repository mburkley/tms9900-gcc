;;-------------------------------------
(define_insn "ashlsi3"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(ashift:SI (match_operand:SI 1 "register_operand" "0,0")
		   (match_operand:HI 2 "shift_count_operand" "r,i")))
   (clobber (match_scratch:HI 3 "=&r,&r"))]
     ""
  {
    tms9900_debug_operands ("ashlsi3", operands, 3);
    rtx ops[7];
    ops[0] = operands[0];  /* Destination */
    ops[1] = operands[1];  /* Source */
    ops[2] = operands[2];  /* Shift count */
    ops[3] = operands[3];  /* Scratch reg */
    ops[4] = gen_rtx_REG (HImode, REGNO (operands[0]) + REGS_PER_WORD);  /* Low word of source */
    if(which_alternative == 0)
    {
      /* Variable shift */
      output_asm_insn("ci   %2, 16",  ops);  /* +0: Compare shift to 16 */
      output_asm_insn("jlt  $+12",    ops);  /* +4: If shift was < 16, goto lt_16 */
      output_asm_insn("jeq  $+4",     ops);  /* +6: If shift was 16, goto eq_16 */
     
      /* Shift count greater than 16 */
      output_asm_insn("sla  %4, %2",  ops);  /* +8: Shift low word */

      /* eq_16: Shift count equals 16 */
      output_asm_insn("mov  %4, %1",  ops);  /* +10: Copy low word to high word */ 
      output_asm_insn("clr  %4",      ops);  /* +12: Clear low word */
      output_asm_insn("jmp  $+20",    ops);  /* +14: Goto end */

      /* lt_16: Shift count less than 16 */
      output_asm_insn("abs  %2",      ops);  /* +16: Test shift count */
      output_asm_insn("jeq  $+16",    ops);  /* +18: If shift==0, goto end */
      output_asm_insn("mov  %4, %3",  ops);  /* +20: Save low word to temp */
      output_asm_insn("sla  %1, %2",  ops);  /* +22: Shift high word */
      output_asm_insn("sla  %4, %2",  ops);  /* +24: Shift low word */
      output_asm_insn("neg  %2",      ops);  /* +26: Get complement of shift count */
      output_asm_insn("srl  %3, %2",  ops);  /* +28: Shift low word bits into high word position */
      output_asm_insn("soc  %3, %1",  ops);  /* +30: Merge shifted low word bits into high word */
      output_asm_insn("neg  %2",      ops);  /* +32: Restore shift count */
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
          output_asm_insn("movb %4, %1",  ops);  /* Move MSB of high word */
          output_asm_insn("sb   %4, %4",  ops);  /* Clear MSB of high word */
          output_asm_insn("swpb %1",      ops);  /* Swap bytes in low word */
          output_asm_insn("swpb %4",      ops);  /* Swap bytes in high word */
        }
        else
        {
          output_asm_insn("mov  %4, %3", ops);  /* Save low word to temp */
          output_asm_insn("sla  %1, %2", ops);  /* Shift high word */
          output_asm_insn("sla  %4, %2", ops);  /* Shift low word */
          output_asm_insn("srl  %3, %5", ops);  /* Shift low word bits into high word position */
          output_asm_insn("soc  %3, %1", ops);  /* Merge low word bits into high word */
        }
      } 
      else
      {
        if(offset > 16)
        {
          output_asm_insn("sla  %4, %6", ops);  /* Shift low word */
        }
        output_asm_insn("mov  %4, %1", ops);  /* Move low word into high word */
        output_asm_insn("clr  %4",     ops);  /* Clear low word */
      }
    }
    return "";
  }
  [(set_attr "length" "34, 10")]
)

;; TODO MGB tidy up - looks a bit convoluted
(define_insn "ashrsi3"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(ashiftrt:SI (match_operand:SI 1 "register_operand" "0,0")
		     (match_operand:HI 2 "shift_count_operand" "r,i")))
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
      output_asm_insn("ci   %2, 16",  ops);  /* +0: Compare shift to 16 */
      output_asm_insn("jlt  $+16",    ops);  /* +4: If shift was < 16, goto lt_16 */
      output_asm_insn("jeq  $+4",     ops);  /* +6: If shift was 16, goto eq_16 */

      /* Shift count greater than 16 */
      output_asm_insn("sra  %1, %2",  ops);  /* +8: Shift high word */

      /* eq_16: Shift count equals 16 */
      output_asm_insn("mov  %1, %4",  ops);  /* +10: Copy high word to low word */ 
      output_asm_insn("seto %1",      ops);  /* +12: Assume negative value, set high word */
      output_asm_insn("jlt  $+4",     ops);  /* +14: If value was negative, skip next instruction */
      output_asm_insn("clr  %1",      ops);  /* +16: Clear high word */
      output_asm_insn("jmp  $+20",    ops);  /* +18: Goto end */

      /* lt_16: Shift count less than 16 */
      output_asm_insn("abs  %2",      ops);  /* +20: Test shift count */
      output_asm_insn("jeq  $+16",    ops);  /* +22: If shift==0, goto end */
      output_asm_insn("mov  %4, %3",  ops);  /* +24: Save high word to temp */
      output_asm_insn("sra  %1, %2",  ops);  /* +26: Shift high word */
      output_asm_insn("srl  %4, %2",  ops);  /* +28: Shift low word */
      output_asm_insn("neg  %2",      ops);  /* +30: Get complement of shift count */
      output_asm_insn("sla  %3, %2",  ops);  /* +32: Shift high word bits into low word position */
      output_asm_insn("soc  %3, %4",  ops);  /* +34: Merge shifted high word bits into low word */
      output_asm_insn("neg  %2",      ops);  /* +36: Restore shift count */
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
        output_asm_insn("mov  %1, %3",  ops);  /* Save high word to temp */
        output_asm_insn("sra  %1, %6",  ops);  /* Shift high word */
        output_asm_insn("srl  %4, %6",  ops);  /* Shift low word */
        output_asm_insn("sla  %3, %5",  ops);  /* Shift high word bits into low word position */
        output_asm_insn("soc  %3, %4",  ops);  /* Merge shifted high word bits into low word */
      } 
      else
      {
        if(offset > 16)
        {
          output_asm_insn("sra  %1, %6", ops);  /* Shift high word */
        }
        output_asm_insn("mov  %1, %4",  ops);  /* Copy high word to low word */ 
        output_asm_insn("seto %1",      ops);  /* Assume negative value, set high word */
        output_asm_insn("jlt  $+4",     ops);  /* If value was negative, skip next instruction */
        output_asm_insn("clr  %1",      ops);  /* Clear high word */
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
		     (match_operand:HI 2 "shift_count_operand" "r,i")))
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
      output_asm_insn("ci   %2, 16",  ops);  /* +0: Compare shift to 16 */
      output_asm_insn("jlt  $+12",    ops);  /* +4: If shift was < 16, goto lt_16 */
      output_asm_insn("jeq  $+4",     ops);  /* +6: If shift was 16, goto eq_16 */

      /* Shift count greater than 16 */
      output_asm_insn("srl  %1, %2",  ops);  /* +8: Shift high word */

      /* eq_16: Shift count equals 16 */
      output_asm_insn("mov  %1, %4",  ops);  /* +10: Copy high word to low word */ 
      output_asm_insn("clr  %1",      ops);  /* +12: Clear high word */
      output_asm_insn("jmp  $+20",    ops);  /* +14: Goto end */

      /* lt_16: Shift count less than 16 */
      output_asm_insn("abs  %2",      ops);  /* +16: Test shift count */
      output_asm_insn("jeq  $+16",    ops);  /* +18: If shift==0, goto end */
      output_asm_insn("mov  %1, %3",  ops);  /* +20: Save high word to temp */
      output_asm_insn("srl  %1, %2",  ops);  /* +22: Shift high word */
      output_asm_insn("srl  %4, %2",  ops);  /* +24: Shift low word */
      output_asm_insn("neg  %2",      ops);  /* +26: Get complement of shift count */
      output_asm_insn("sla  %3, %2",  ops);  /* +28: Shift high word bits into low word position */
      output_asm_insn("soc  %3, %4",  ops);  /* +30: Merge shifted high word bits into low word */
      output_asm_insn("neg  %2",      ops);  /* +32: Restore shift count */
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
          output_asm_insn("swpb %1",      ops);  /* Swap bytes in low word */
          output_asm_insn("swpb %4",      ops);  /* Swap bytes in high word */
          output_asm_insn("movb %1, %4",  ops);  /* Move MSB of high word */
          output_asm_insn("sb   %1, %1",  ops);  /* Clear MSB of high word */
        }
        else
        {
          output_asm_insn("mov  %1, %3",  ops);  /* Save high word to temp */
          output_asm_insn("srl  %1, %6",  ops);  /* Shift high word */
          output_asm_insn("srl  %4, %6",  ops);  /* Shift low word */
          output_asm_insn("sla  %3, %5",  ops);  /* Shift high word bits into low word position */
          output_asm_insn("soc  %3, %4",  ops);  /* Merge shifted high word bits into low word */
        }
      } 
      else
      {
        if(offset > 16)
        {
          output_asm_insn("srl  %1, %6", ops);  /* Shift high word */
        }
        output_asm_insn("mov  %1, %4",  ops);  /* Copy high word to low word */ 
        output_asm_insn("clr  %1",      ops);  /* Clear high word */
      }
    }
    return "";
  }
  [(set_attr "length" "34, 10")]
)

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

      output_asm_insn("a    %2, %0", operands);
      output_asm_insn("a    %2, %0", low_words);
      output_asm_insn("jnc  $+%0",   offset);
      output_asm_insn("inc  %0",     operands);
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
              output_asm_insn("ai   %0, %2", low_words);
            else
            {
              output_asm_insn("li   %3, %2", low_words);              
              output_asm_insn("a    %3, %0", low_words);              
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
              output_asm_insn("ai   %0, %2", operands);
            else
            {
              output_asm_insn("li   %3, %2", operands);              
              output_asm_insn("a    %3, %0", operands);              
            }
        }
      }
    }

    return("");
  }
  [(set_attr "length" "8,10,14,4,8,12, 10,12,16,6,12,16,  14,16,18,8,16,20")])

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

      output_asm_insn("s    %2, %0", lateoperands);
      output_asm_insn("s    %2, %0", operands);
      output_asm_insn("joc  $+%0", offset);
      output_asm_insn("dec  %0", lateoperands);
      return("");
    }
    /* EMW - This never seems to be called... */
    lateoperands[2] = GEN_INT((INTVAL(operands[2]) >> 16) & 0xffff);
    operands[2] = GEN_INT(INTVAL(operands[2]) & 0xffff);
  
    if(INTVAL(operands[2]))
    { 
      output_asm_insn("ai   %0, %2", operands);
      output_asm_insn("jnc  $+%0", offset);
      output_asm_insn("dec  %0", lateoperands);
    }
    if(INTVAL(lateoperands[2]))
    {
      operands[2] = GEN_INT(-INTVAL(operands[2]));
      output_asm_insn("ai   %0, %2", lateoperands);
    }
    return("");
  }
  [(set_attr "length" "8,10,12,4,8,12, 12,14,16,4,12,16,  16,18,18,6,12,18")])

;; MGB TMS9900 doesn't have signed multiply, but changing it to umulsi3 causes
;; the compiler to emit a bl to __mulsi3 so have to keep it for now
;;-------------------------------------
;; (define_insn "umulsi3"
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

    output_asm_insn("mov  %1, %5",args);  // H = R1
    output_asm_insn("mpy  %3, %0",args);  // [R0,R1] = R0*G1
    output_asm_insn("mov  %1, %4",args);  // T = LSW(R0*G1)
    output_asm_insn("mov  %5, %0",args);  // R0 = H
    output_asm_insn("mpy  %2, %0",args);  // [R0,R1] = R1*G0
    output_asm_insn("a    %1, %4",args);  // T += LSW(R1*G0)
    output_asm_insn("mov  %5, %0",args);  // R0 = H
    output_asm_insn("mpy  %3, %0",args);  // [R0,R1] = R1*G1
    output_asm_insn("a    %4, %0",args);  // R0 += T
    return("");
  }
  [(set_attr "length" "18,22,26")])


