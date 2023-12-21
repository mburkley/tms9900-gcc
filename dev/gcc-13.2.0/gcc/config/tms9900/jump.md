;;-------------------------------------------------------------------
;;  Branch Instructions
;;-------------------------------------------------------------------


;;-------------------------------------------------------------------
;; Jump if equal
(define_insn "beq"
  [(set (pc)
	(if_then_else (eq (reg:CC CC_REGNUM)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  {
    return(output_branch("jeq", "jne", get_attr_length(insn)));
  }
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 12)))])


;;-------------------------------------
;; Jump if equal, reversed comparison operands
(define_insn "*beq_reversed"
  [(set (pc)
	(if_then_else (eq (reg:CC CC_REGNUM)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  {
    return(output_branch("jne", "jeq", get_attr_length(insn)));
  }
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 12)))])


;;-------------------------------------------------------------------
;; Jump if not equal
(define_insn "bne"
  [(set (pc)
	(if_then_else (ne (reg:CC CC_REGNUM)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "* return output_branch(\"jne\", \"jeq\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 12)))])


;;-------------------------------------
;; Jump if not equal, reversed comparison operands
(define_insn "*bne_reversed"
  [(set (pc)
	(if_then_else (ne (reg:CC CC_REGNUM)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "* return output_branch(\"jeq\", \"jne\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 12)))])


;;-------------------------------------------------------------------
;; Jump if unsigned less than
(define_insn "bltu"
  [(set (pc)
	(if_then_else (ltu (reg:CC CC_REGNUM)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "* return output_branch(\"jl\", \"jhe\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 12)))])


;;-------------------------------------
;; Jump if unsigned less than, reversed comparison operands
(define_insn "*bltu_reversed"
  [(set (pc)
	(if_then_else (ltu (reg:CC CC_REGNUM)
			   (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "* return output_branch(\"jhe\", \"jl\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 12)))])


;;-------------------------------------------------------------------
;; Jump if unsigned less than or equal
(define_insn "bleu"
  [(set (pc)
	(if_then_else (leu (reg:CC CC_REGNUM)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "* return output_branch(\"jle\", \"jh\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 12)))])


;;-------------------------------------
;; Jump if unsigned less than or equal, reversed comparison operands
(define_insn "*bleu_reversed"
  [(set (pc)
	(if_then_else (leu (reg:CC CC_REGNUM)
			   (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "* return output_branch(\"jh\", \"jle\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 12)))])


;;-------------------------------------------------------------------
;; Jump if unsigned greater than
(define_insn "bgtu"
  [(set (pc)
	(if_then_else (gtu (reg:CC CC_REGNUM)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "* return output_branch(\"jh\", \"jle\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 12)))])


;;-------------------------------------
;; Jump if unsigned greater than, reversed comparison operands
(define_insn "*bgtu_reversed"
  [(set (pc)
	(if_then_else (gtu (reg:CC CC_REGNUM)
			   (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "* return output_branch(\"jle\", \"jh\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 12)))])


;;-------------------------------------------------------------------
;; Jump if unsigned greater than or equal
(define_insn "bgeu"
  [(set (pc)
	(if_then_else (geu (reg:CC CC_REGNUM)
			   (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "* return output_branch(\"jhe\", \"jl\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 12)))])


;;-------------------------------------
;; Jump if unsigned greater than or equal, reversed comparison operands
(define_insn "*bgeu_reversed"
  [(set (pc)
	(if_then_else (geu (reg:CC CC_REGNUM)
			   (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "* return output_branch(\"jl\", \"jhe\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 12)))])


;;-------------------------------------------------------------------
;; Jump if less than
(define_insn "blt"
  [(set (pc)
	(if_then_else (lt (reg:CC CC_REGNUM)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "* return output_branch(\"jlt\", \"GE\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 18)
				      (const_int 12)))])


;;-------------------------------------
;; Jump if less than, reversed comparison operands
; Was commented out
(define_insn "*blt_reversed"
  [(set (pc)
	(if_then_else (lt (reg:CC CC_REGNUM)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "* return output_branch(\"GE\", \"jlt\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 14)))])


;;-------------------------------------------------------------------
;; Jump if less than or equal
(define_insn "ble"
  [(set (pc)
	(if_then_else (le (reg:CC CC_REGNUM)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "* return output_branch(\"LE\", \"jgt\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 14)))])


;;-------------------------------------
;; Jump if less than or equal, reversed comparison operands
(define_insn "*ble_reversed"
  [(set (pc)
	(if_then_else (le (reg:CC CC_REGNUM)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "* return output_branch(\"jgt\", \"LE\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 18)
				      (const_int 12)))])


;;-------------------------------------------------------------------
;; Jump if greater than
(define_insn "bgt"
  [(set (pc)
	(if_then_else (gt (reg:CC CC_REGNUM)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "* return output_branch(\"jgt\", \"LE\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 18)
				      (const_int 12)))])


;;-------------------------------------
;; Jump if greater than, reversed comparison operands
; Was commented out
(define_insn "*bgt_reversed"
  [(set (pc)
	(if_then_else (gt (reg:CC CC_REGNUM)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "* return output_branch(\"LE\", \"jgt\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 14)))])


;;-------------------------------------------------------------------
;; Jump if greater than or equal
; Was commented out
(define_insn "bge"
  [(set (pc)
	(if_then_else (ge (reg:CC CC_REGNUM)
			  (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "* return output_branch(\"GE\", \"jlt\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 16)
				      (const_int 14)))])


;;-------------------------------------
;; Jump if greater than or equal, reversed comparison operands
(define_insn "*bge_reversed"
  [(set (pc)
	(if_then_else (ge (reg:CC CC_REGNUM)
			  (const_int 0))
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
  ""
  "* return output_branch(\"jlt\", \"GE\", get_attr_length(insn));"
  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 18)
				      (const_int 12)))])


;;-------------------------------------------------------------------
;;  Jump Operations
;;-------------------------------------------------------------------


;;-------------------------------------------------------------------
;; Unconditional jump to label
(define_insn "jump"
  [(set (pc)
	(label_ref (match_operand 0 "" "")))]
  ""
  "*
  return output_jump(get_attr_length(insn));"

  [(set (attr "length") (if_then_else (ior (le (minus (match_dup 0)
						      (pc))
					       (const_int -252))
					   (ge (minus (match_dup 0)
						      (pc))
					       (const_int 256)))
				      (const_int 14)
				      (const_int 12)))])


;;-------------------------------------------------------------------
;; Unconditional jump using pointer
(define_insn "indirect_jump"
  [(set (pc) (match_operand:HI 0 "nonimmediate_operand" "r,Q"))]
  ""
  "b    %0"
  [(set_attr "length" "2,4")])


;;-------------------------------------------------------------------
;; Unconditional jump using jump table
(define_insn "tablejump"
  [(clobber (match_scratch:HI 2 "=r,r"))
   (set (pc) (match_operand:HI 0 "nonimmediate_operand" "rR,Q"))
   (use (label_ref (match_operand 1 "" "")))]
  ""
  {
    output_asm_insn("mov  %0, %2", operands);
    output_asm_insn("b    *%2",    operands);
    return(""); 
  }
  [(set_attr "length" "4,6")])



