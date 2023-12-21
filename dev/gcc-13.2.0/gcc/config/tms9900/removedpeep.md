;;-------------------------------------------------------------------
;; Arithmetic shift left (for QI mode)

(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
	(sign_extend:HI(match_operand:QI 1 "register_operand" "")))
   (set (match_operand:HI 2 "register_operand" "")
	(ashift:HI (match_dup 0)
                   (plus:HI (match_operand:HI 3 "const_int_operand" "")
                            (match_operand:HI 4 "const_int_operand" ""))))]
  ""
  [(set (match_dup 2)
        (match_dup 1))
   (set (match_dup 2)
        (ashift:QI (match_dup 2)
                   (match_dup 3)))]
  "")



;;-------------------------------------
(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
	(sign_extend:HI(match_operand:QI 1 "register_operand" "")))
   (set (match_operand:HI 3 "register_operand" "")
	(ashiftrt:HI (match_operand:HI 4 "register_operand" "")
		     (match_operand:HI 5 "shift_count_operand" "")))
   (set (match_operand:QI 6 "register_operand" "")
        (subreg:QI (match_operand:HI 7 "register_operand" "") 1))]
  ""
  [(set (match_dup 6)
        (match_dup 1))
   (set (match_dup 6)
        (ashiftrt:QI (match_dup 6)
                     (match_dup 5)))]
  "")

(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
	(sign_extend:HI(match_operand:QI 1 "register_operand" "")))
   (set (match_operand:HI 3 "register_operand" "")
	(lshiftrt:HI (match_operand:HI 4 "register_operand" "")
		     (match_operand:HI 5 "shift_count_operand" "")))
   (set (match_operand:QI 6 "register_operand" "")
        (subreg:QI (match_operand:HI 7 "register_operand" "") 1))]
  ""
  [(set (match_dup 6)
        (match_dup 1))
   (set (match_dup 6)
        (lshiftrt:QI (match_dup 6)
                     (match_dup 5)))]
  "")
      

;;-------------------------------------------------------------------
;;  Optimizations For Comparisons
;;-------------------------------------------------------------------

;;-------------------------------------------------------------------
;; Optimization for X == {-2,-1,1,2}
(define_peephole2
  [(set (reg:CC CC_REGNUM)
	(compare:CC (match_operand:HI 0 "register_operand" "")
		 (match_operand:HI 1 "immediate_operand" "")))
   (set (pc) (if_then_else (eq (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_operand 2 "" ""))
                           (pc)))]
  "peep2_reg_dead_p(2, operands[0])"
  [(set (match_operand:HI 0 "register_operand" "")
	(plus:HI (match_dup 0) (neg:HI (match_dup 1))))
   (set (reg:CC CC_REGNUM) (match_dup 0))
   (set (pc) (if_then_else (eq (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_dup 2))
                           (pc)))]
  )


(define_insn "*sub_const_hi"
  [(set (match_operand:HI 0 "register_operand" "=r,r")
        (plus:HI (match_operand:HI 1 "register_operand" "0,0")
                 (neg:HI (match_operand:HI 2 "immediate_operand" "LMNO, i"))))]
  ""
  {
    operands[2] = GEN_INT(-INTVAL(operands[2]));
    switch(INTVAL(operands[2]))
    {
      case -2: output_asm_insn("dect %0",     operands); break;
      case -1: output_asm_insn("dec  %0",     operands); break;
      case  1: output_asm_insn("inc  %0",     operands); break;
      case  2: output_asm_insn("inct %0",     operands); break;
      default: output_asm_insn("ai   %0, %2", operands); break;
    }
    return("");
  }
  [(set_attr "length" "2,4")])


;;-------------------------------------------------------------------
;; Optimization for X != {-2,-1,1,2}
(define_peephole2
  [(set (reg:CC CC_REGNUM)
	(compare:CC (match_operand:HI 0 "register_operand" "")
		 (match_operand:HI 1 "immediate_operand" "")))
   (set (pc) (if_then_else (ne (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_operand 2 "" ""))
                           (pc)))]
  "peep2_reg_dead_p(2, operands[0])"
  [(set (match_operand:HI 0 "register_operand" "")
	(plus:HI (match_dup 0) (neg:HI (match_dup 1))))
   (set (reg:CC CC_REGNUM) (match_dup 0))
   (set (pc) (if_then_else (ne (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_dup 2))
                           (pc)))]
  )


