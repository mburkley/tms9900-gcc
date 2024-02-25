;;- Machine description for the TMS9900 for GNU C compiler
;; Copyright (C) 1994, 1995, 1997, 1998, 1999, 2000, 2001, 2004, 2005
;; 2007, 2008 Free Software Foundation, Inc.

;; Copyright 2009 Eric Welser (EMW)
;; Copyright 2023 Mark Burkley (MGB)

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 3, or (at your option)
;; any later version.

;; GCC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.


;;-------------------------------------------------------------------
;; dummy MD file.  Generates no back end code, just comments.  Used to try stuff
;; out
;;-------------------------------------------------------------------


;; define attributes
;; currently type is only fpu or arith or unknown, maybe branch later ?
;; default is arith
(define_attr "type" "unknown,arith,fp" (const_string "arith"))

;; length default is 1 words each
(define_attr "length" "" (const_int 2))

;; a user's asm statement
(define_asm_attributes
  [(set_attr "type" "unknown")
; all bets are off how long it is - make it 256, forces long jumps 
; whenever jumping around it !!!
   (set_attr "length" "256")])

;;-------------------------------------------------------------------
;;  UNSPEC Definitions
;;-------------------------------------------------------------------
(define_constants
  [(UNSPEC_RETURN  0)])


;;-------------------------------------------------------------------
;;  Predicate Definitions
;;-------------------------------------------------------------------

;;-------------------------------------------------------------------
;;  Function Calls
;;-------------------------------------------------------------------


;;-------------------------------------------------------------------
;; Jump to a subroutine which returns a value
(define_insn "call"
  [(call (match_operand:HI 0 "general_operand" "g")
         (match_operand:HI 1 "general_operand"  "g"))
  ]
  ""
  {
    tms9900_debug_operands ("call", insn, operands, 2);
    if(SIBLING_CALL_P(insn))
      output_asm_insn("; tailcall %0 (HI:%1)", operands);
    else
      output_asm_insn("; call %0 (HI:%1)", operands);
    return("");
  }
)


;-------------------------------------------------------------------
; Jump to a subroutine which returns a value
(define_insn "call_value"
  [(set (match_operand 0 "" "")
        (call (match_operand:HI 1 "general_operand" "g")
	      (match_operand:HI 2 "general_operand" "g")))
  ]
  ""
  {
    tms9900_debug_operands ("call_value", insn, operands, 3);
    if(SIBLING_CALL_P(insn))
      output_asm_insn("; %0 = tailcall %1 (HI:%2)", operands);
    else
      output_asm_insn("; %0 = call %1 (%2)", operands);
    return("");
  }
)


;; The O2 and O3 optimise options replace BL @xxx with LI Ry,xxx and BL *Ry
;; when the BL is inside a loop, even though the LI is also inside the loop,
;; which makes it less efficient.  This peephole undoes this "optimisation"

; There is an issue with this when the register used to hold the jump address
; gets spilled to the stack.  The resulting attempt to do call @6[sp] fails of
; course.  Popping from stack needs to go through a reg.  But why spill a label
; address to stack?  That makes no sense.

; (define_peephole2
;   [(set (match_operand:HI 0 "general_operand" "")
; 	(match_operand:HI 1 "general_operand" ""))
;    (set (match_operand:HI 2 "general_operand" "")
; 	(call (mem:HI (match_dup 0))
;               (match_operand:HI 3 "general_operand" "")))]
;   ""
;   [(set (match_dup 2)
;         (call (mem:HI (match_dup 1))
;               (match_dup 3)))]
;   "")



;;-------------------------------------------------------------------
;; Define function prologue
(define_expand "prologue"
  [(const_int 0)]
  ""
{
  tms9900_debug_operands ("prologue", NULL_RTX, NULL, 0);
  tms9900_expand_prologue();
  DONE;
})


;;-------------------------------------------------------------------
;; Define function epilogue
(define_expand "epilogue"
  [(return)]
  ""
{
  tms9900_debug_operands ("epilogue", NULL_RTX, NULL, 0);
  tms9900_expand_epilogue(false);
  DONE;
})

(define_expand "sibcall_epilogue"
  [(return)]
  ""
{
  tms9900_debug_operands ("sibcall_epilogue", NULL_RTX, NULL, 0);
  tms9900_expand_epilogue(true);
  DONE;
})


