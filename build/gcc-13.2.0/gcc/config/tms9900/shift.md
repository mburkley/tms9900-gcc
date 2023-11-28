;;-------------------------------------------------------------------
;;  Bit Shift Operations
;;-------------------------------------------------------------------


;;-------------------------------------------------------------------
;; Arithmetic shift left



;;-------------------------------------
(define_insn "ashlhi3"
  [(set (match_operand:HI 0 "register_operand" "=r,r")
	(ashift:HI (match_operand:HI 1 "register_operand" "0,0")
		   (match_operand:HI 2 "shift_count_operand" "r,i")))]
  ""
  {
    tms9900_debug_operands ("ashlhi3", operands, 3);
    if(which_alternative == 0)
    {
      output_asm_insn("abs  r0",     operands);
      output_asm_insn("jeq  $+4",    operands);
      output_asm_insn("sla  %0, 0",  operands);
    }
    else if(INTVAL(operands[2]) > 0)
    {
      output_asm_insn("sla  %0, %2", operands);
    }
    return(""); 
  }
  [(set_attr "length" "6,2")])


;;-------------------------------------
(define_insn "ashlqi3"
  [(set (match_operand:QI 0 "register_operand" "=r,r")
	(ashift:QI (match_operand:QI 1 "register_operand" "0,0")
		   (match_operand:HI 2 "shift_count_operand" "r,i")))]
  ""
  {
    tms9900_debug_operands ("ashlqi3", operands, 3);
    if(which_alternative == 0)
    {
      output_asm_insn("abs  r0",        operands);
      output_asm_insn("jeq  $+8",       operands);  /* If shift count is zero, do nothing */
      output_asm_insn("andi %0, >FF00", operands);
      output_asm_insn("sla  %0, 0",     operands);
    }
    else if(INTVAL(operands[2]) > 0)
    {
      output_asm_insn("andi %0, >FF00", operands);
      output_asm_insn("sla  %0, %2",    operands);
    }
    return(""); 
  }
  [(set_attr "length" "10,6")])


;;-------------------------------------------------------------------
;; Arithmetic shift right


;;-------------------------------------
(define_insn "ashrhi3"
  [(set (match_operand:HI 0 "register_operand" "=r,r")
	(ashiftrt:HI (match_operand:HI 1 "register_operand" "0,0")
		     (match_operand:HI 2 "shift_count_operand" "r,i")))]
  ""
  {
    tms9900_debug_operands ("ashrhi3", operands, 3);
    if(which_alternative == 0)
    {
      output_asm_insn("abs  r0",     operands);
      output_asm_insn("jeq  $+4",    operands);
      output_asm_insn("sra  %0, 0",  operands);
    }
    else if(INTVAL(operands[2]) > 0)
    {
      output_asm_insn("sra  %0, %2", operands);
    }
    return(""); 
  }
  [(set_attr "length" "6,2")])


;;-------------------------------------
(define_insn "ashrqi3"
  [(set (match_operand:QI 0 "register_operand" "=r,r")
	(ashiftrt:QI (match_operand:QI 1 "register_operand" "0,0")
		     (match_operand:HI 2 "shift_count_operand" "r,i")))]
  ""
  {
    tms9900_debug_operands ("ashrqi3", operands, 3);
    if(which_alternative == 0)
    {
      output_asm_insn("abs  r0",        operands);
      output_asm_insn("jeq  $+4",       operands);
      output_asm_insn("sra  %0, 0",     operands);
    }
    else if(INTVAL(operands[2]) > 0)
    {
      output_asm_insn("sra  %0, %2",    operands);
    }
    return(""); 
  }
  [(set_attr "length" "6,2")])



;;-------------------------------------------------------------------
;; Logical shift right



;;-------------------------------------
(define_insn "lshrhi3"
  [(set (match_operand:HI 0 "register_operand" "=r,r")
	(lshiftrt:HI (match_operand:HI 1 "register_operand" "0,0")
		     (match_operand:HI 2 "shift_count_operand" "r,i")))]
  ""
  {
    tms9900_debug_operands ("lshrhi3", operands, 3);
    if(which_alternative == 0)
    {
      output_asm_insn("abs  r0",     operands);
      output_asm_insn("jeq  $+4",    operands);
      output_asm_insn("srl  %0, 0",  operands);
    }
    else if(INTVAL(operands[2]) > 0)
    {
      output_asm_insn("srl  %0, %2", operands);
    }
    return(""); 
  }
  [(set_attr "length" "6,2")])


;;-------------------------------------
(define_insn "lshrqi3"
  [(set (match_operand:QI 0 "register_operand" "=r,r")
	(lshiftrt:QI (match_operand:QI 1 "register_operand" "0,0")
		     (match_operand:HI 2 "shift_count_operand" "r,i")))]
  ""
  {
    tms9900_debug_operands ("lshrqi3", operands, 3);
    if(which_alternative == 0)
    {
      output_asm_insn("abs  r0",        operands);
      output_asm_insn("jeq  $+4",       operands);
      output_asm_insn("srl  %0, 0",     operands);
    }
    else if(INTVAL(operands[2]) > 0)
    {
      output_asm_insn("srl  %0, %2",    operands);
    }
    return(""); 
  }
  [(set_attr "length" "6,2")])



;;-------------------------------------------------------------------
;; Rotate 


;;-------------------------------------
(define_expand "rotlhi3"
  [(set (match_operand:HI 0 "register_operand" "=r,r")
        (rotatert:HI (match_operand:HI 1 "register_operand" "0,0")
                     (match_operand:HI 2 "shift_count_operand" "r,i")))]
  ""
  {
    tms9900_debug_operands ("rotlhi3", operands, 3);
    if (GET_CODE (operands[2]) == CONST_INT)
      operands[2] = GEN_INT ((16 - INTVAL (operands[2])) % 16);
    else
      {
        rtx reg = gen_reg_rtx (HImode);
        emit_insn (gen_subhi3 (reg, GEN_INT (16), operands[2]));
        operands[2] = reg;
      }
  })


;;-------------------------------------
(define_insn "rotrhi3"
  [(set (match_operand:HI 0 "register_operand" "=r,r")
        (rotatert:HI (match_operand:HI 1 "register_operand" "0,0")
                     (match_operand:HI 2 "shift_count_operand" "r,i")))]
  ""
  "@
  src  %0, 0
  src  %0, %2" 
 [(set_attr "length" "2,2")])