;;-------------------------------------------------------------------
;;  Optimizations For Byte Compares
;;-------------------------------------------------------------------


;;-------------------------------------------------------------------
;; Optimization for (unsigned char)X > N
(define_peephole2
  [(set (match_operand:QI 0 "register_operand" "")
        (match_operand:QI 1 "const_int_operand" ""))
   (set (reg:CC CC_REGNUM)
        (compare:CC (match_operand:QI 2 "register_operand" "")
                 (match_dup 0)))
   (set (pc) (if_then_else (gtu (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_operand 3 "" ""))
                           (pc)))]
  "peep2_reg_dead_p(3, operands[0])"
  [(set (reg:CC CC_REGNUM)
        (compare:CC (match_dup 2)
                 (plus:HI (const_int 255)
                          (mult:HI (match_dup 1)
                                   (const_int 256)))))
   (set (pc) (if_then_else (gtu (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_dup 3))
                           (pc)))]
  )

;; TODO assume this can be deleted
(define_insn "*cmpqi_as_hi"
  [(set (reg:CC CC_REGNUM)
      (compare:CC (match_operand:QI 0 "register_operand" "r")
         (plus:HI (match_operand:HI 1 "const_int_operand" "i")
            (mult:HI (match_operand:HI 2 "const_int_operand" "i")
               (match_operand:HI 3 "const_int_operand" "i")))))]
  "INTVAL(operands[3]) == 256 && (INTVAL(operands[1]) == 0 || INTVAL(operands[1]) == 255)"
  {
    operands[1] = GEN_INT(INTVAL(operands[2]) * 256 + INTVAL(operands[1]));
    return("ci   %0, %1");
  }
  [(set_attr "length" "4")])


;;-------------------------------------------------------------------
;; Optimization for (unsigned char)X < N
(define_peephole2
  [(set (match_operand:QI 0 "register_operand" "")
        (match_operand:QI 1 "const_int_operand" ""))
   (set (reg:CC CC_REGNUM)
        (compare:CC (match_operand:QI 2 "register_operand" "")
                 (match_dup 0)))
   (set (pc) (if_then_else (ltu (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_operand 3 "" ""))
                           (pc)))]
  "peep2_reg_dead_p(3, operands[0])"
  [(set (reg:CC CC_REGNUM)
        (compare:CC (match_dup 2)
                 (plus:HI (const_int 0)
                          (mult:HI (match_dup 1)
                                   (const_int 256)))))
   (set (pc) (if_then_else (ltu (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_dup 3))
                           (pc)))]
  )


;;-------------------------------------------------------------------
;; Optimization for (unsigned char)X >= N
(define_peephole2
  [(set (match_operand:QI 0 "register_operand" "")
        (match_operand:QI 1 "const_int_operand" ""))
   (set (reg:CC CC_REGNUM)
        (compare:CC (match_operand:QI 2 "register_operand" "")
                 (match_dup 0)))
   (set (pc) (if_then_else (geu (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_operand 3 "" ""))
                           (pc)))]
  "peep2_reg_dead_p(3, operands[0])"
  [(set (reg:CC CC_REGNUM)
        (compare:CC (match_dup 2)
                 (plus:HI (const_int 0)
                          (mult:HI (match_dup 1)
                                   (const_int 256)))))
   (set (pc) (if_then_else (geu (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_dup 3))
                           (pc)))]
  )


;;-------------------------------------------------------------------
;; Optimization for (unsigned char)X <= N
(define_peephole2
  [(set (match_operand:QI 0 "register_operand" "")
        (match_operand:QI 1 "const_int_operand" ""))
   (set (reg:CC CC_REGNUM)
        (compare:CC (match_operand:QI 2 "register_operand" "")
                 (match_dup 0)))
   (set (pc) (if_then_else (leu (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_operand 3 "" ""))
                           (pc)))]
  "peep2_reg_dead_p(3, operands[0])"
  [(set (reg:CC CC_REGNUM)
        (compare:CC (match_dup 2)
                 (plus:HI (const_int 255)
                          (mult:HI (match_dup 1)
                                   (const_int 256)))))
   (set (pc) (if_then_else (leu (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_dup 3))
                           (pc)))]
  )