;;-------------------------------------------------------------------
;; Define function return
(define_insn "*rt"
  [(unspec [(match_operand:HI 0 "general_operand" "")] UNSPEC_RETURN)]
  ""
  {
    tms9900_debug_operands ("*rt", insn, operands, 1);
    return("; return");
  }
)


;;-------------------------------------------------------------------
;;  Comparison Instructions
;;-------------------------------------------------------------------

;;-------------------------------------
(define_insn "tsthi"
  [(set (cc0)
	(match_operand:HI 0 "nonimmediate_operand" "g"))]
  ""
  {
    tms9900_debug_operands ("tsthi", insn, operands, 1);
    return("; HI:%0 == 0?");
  }
)


;;-------------------------------------
(define_insn "tstqi"
  [(set (cc0)
	(match_operand:QI 0 "nonimmediate_operand" "g"))]
  ""
  {
    tms9900_debug_operands ("tstqi", insn, operands, 1);
    return("; QI:%0 == 0?");
  }
)
 
;;-------------------------------------
(define_insn "cmphi"
  [(set (cc0)
	(compare (match_operand:HI 0 "nonimmediate_operand" "g")
		 (match_operand:HI 1 "general_operand"      "g")))]
  ""
  {
    tms9900_debug_operands ("cmphi", insn, operands, 2);
    return("; compare(HI:%0,HI:%1)");
  }
)

; mov r1,@x 
; cmp r2, r1

; cmp r2,@x

(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
	(match_operand:HI 1 "memory_operand" ""))
   (set (cc0)
        (compare (match_operand:HI 2 "register_operand" "")
                 (match_dup 0)))]
  ""
  [(set (cc0) 
        (compare (match_dup 2)
                 (match_dup 1)))]
  "")


;;-------------------------------------
;
; Use r0 as a temp to hold an immediate and then do a compare byte
;
; NOTE - for cmpqi and movqi the immediate constraint is 'n' not 'i' as it must
;        be a const-int not a label
;
(define_insn "cmpqi"
  [(set (cc0)
	(compare (match_operand:QI 0 "nonimmediate_operand" "g")
		 (match_operand:QI 1 "general_operand"  "g")))]
  ""
  {
    tms9900_debug_operands ("cmpqi", insn, operands, 2);
    return("; compare(QI:%0,QI:%1)");
  }
)

;;-------------------------------------------------------------------
;;  Move Operations
;;-------------------------------------------------------------------


;;-------------------------------------------------------------------
;; Move byte value
(define_insn "movqi"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=g")
        (match_operand:QI 1 "general_operand"      "g"))]
  ""
  {
    tms9900_debug_operands ("movqi", insn, operands, 2);
    tms9900_operand_subreg_offset (operands[1], QImode);

    return ("; QI:%0 = QI:%1");
  }
)

;;-------------------------------------------------------------------
;; Move two-byte value
;;
;; Constraints O and M are listed as separate alternatives to allow SETO or CLR
;; to be used which would have a different length to LI.  SETO and CLR can also
;; work with R and Q.

(define_insn "movhi"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=g")
        (match_operand:HI 1 "general_operand"      "g"))]
  ""
  {
    tms9900_debug_operands ("movhi", insn, operands, 2);
    tms9900_operand_subreg_offset (operands[1], HImode);

    return ("; HI:%0 = HI:%1");
  }
)

;;-------------------------------------------------------------------
;; Move four-byte value as two 2-byte regs.  If immediate, emit two movhi insns
;; with high and low part of constants.  Constraints O and M are allowed so we
;; can issue seto or clr for -1 and 0 respectively
;;
;; Changed this from an expand back to an insn
;;-------------------------------------------------------------------
(define_insn "movsi"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=g")
	(match_operand:SI 1 "general_operand"      "g"))]
  ""
  {
    tms9900_debug_operands ("movsi", insn, operands, 2);
    return ("; SI:%0 = SI:%1");
  }
)

;;-------------------------------------------------------------------
;; Type Conversions
;;-------------------------------------------------------------------


;;-------------------------------------------------------------------
;; Zero Extend
;; If op1 is not a register, we need to mov the value to op0 first.  As a
;; result, R and Q constraints are different length to r.
;;-------------------------------------

