
;; DELETED - would put byte in wrong half of word
;;-------------------------------------------------------------------
;; Optimization for (int)X = (unsigned char)((int)X)
(define_peephole2
  [(set (match_operand:QI 0 "register_operand" "")
        (subreg:QI (match_operand:HI 1 "register_operand") 1))
   (set (match_operand:HI 2 "register_operand" "")
        (zero_extend:HI (match_dup 0)))]
  "REGNO(operands[0]) == REGNO(operands[2])"
  [(set (match_dup 2)
        (match_dup 1))
   (set (match_dup 2)
        (and:HI (match_dup 2) (const_int 255)))]
)

;;-------------------------------------
;
; Byte fetch from packed structure.  This is trying to reduce the lengthy
; sequence emitted by gcc to move a word one byte at a time.  I couldn't find a
; way to emit insns that have the correct mode since the fetches are QI but the
; result is HI.
;
; Replace:
;   movb @x, r1
;   sla  r1, 8
;   movb @x+1, r2
;   srl  r2, 8
;   soc  r1,r2
;
; With:
;   movb @x+1, r2
;   swpb r2
;   movb @x, r2
;
; Where:
;   op[0] = QI:r1
;   op[1] = mem/QI:@x
;   op[2] = HI:r1
;   op[3] = HI:r2
;   op[4] = mem/QI:@x+1

(define_peephole2
  [(set (match_operand:QI 0 "register_operand" "")
	(match_operand:QI 1 "memory_operand" ""))
   (set (match_operand:HI 2 "register_operand" "")
        (ashift:HI (match_dup 2) (const_int 8)))
   (set (match_operand:HI 3 "register_operand" "")
	(zero_extend:HI (match_operand:QI 4 "memory_operand" "")))
   (set (match_dup 3)
        (ior (match_dup 3) (match_dup 2)))]       
  ""
  [(set (match_dup 0) (match_dup 4))
   (set (match_dup 2) (truncate:QI (match_dup 1)))
   (set (match_dup 3) (match_dup 1))]
  {
    tms9900_debug_operands ("peep-packed-movhi", NULL_RTX, operands, 6);
    int msb = INTVAL(XEXP (operands[1], 0));
    int lsb = INTVAL(XEXP (operands[4], 0));

    // Test some conditions.  Once these are proven working they can be moved to
    // the condition field above.
    tms9900_inline_debug ("; msb=%x lsb=%x\n", msb, lsb);
    if (msb+1 != lsb)
      FAIL;

    tms9900_inline_debug ("; or[0]-reg=%d lsbreg=%d\n", REGNO (operands[5]),
    REGNO (operands[3]));
    if (REGNO (operands[5]) != REGNO (operands[3]))
      FAIL;

    tms9900_inline_debug ("; or[2]-reg=%d msbreg=%d\n", REGNO (operands[2]),
    REGNO (operands[0]));
    if (REGNO (operands[2]) != REGNO (operands[0]))
      FAIL;

    #if 0
    // This doesn't work - emits it outside the function
    output_asm_insn("movb %4, %3",  operands);
    output_asm_insn("swpb %3",  operands);
    output_asm_insn("movb %1, %3",  operands);
    DONE;
    #endif

    #if 0
    // Replace doesn't work since the final mov has a HI and a QI operand
    tms9900_inline_debug ("; DO replace\n");
    #else
    // Emit doesn't work for the same reason, final movqi is trying to put
    // result into a HI
    tms9900_inline_debug ("; DO emit\n");
    emit_insn (gen_tms9900_movqi (operands[0], operands[4]));
    emit_insn (gen_trunchiqi2 (operands[2], operands[0]));
    emit_insn (gen_tms9900_movqi (operands[3], operands[1]));
    DONE;
    #endif
  }
)