;;-------------------------------------------------------------------
;; Optimization for (char)X > N
(define_peephole2
  [(set (match_operand:QI 0 "register_operand" "")
        (match_operand:QI 1 "const_int_operand" ""))
   (set (reg:CC CC_REGNUM)
        (compare:CC (match_operand:QI 2 "register_operand" "")
                 (match_dup 0)))
   (set (pc) (if_then_else (gt (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_operand 3 "" ""))
                           (pc)))]
  "peep2_reg_dead_p(3, operands[0])"
  [(set (reg:CC CC_REGNUM)
        (compare:CC (match_dup 2)
                 (plus:HI (const_int 255)
                          (mult:HI (match_dup 1)
                                  (const_int 256)))))
   (set (pc) (if_then_else (gt (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_dup 3))
                           (pc)))]
  )


;;-------------------------------------------------------------------
;; Optimization for (char)X < N
(define_peephole2
  [(set (match_operand:QI 0 "register_operand" "")
        (match_operand:QI 1 "const_int_operand" ""))
   (set (reg:CC CC_REGNUM)
        (compare:CC (match_operand:QI 2 "register_operand" "")
                 (match_dup 0)))
   (set (pc) (if_then_else (lt (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_operand 3 "" ""))
                           (pc)))]
  "peep2_reg_dead_p(3, operands[0])"
  [(set (reg:CC CC_REGNUM)
        (compare:CC (match_dup 2)
                 (plus:HI (const_int 0)
                          (mult:HI (match_dup 1)
                                   (const_int 256)))))
   (set (pc) (if_then_else (lt (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_dup 3))
                           (pc)))]
  )


;;-------------------------------------------------------------------
;; Optimization for (char)X >= N
(define_peephole2
  [(set (match_operand:QI 0 "register_operand" "")
        (match_operand:QI 1 "const_int_operand" ""))
   (set (reg:CC CC_REGNUM)
        (compare:CC (match_operand:QI 2 "register_operand" "")
                 (match_dup 0)))
   (set (pc) (if_then_else (ge (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_operand 3 "" ""))
                           (pc)))]
  "peep2_reg_dead_p(3, operands[0])"
  [(set (reg:CC CC_REGNUM)
        (compare:CC (match_dup 2)
                 (plus:HI (const_int 0)
                          (mult:HI (match_dup 1)
                                   (const_int 256)))))
   (set (pc) (if_then_else (ge (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_dup 3))
                           (pc)))]
  )


;;-------------------------------------------------------------------
;; Optimization for (char)X <= N
(define_peephole2
  [(set (match_operand:QI 0 "register_operand" "")
        (match_operand:QI 1 "const_int_operand" ""))
   (set (reg:CC CC_REGNUM)
        (compare:CC (match_operand:QI 2 "register_operand" "")
                 (match_dup 0)))
   (set (pc) (if_then_else (le (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_operand 3 "" ""))
                           (pc)))]
  "peep2_reg_dead_p(3, operands[0])"
  [(set (reg:CC CC_REGNUM)
        (compare:CC (match_dup 2)
                 (plus:HI (const_int 255)
                          (mult:HI (match_dup 1)
                                   (const_int 256)))))
   (set (pc) (if_then_else (le (reg:CC CC_REGNUM) (const_int 0))
                           (label_ref (match_dup 3))
                           (pc)))]
  )


;;-------------------------------------------------------------------
;;  Optimizations For Bit Shift And Cast
;;-------------------------------------------------------------------