(define_insn "zero_extendqihi2"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=g")
	(zero_extend:HI (match_operand:QI 1 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("zero_extendqihi2", NULL_RTX, operands, 2);
    return ("; HI:%0 = zero_extend(QI:%1)");
  }
)


;;-------------------------------------
; TODO Could do a gen_lowpart into op0 to prevent the tmp
(define_insn "zero_extendqisi2"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=g")
	(zero_extend:SI (match_operand:QI 1 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("zero_extendqisi2", NULL_RTX, operands, 2);
    return ("; SI:%0 = zero_extend(QI:%1)");
  }
)

(define_insn "zero_extendhisi2"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=g")
	(zero_extend:SI (match_operand:HI 1 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("zero_extendhisi2", insn, operands, 2);
    return ("; SI:%0 = zero_extend(HI:%1)");
  }
)


;;-------------------------------------------------------------------
;; Sign Extend
;;-------------------------------------------------------------------

(define_insn "extendqihi2"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=g")
	(sign_extend:HI (match_operand:QI 1 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("extendqihi2", insn, operands, 2);
    return ("; HI:%0 = extend(QI:%1)");
  }
)
			 

(define_insn "extendqisi2"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=g")
	(sign_extend:SI (match_operand:QI 1 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("extendqisi2", NULL_RTX, operands, 2);
    return ("; SI:%0 = extend(QI:%1)");
  }
)

;;-------------------------------------
(define_insn "extendhisi2"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=g")
	(sign_extend:SI (match_operand:HI 1 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("extendhisi2", insn, operands, 2);
    return ("; SI:%0 = extend(HI:%1)");
  }
)

;;-------------------------------------------------------------------
;; Truncate
;
;  swpb will suffice for non-strict in place truncation.  There is no strict variant of
;  truncate so I am assuming we doesn't care about unused bits.
;;-------------------------------------

(define_insn "trunchiqi2"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=g")
	(truncate:QI (match_operand:HI 1 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("trunchiqi2", insn, operands, 2);
    return ("; QI:%0 = truncate(HI:%1)");
  }
)

(define_insn "truncsihi2"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=g")
	(truncate:HI (match_operand:SI 1 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("truncsihi2", insn, operands, 2);
    return ("; HI:%0 = truncate(SI:%1)");
  }
)

(define_insn "truncsiqi2"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=g")
	(truncate:QI (match_operand:SI 1 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("truncsiqi2", NULL_RTX, operands, 2);
    return ("; QI:%0 = truncate(SI:%1)");
  }
)

;;-------------------------------------------------------------------
;;  Branch Instructions
;;-------------------------------------------------------------------


;;-------------------------------------------------------------------
;; Jump if equal
(define_insn "beq"
  [(set (pc)
	(if_then_else (eq (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    tms9900_debug_operands ("beq", insn, operands, 1);
    return ("; beq    %0");
  }
)

;;-------------------------------------------------------------------
;; Jump if not equal
(define_insn "bne"
  [(set (pc)
	(if_then_else (ne (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    tms9900_debug_operands ("bne", insn, operands, 1);
    return ("; bne    %0");
  }
)

;;-------------------------------------------------------------------
;; Jump if unsigned less than
(define_insn "bltu"
  [(set (pc)
	(if_then_else (ltu (cc0)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    tms9900_debug_operands ("bltu", insn, operands, 1);
    return ("; bltu   %0");
  }
)

;;-------------------------------------------------------------------
;; Jump if unsigned less than or equal
(define_insn "bleu"
  [(set (pc)
	(if_then_else (leu (cc0)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    tms9900_debug_operands ("bleu", insn, operands, 1);
    return ("; bleu   %0");
  }
)

;;-------------------------------------------------------------------
;; Jump if unsigned greater than
(define_insn "bgtu"
  [(set (pc)
	(if_then_else (gtu (cc0)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    tms9900_debug_operands ("bgtu", insn, operands, 1);
    return ("; bgtu   %0");
  }
)

;;-------------------------------------------------------------------
;; Jump if unsigned greater than or equal
(define_insn "bgeu"
  [(set (pc)
	(if_then_else (geu (cc0)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    tms9900_debug_operands ("bgeu", insn, operands, 1);
    return ("; bgeu   %0");
  }
)

;;-------------------------------------------------------------------
;; Jump if less than
(define_insn "blt"
  [(set (pc)
	(if_then_else (lt (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    tms9900_debug_operands ("blt", insn, operands, 1);
    return ("; blt    %0");
  }
)

;;-------------------------------------------------------------------
;; Jump if less than or equal
(define_insn "ble"
  [(set (pc)
	(if_then_else (le (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    tms9900_debug_operands ("ble", insn, operands, 1);
    return ("; ble    %0");
  }
)

;;-------------------------------------------------------------------
;; Jump if greater than
(define_insn "bgt"
  [(set (pc)
	(if_then_else (gt (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    tms9900_debug_operands ("bgt", insn, operands, 1);
    return ("; bgt    %0");
  }
)

;;-------------------------------------------------------------------
;; Jump if greater than or equal
; Was commented out
(define_insn "bge"
  [(set (pc)
	(if_then_else (ge (cc0)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    tms9900_debug_operands ("bge", insn, operands, 1);
    return ("; bge    %0");
  }
)


;;-------------------------------------------------------------------
;;  Jump Operations
;;-------------------------------------------------------------------


;;-------------------------------------------------------------------
;; Unconditional jump to label
(define_insn "jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  ""
  {
    tms9900_debug_operands ("jump", insn, operands, 1);
    return ("jmp    %0");
  }
)

;;-------------------------------------------------------------------
;; Unconditional jump using pointer
(define_insn "indirect_jump"
  [(set (pc) (match_operand:HI 0 "general_operand" ""))]
  ""
  {
    tms9900_debug_operands ("indirect_jump", insn, operands, 1);
    return "; indirect jump %0";
  }
)


;;-------------------------------------------------------------------
;; Unconditional jump using jump table
(define_insn "tablejump"
  [(clobber (match_scratch:HI 2 "=r"))
   (set (pc) (match_operand:HI 0 "general_operand" "g"))
   (use (label_ref (match_operand 1 "" "")))]
  ""
  {
    tms9900_debug_operands ("tablejump", insn, operands, 1);
    output_asm_insn("mov  %0, %2", operands);
    output_asm_insn("b    *%2",    operands);
    return(""); 
  }
)


;;-------------------------------------------------------------------
;;  Bit Shift Operations
;;-------------------------------------------------------------------

;; NOTE : On the TMS9900 a shift count of 0 is interpreted as
;; shift by 16, so test for 0 and jump over the shift if true.  Do the same in
;; all other shift by register insns as well.

;; Accept any shift count but if it is not a constant, load it into r0 first.

;; If we are asked to shift by a constant 0 then the
;; compiler is being stupid so detect this (constraint 'O') and emit nothing

;;-------------------------------------------------------------------
;; Arithmetic shift left.
;;
;;-------------------------------------------------------------------
(define_insn "ashlhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand" "")
	(ashift:HI (match_operand:HI 1 "general_operand" "")
		   (match_operand:HI 2 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("ashlhi3", insn, operands, 3);
    return("; ashlhi %0 = %1 << %2"); 
  }
)


;;-------------------------------------
(define_insn "ashlqi3"
  [(set (match_operand:QI 0 "nonimmediate_operand" "")
        (ashift:QI (match_operand:QI 1 "general_operand" "")
                   (match_operand:HI 2 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("ashlqi3", insn, operands, 3);
    return("; ashlqi %0 = %1 << %2"); 
  }
)



;;-------------------------------------
(define_insn "ashrhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand" "")
	(ashiftrt:HI (match_operand:HI 1 "general_operand" "")
		     (match_operand:HI 2 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("ashrhi3", insn, operands, 3);
    return("; ashrhi %0 = %1 >> %2"); 
  }
)


;;-------------------------------------
(define_insn "ashrqi3"
  [(set (match_operand:QI 0 "nonimmediate_operand" "")
	(ashiftrt:QI (match_operand:QI 1 "general_operand" "")
		     (match_operand:HI 2 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("ashrqi3", insn, operands, 3);
    return("; ashrqi %0 = %1 >> %2"); 
  }
)


;;-------------------------------------------------------------------
;; Logical shift right



;;-------------------------------------
(define_insn "lshrhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand" "")
	(lshiftrt:HI (match_operand:HI 1 "general_operand" "")
		     (match_operand:HI 2 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("lshrhi3", insn, operands, 3);
    return("; lshrhi %0 = %1 >> %2"); 
  }
)

; shift right 8 followed by truncate (swpb) negate each other so remove

(define_peephole2
  [(set (match_operand:HI 0 "register_operand" "")
	(lshiftrt:HI (match_dup 0)
		     (const_int 8)))
   (set (match_operand:QI 1 "register_operand" "")
	(truncate:QI(match_dup 0)))
   (set (match_operand:QI 2 "nonimmediate_operand" "")
	(match_dup 1))]
  ""
  [(set (match_dup 2)
	(subreg:QI (match_dup 0) 1))]
  "")
      


;;-------------------------------------
(define_insn "lshrqi3"
  [(set (match_operand:QI 0 "nonimmediate_operand" "")
	(lshiftrt:QI (match_operand:QI 1 "general_operand" "")
		     (match_operand:HI 2 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("lshrqi3", insn, operands, 3);
    return("; lshrqi %0 = %1 >> %2"); 
  }
)



;;-------------------------------------------------------------------
;; Rotate 


;;-------------------------------------
(define_insn "rotlhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand" "")
        (rotatert:HI (match_operand:HI 1 "general_operand" "")
                     (match_operand:HI 2 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("rotlhi3", NULL_RTX, operands, 3);
    return("; rotlhi %0 = %1 >> %2"); 
  })


;;-------------------------------------------------------------------
;;  Bitwise Operations
;;-------------------------------------------------------------------

;;-------------------------------------------------------------------
;; And
;;
;; Uses r0 as a clobber
;;-------------------------------------
(define_insn "andhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand"         "=g")
	(and:HI (match_operand:HI 1 "general_operand" "0")
		(match_operand:HI 2 "general_operand"      "g")))]
  ""
  {
    tms9900_debug_operands ("andhi3", insn, operands, 3);
    tms9900_operand_subreg_offset (operands[1], HImode);
    tms9900_operand_subreg_offset (operands[2], HImode);

    return("; HI:%0 = HI:%1 & HI:%2"); 
  }
)


;;-------------------------------------
;;
;; NOTE - we assume that constraint 'i' will only pass a CONST_INT as labels
;; etc don't make sense in the context of a byte operation.  Assert if assumption
;; not true.  If the constant evaluates to 0x00 or 0xFF then apply a short circuit.
;; This will mean length is shorter than declared but that should not matter.
;;
(define_insn "andqi3"
  [(set (match_operand:QI 0 "nonimmediate_operand"         "=g")
	(and:QI (match_operand:QI 1 "general_operand" "0")
		(match_operand:QI 2 "general_operand"      "g")))]
  ""
  {
    tms9900_debug_operands ("andqi3", insn, operands, 3);
    tms9900_operand_subreg_offset (operands[1], QImode);
    tms9900_operand_subreg_offset (operands[2], QImode);

    return("; QI:%0 = QI:%1 & QI:%2"); 
  }
)



;;-------------------------------------------------------------------
;; Or


;;-------------------------------------
(define_insn "iorhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand"         "=g")
	(ior:HI (match_operand:HI 1 "general_operand" "0")
		(match_operand:HI 2 "general_operand"      "g")))]
  ""
  {
    tms9900_debug_operands ("iorhi3", insn, operands, 3);
    tms9900_operand_subreg_offset (operands[1], HImode);
    tms9900_operand_subreg_offset (operands[2], HImode);

    return("; HI:%0 = HI:%1 | HI:%2"); 
  }
)


;;-------------------------------------
(define_insn "iorqi3"
   [(set (match_operand:QI 0 "nonimmediate_operand"        "=g")
	(ior:QI (match_operand:QI 1 "general_operand" "0")
		(match_operand:QI 2 "general_operand"      "g")))]
  ""
  {
    tms9900_debug_operands ("iorqi3", insn, operands, 3);
    tms9900_operand_subreg_offset (operands[1], QImode);
    tms9900_operand_subreg_offset (operands[2], QImode);

    return("; QI:%0 = QI:%1 | QI:%2"); 
  }
)


;;-------------------------------------------------------------------
;; Xor


;;-------------------------------------
(define_insn "xorhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=g")
	(xor:HI (match_operand:HI 1 "general_operand" "0")
		(match_operand:HI 2 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("xorhi3", insn, operands, 3);
    tms9900_operand_subreg_offset (operands[1], HImode);
    tms9900_operand_subreg_offset (operands[2], HImode);

    return "; HI:%0 = (HI:%1 ^ HI:%2)";
  }
)


;;-------------------------------------
;; This works for regs only as xor is 16-bit only and should not be used on
;; memory locations as it will affect the neighbouring byte
(define_insn "xorqi3"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=g")
	(xor:QI (match_operand:QI 1 "general_operand" "0")
		(match_operand:QI 2 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("xorqi3", insn, operands, 3);
    tms9900_operand_subreg_offset (operands[1], QImode);
    tms9900_operand_subreg_offset (operands[2], QImode);

    return("; QI:%0 = QI:%1 ^ QI:%2"); 
  }
)


;;-------------------------------------------------------------------
;; Not
;;-------------------------------------
(define_insn "one_cmplhi2"
  [(set (match_operand:HI 0 "nonimmediate_operand" "")
        (not:HI (match_operand:HI 1 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("one_cmplhi2", insn, operands, 3);
    return("; HI:%0 =~ %1"); 
  }
)


;;-------------------------------------
;; This works for regs only as inv is 16-bit only and should not be used on
;; memory locations as it will affect the neighbouring byte
(define_insn "one_cmplqi2"
  [(set (match_operand:QI 0 "nonimmediate_operand" "")
        (not:QI (match_operand:QI 1 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("one_cmplqi2", insn, operands, 3);
    return("; QI:%0 =~ %1"); 
  }
)


;;-------------------------------------------------------------------
;;  Arithmetic Operations
;;-------------------------------------------------------------------


;;-------------------------------------------------------------------
;; Add

; NOTES:
; Without "0" constraint, emits z = x + y which is no good on tms

;;-------------------------------------
(define_insn "addhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand"          "=g")
	(plus:HI (match_operand:HI 1 "general_operand" "0")
		 (match_operand:HI 2 "general_operand"      "g")))]
  ""
  {
    tms9900_debug_operands ("addhi3", insn, operands, 3);
    tms9900_operand_subreg_offset (operands[1], HImode);
    tms9900_operand_subreg_offset (operands[2], HImode);

    return("; HI:%0 = HI:%1 + HI:%2"); 
  }
)


;;-------------------------------------
(define_insn "addqi3"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=g")
	(plus:QI (match_operand:QI 1 "general_operand" "0")
		 (match_operand:QI 2 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("addqi3", insn, operands, 3);
    tms9900_operand_subreg_offset (operands[1], QImode);
    tms9900_operand_subreg_offset (operands[2], QImode);

    return("; QI:%0 = QI:%1 + QI:%2"); 
  }
)


;;-------------------------------------------------------------------
;; Subtract
;;-------------------------------------
(define_insn "*sub_extend"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=g")
	(minus:HI (sign_extend:HI (match_operand:QI 1 "general_operand" "0"))
		  (match_operand:HI 2 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("subhextend", insn, operands, 3);
    return("; HI:%0 = extend(QI:%1) - HI:%2"); 
  }
)

(define_insn "subhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=g")
	(minus:HI (match_operand:HI 1 "general_operand" "0")
		  (match_operand:HI 2 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("subhi3", insn, operands, 3);
    tms9900_operand_subreg_offset (operands[1], HImode);
    tms9900_operand_subreg_offset (operands[2], HImode);

    return("; HI:%0 = HI:%1 - HI:%2"); 
  }
)

(define_insn "subqi3"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=g")
	(minus:QI (match_operand:QI 1 "general_operand" "0")
		  (match_operand:QI 2 "general_operand" "g")))]
  ""
  {
    tms9900_debug_operands ("subqi3", insn, operands, 3);
    tms9900_operand_subreg_offset (operands[1], QImode);
    tms9900_operand_subreg_offset (operands[2], QImode);

    return("; QI:%0 = QI:%1 - QI:%2"); 
  }
)


;;-------------------------------------------------------------------
;; Multiply
;;
;;-------------------------------------
; Expand for signed HI = QI x QI
; TODO check sign of highpart of HI
(define_insn "mulqihi3"
   [(set (match_operand:HI 0 "nonimmediate_operand" "")
        (mult:HI (match_operand:QI 1 "general_operand" "")
                 (match_operand:QI 2 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("mulqihi3", NULL_RTX, operands, 3);
    return("; mulqihi %0 %1 %2"); 
  }
)


;;-------------------------------------
; Expand for signed SI = HI x HI
;
(define_insn "mulhisi3"
  [(set (match_operand:SI 0 "nonimmediate_operand" "")
	(mult:SI (zero_extend (match_operand:HI 1 "general_operand" ""))
		 (match_operand:HI 2 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("mulqihi3", NULL_RTX, operands, 3);
    return("; mulhisi %0 %1 %2"); 
  }
)


;;-------------------------------------
; expand HI=HIxHI to native SI=SIxHI

(define_insn "mulhi3"
  [(set (match_operand:HI 0 "nonimmediate_operand" "")
        (mult:HI (match_operand:HI 1 "general_operand" "")
                 (match_operand:HI 2 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("mulhi3", NULL_RTX, operands, 3);
    return("; mulhi %0 %1 %2"); 
   }
)

;;-------------------------------------
;; byte mul can only work in registers as we need to expand to HI mode

(define_insn "udivmodqi4"
  [(parallel [
     (set (match_operand:QI 0 "nonimmediate_operand" "")
          (div:QI (match_operand:QI 1 "general_operand" "")
                  (match_operand:QI 2 "general_operand" "")))
     (set (match_operand:QI 3 "nonimmediate_operand" "")
          (mod:QI (match_dup 1) 
                  (match_dup 2)))])]
  ""
  {
    tms9900_debug_operands ("udivmodqi4", NULL_RTX, operands, 4);
    return("; udivmodqi4 %0 %1 %2 %3"); 
  })

(define_insn "udivmodhi4"
  [(parallel [
     (set (match_operand:HI 0 "nonimmediate_operand" "")
          (div:HI (match_operand:HI 1 "general_operand" "")
                  (match_operand:HI 2 "general_operand" "")))
     (set (match_operand:HI 3 "nonimmediate_operand" "")
          (mod:HI (match_dup 1) 
                  (match_dup 2)))])]
  ""
  {
    tms9900_debug_operands ("udivmodhi4", NULL_RTX, operands, 4);
    return("; udivmodhi4 %0 %1 %2 %3"); 
  })


;;-------------------------------------------------------------------
;; Divide and Modulus
;; native method HI = SI / HI , HI = SI % HI
;;-------------------------------------
(define_insn "udivmodsihi3"
  [(set (match_operand:SI 0 "nonimmediate_operand" "")
        (ior:SI
          (ashift:SI
            (zero_extend:SI
              (div:HI (match_operand:SI 1 "general_operand" "")
                      (match_operand:HI 2 "general_operand" "")))
            (const_int 16))
          (zero_extend:SI 
            (mod:HI (match_dup 1)
                    (match_dup 2)))))]
  ""
  {
    tms9900_debug_operands ("udivmodsihi3", insn, operands, 3);
    return("; udivmodsi4 %0 %1 %2 %3"); 
  }
)



;;-------------------------------------------------------------------
;; Absolute Value

;;-------------------------------------
(define_insn "abshi2"
  [(set (match_operand:HI 0 "nonimmediate_operand" "")
	(abs:HI (match_operand:HI 1 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("abshi2", insn, operands, 2);
    return("abs %0 %1"); 
  }
)


;;-------------------------------------
;; register only as abs is 16-bit
(define_insn "absqi2"
  [(set (match_operand:QI 0 "nonimmediate_operand" "")
	(abs:QI (match_operand:QI 1 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("absqi2", insn, operands, 2);
    return("; absqi %0 %1"); 
  }
)


;;-------------------------------------------------------------------
;; Negate

;;-------------------------------------
(define_insn "neghi2"
  [(set (match_operand:HI 0 "nonimmediate_operand" "")
	(neg:HI (match_operand:HI 1 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("neghi2", insn, operands, 2);
    return("; neghi %0 %1"); 
  }
)


;;-------------------------------------
;; TODO andi only reqiured for strict
(define_insn "negqi2"
  [(set (match_operand:QI 0 "nonimmediate_operand" "")
	(neg:QI (match_operand:QI 1 "general_operand" "")))]
  ""
  {
    tms9900_debug_operands ("negqi2", insn, operands, 2);
    return("; negqi %0 %1"); 
  }
)


;;-------------------------------------------------------------------
;; Other Instructions
;;-------------------------------------------------------------------


;;-------------------------------------------------------------------
;; No-op
(define_insn "nop"
  [(const_int 0)]
  ""
  "nop"
)