;;-------------------------------------------------------------------
;; Optimization for (char)X = (int X) >> N
(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
        (ashiftrt:HI (match_dup 0)
            (match_operand:HI 1 "const_int_operand" "")))
   (set (match_operand:QI 2 "register_operand" "")
        (subreg:QI (match_dup 0) 1))]
  "REGNO(operands[0]) == REGNO(operands[2])"
  [(set (match_dup 0)
        (ashiftrt:HI (match_dup 0)
            (minus:HI (match_dup 1) 
                      (const_int 8))))]
  )

(define_insn "*ashiftrt_hi_to_qi"
  [(set (match_operand:HI 0 "register_operand" "=r")
        (ashiftrt:HI (match_dup 0)
            (minus:HI (match_operand:HI 1 "const_int_operand" "i") 
                      (match_operand:HI 2 "const_int_operand" "i"))))]
  ""
  {
    int shift = INTVAL (operands[1]);
    if(shift < 8)
    {
      operands[1] = GEN_INT(8 - shift);
      output_asm_insn("sla  %0, %1", operands);
    }
    else if(shift > 8)
    {
      operands[1] = GEN_INT(shift - 8);
      output_asm_insn("sra  %0, %1", operands);
    }
    return "";
  }
  [(set_attr "length" "2")])


;;-------------------------------------------------------------------
;; Optimization for (unsigned char)X = (unsigned int X) >> N
(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
        (lshiftrt:HI (match_dup 0)
            (match_operand:HI 1 "const_int_operand" "")))
   (set (match_operand:QI 2 "register_operand" "")
        (subreg:QI (match_dup 0) 1))]
  "REGNO(operands[0]) == REGNO(operands[2])"
  [(set (match_dup 0)
        (lshiftrt:HI (match_dup 0)
            (minus:HI (match_dup 1) 
                      (const_int 8))))]
  )


(define_insn "*lshiftrt_hi_to_qi"
  [(set (match_operand:HI 0 "register_operand" "=r")
        (lshiftrt:HI (match_dup 0)
            (minus:HI (match_operand:HI 1 "const_int_operand" "i") 
                      (match_operand:HI 2 "const_int_operand" "i"))))]
  ""
  {
    int shift = INTVAL (operands[1]);
    if(shift < 8)
    {
      operands[1] = GEN_INT(8 - shift);
      output_asm_insn("sla  %0, %1", operands);
    }
    else if(shift > 8)
    {
      operands[1] = GEN_INT(shift - 8);
      output_asm_insn("srl  %0, %1", operands);
    }
    return "";
  }
  [(set_attr "length" "2")])


;;-------------------------------------------------------------------
;; Optimization for (char)X = (int X) << N
(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
        (ashift:HI (match_dup 0)
            (match_operand:HI 1 "const_int_operand" "")))
   (set (match_operand:QI 2 "register_operand" "")
        (subreg:QI (match_dup 0) 1))]
  "REGNO(operands[0]) == REGNO(operands[2])"
  [(set (match_dup 0)
        (ashift:HI (match_dup 0)
            (plus:HI (match_dup 1) 
                      (const_int 8))))]
  )


(define_insn "*ashift_hi_to_qi"
  [(set (match_operand:HI 0 "register_operand" "=r")
        (ashift:HI (match_dup 0)
            (plus:HI (match_operand:HI 1 "const_int_operand" "i") 
                      (match_operand:HI 2 "const_int_operand" "i"))))]
  ""
  {
    operands[1] = GEN_INT(8 + INTVAL (operands[1]));
    output_asm_insn("sla  %0, %1", operands);
    return("");
  }
  [(set_attr "length" "2")])


;;-------------------------------------------------------------------
;; Optimization for (int)X = (char X) >> N
(define_peephole2
  [(set (match_operand:QI 0 "register_operand" "")
        (ashiftrt:QI (match_dup 0)
            (match_operand:HI 1 "const_int_operand" "")))
   (set (match_operand:HI 2 "register_operand" "")
        (sign_extend:HI (match_dup 0)))]
  "REGNO(operands[0]) == REGNO(operands[2])"
  [(set (match_dup 2)
        (ashiftrt:HI (match_dup 2)
            (plus:HI (match_dup 1) 
                      (const_int 8))))]
  )


(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
	(sign_extend:HI (match_operand:QI 1 "register_operand" "")))
   (set (match_operand:HI 2 "register_operand" "")
        (ashiftrt:HI (match_dup 2)
            (match_operand:HI 3 "const_int_operand" "")))]
  "REGNO(operands[0]) == REGNO(operands[2])"
  [(set (match_dup 2)
        (ashiftrt:HI (match_dup 2)
            (plus:HI (match_dup 3) 
                      (const_int 8))))]
  )


(define_insn "*ashiftrt_qi_to_hi"
  [(set (match_operand:HI 0 "register_operand" "=r")
        (ashiftrt:HI (match_dup 0)
            (plus:HI (match_operand:HI 1 "const_int_operand" "i") 
                      (match_operand:HI 2 "const_int_operand" "i"))))]
  ""
  {
    int shift = INTVAL (operands[1]) + 8;
    if(shift > 15) shift = 15;
    operands[1] = GEN_INT(shift);
    output_asm_insn("sra  %0, %1", operands);
    return("");
  }
  [(set_attr "length" "2")])


;;-------------------------------------------------------------------
;; Optimization for (int)X = (unsigned char X) >> N
(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
        (lshiftrt:HI (match_operand:HI 1 "register_operand" "")
            (match_operand:HI 2 "const_int_operand" "")))
   (set (match_operand:HI 3 "register_operand" "")
        (and:HI (match_operand:HI 4 "const_int_operand" "")
                (match_operand:HI 5 "const_int_operand" "")))]
  "(REGNO(operands[0]) == REGNO(operands[3])) && (INTVAL(operands[5]) == (1<<(8-INTVAL(operands[2])))-1)"
  [(set (match_dup 0)
        (lshiftrt:HI (match_dup 0)
            (plus:HI (match_dup 2) 
                      (const_int 8))))]
  )


(define_insn "*lshiftrt_qi_to_hi"
  [(set (match_operand:HI 0 "register_operand" "=r")
        (lshiftrt:HI (match_dup 0)
            (plus:HI (match_operand:HI 1 "const_int_operand" "i") 
                      (match_operand:HI 2 "const_int_operand" "i"))))]
  ""
  {
    int shift = INTVAL (operands[1]) + 8;
    if(shift > 15) shift = 15;
    operands[1] = GEN_INT(shift);
    output_asm_insn("srl  %0, %1", operands);
    return("");
  }
  [(set_attr "length" "2")])


;;-------------------------------------------------------------------
;; Optimization for (int)X = ((int)(char X)) << N
(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
        (sign_extend:HI (match_operand:QI 1 "register_operand" "")))
   (set (match_operand:HI 3 "register_operand" "")
        (ashift:HI (match_operand:HI 4 "register_operand" "")
                   (match_operand:HI 5 "const_int_operand" "")))]
  "(REGNO(operands[0]) == REGNO(operands[3]))"
  [(set (match_dup 0)
        (ashiftrt:HI (const_int 8)
            (ashift:HI (match_dup 0) 
                       (match_dup 5))))]
  )

(define_insn "*qi_hi_shift"
  [(set (match_operand:HI 0 "register_operand" "=r")
        (ashiftrt:HI (match_operand:HI 1 "const_int_operand" "i")
            (ashift:HI (match_operand:HI 2 "register_operand" "0") 
                       (match_operand:HI 3 "const_int_operand" "i"))))]
  ""
  {
    int shift = INTVAL(operands[3]);
    if(shift == 0) {
      output_asm_insn("swpb %0", operands);
      }
    else if(shift >= 1 && shift <= 7) { 
      operands[3] = GEN_INT(8-shift);
      output_asm_insn("sra  %0, %3", operands);
      }
    else if(shift >= 9 && shift <= 15) {
      operands[3] = GEN_INT(shift-8);
      output_asm_insn("sla  %0, %3", operands);
      }

    operands[3] = GEN_INT(0x00FF << shift);
    output_asm_insn("andi %0, %3", operands);
   
    return("");
  }
  [(set_attr "length" "6")])


;;-------------------------------------------------------------------
;; Optimization for (unsigned int)X = ((unsigned int)(unsigned char X)) << N
(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
        (zero_extend:HI (match_operand:QI 1 "register_operand" "")))
   (set (match_operand:HI 3 "register_operand" "")
        (ashift:HI (match_operand:HI 4 "register_operand" "")
                   (match_operand:HI 5 "const_int_operand" "")))]
  "(REGNO(operands[0]) == REGNO(operands[3]))"
  [(set (match_dup 0)
        (lshiftrt:HI (const_int 8)
            (ashift:HI (match_dup 0) 
                       (match_dup 5))))]
  )


(define_insn "*unsigned_qi_hi_shift"
  [(set (match_operand:HI 0 "register_operand" "=r")
        (lshiftrt:HI (match_operand:HI 1 "const_int_operand" "i")
            (ashift:HI (match_operand:HI 2 "register_operand" "0") 
                       (match_operand:HI 3 "const_int_operand" "i"))))]
  ""
  {
    int shift = INTVAL(operands[3]);
    if(shift >= 0 && shift <= 7) { 
      operands[3] = GEN_INT(8-shift);
      output_asm_insn("srl  %0, %3", operands);
      }
    else if(shift >= 9 && shift <= 15) {
      operands[3] = GEN_INT(shift-8);
      output_asm_insn("sla  %0, %3", operands);
      }

    operands[3] = GEN_INT(0xFFFF << shift);
    output_asm_insn("andi %0, %3", operands);
   
    return("");
  }
  [(set_attr "length" "6")])


;;-------------------------------------------------------------------
;; Optimization for (unsigned long)X = (unsigned long Y) >> 16
;;   Original code:
;;     mov  r3, r6
;;     mov  r4, r7
;;     mov  r6, r7
;;     clr  r6
;;
;;   Optimized:
;;     mov r3, r7
;;     clr r6
(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
        (match_operand:HI 1 "register_operand" ""))
   (set (match_operand:HI 2 "register_operand" "")
        (match_operand:HI 3 "register_operand" ""))
   (parallel[
    (clobber (match_operand:HI 4 "register_operand" ""))
    (set (match_operand:SI 5 "register_operand" "")
         (lshiftrt:SI (match_operand:SI 6 "register_operand" "")
                      (match_operand:HI 7 "const_int_operand" "")))
   ])]
  "((REGNO(operands[0]) == REGNO(operands[5])) &&
    (REGNO(operands[2]) == (REGNO(operands[0])+1)) &&
    (INTVAL(operands[7]) == 16)
   )"
  [(set (match_dup 2) (match_dup 1))
   (set (match_dup 0) (const_int 0))]
)


(define_insn "*set_consthi2"
  [(set (match_operand:HI 0 "register_operand" "=r")
        (match_operand:HI 1 "const_int_operand" "i"))]
  ""
  {
    if(INTVAL(operands[1]) == 0) {
      output_asm_insn("clr  %0", operands);
    } else if((INTVAL(operands[1]) & 0xFFFF) == 0xFFFF) {
      output_asm_insn("seto  %0", operands);
    } else {
      output_asm_insn("li   %0, %1", operands);
    }
    return("");
  }
  [(set_attr "length" "4")])


;;-------------------------------------------------------------------
;; Optimization for X = Y << 16
;;   Original code:
;;     mov  r4, r6
;;     mov  r5, r7
;;     mov  r7, r6
;;     clr  r7
;;
;;   Optimized:
;;     mov r5, r6
;;     clr r7
(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
        (match_operand:HI 1 "register_operand" ""))
   (set (match_operand:HI 2 "register_operand" "")
        (match_operand:HI 3 "register_operand" ""))
   (parallel[
    (clobber (match_operand:HI 4 "register_operand" ""))
    (set (match_operand:SI 5 "register_operand" "")
         (ashift:SI (match_operand:SI 6 "register_operand" "")
                      (match_operand:HI 7 "const_int_operand" "")))
   ])]
  "((REGNO(operands[0]) == REGNO(operands[5])) &&
    (REGNO(operands[2]) == (REGNO(operands[0])+1)) &&
    (INTVAL(operands[7]) == 16)
   )"
  [(set (match_dup 0) (match_dup 3))
   (set (match_dup 2) (const_int 0))]
)


;-------------------------------------------------------------------
;; Optimization for byte array initializations
; This handles sequences like:
;   li   r1, >1200
;   movb r1, *r2
;   li   r1, >3400
;   movb r1, @1(r2)
;
; and converts to:
;   li   r1, >1234
;   movb r1, *r2
;   swpb r1
;   movb r1, @1(r2)
;
; This saves two bytes and is slightly faster
(define_peephole2
  [(match_scratch:HI 6 "")
   (set (match_operand:QI 0 "register_operand" "")
        (match_operand:QI 1 "const_int_operand" ""))
   (set (match_operand:QI 2 "nonimmediate_operand" "")
        (match_dup 0))
   (set (match_operand:QI 3 "register_operand" "")
        (match_operand:QI 4 "const_int_operand" ""))
   (set (match_operand:QI 5 "nonimmediate_operand" "")
        (match_dup 3))]
  "peep2_reg_dead_p(4, operands[0]) && peep2_reg_dead_p(4, operands[3])"
  [(set (match_dup 6)
        (ior:HI (ashift:HI (match_dup 1) 
                     (const_int 8))
             (match_dup 4)))
   (set (match_dup 2)
        (subreg:QI (ashiftrt:HI (match_dup 6) (const_int 8)) 1))
   (set (match_dup 5)
        (subreg:QI (ashiftrt:HI (match_dup 6) (const_int 0)) 1))]
)


(define_insn "*movhi_combine_consts"
  [(set (match_operand:HI 0 "register_operand" "")
        (ior:HI (ashift:HI (match_operand:QI 1 "const_int_operand" "")
                           (match_operand:QI 2 "const_int_operand" ""))
                (match_operand:QI 3 "const_int_operand" "")))]
  ""
  {
    operands[1] = GEN_INT(((INTVAL(operands[1]) & 0xFF) << 8) |
                           (INTVAL(operands[3]) & 0xFF));
    return "li   %0, %1";
  }
  [(set_attr "length" "4")])


(define_insn "*movqi_for_initializer"
  [(set (match_operand:QI 0 "memory_operand" "=rR>,Q")
        (subreg:QI (ashiftrt:HI (match_operand:HI 1 "register_operand" "r,r")
                              (match_operand:HI 2 "const_int_operand" "i,i")) 1))]
  ""
  {
    if(INTVAL(operands[2]) == 8)
    {
      output_asm_insn("movb %1, %0", operands);
    }
    else if(INTVAL(operands[2]) == 0)
    {
      output_asm_insn("swpb %1", operands);
      output_asm_insn("movb %1, %0", operands);
    }
   return("");
  }
  [(set_attr "length" "2,4")])


;;-------------------------------------------------------------------
;; Optimization for memory-to-memory copies
;; Combine a mem-reg-mem copy into a mem-mem copy
(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
        (match_operand:HI 1 "memory_operand" ""))
   (set (match_operand:HI 2 "memory_operand" "")
        (match_dup 0))]
  "peep2_reg_dead_p(2, operands[0])"
  [(set (match_operand:HI 2 "memory_operand" "")
        (match_operand:HI 1 "memory_operand" ""))]
)

   
(define_peephole2
  [(set (match_operand:QI 0 "register_operand" "")
        (match_operand:QI 1 "memory_operand" ""))
   (set (match_operand:QI 2 "memory_operand" "")
        (match_dup 0))]
  "peep2_reg_dead_p(2, operands[0])"
  [(set (match_operand:QI 2 "memory_operand" "")
        (match_operand:QI 1 "memory_operand" ""))]
)


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

(define_insn "*andi_const"
  [(set (match_operand:HI 0 "register_operand" "")
        (and:HI (match_dup 0)
                (match_operand:HI 1 "const_int_operand" "")))]
  ""
  {
    int val = INTVAL(operands[1]) & 0xFFFF;
    if(val == 0)
      return "clr  %0";
    else if(val == 0xFFFF)
      return "";
    else
      return "andi %0, %1";
  }
  [(set_attr "length" "4")])

