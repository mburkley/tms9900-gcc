;;-------------------------------------------------------------------
;;  Function Calls
;;-------------------------------------------------------------------


;;-------------------------------------------------------------------
;; Jump to a subroutine which returns a value
(define_insn "call"
  [(call (match_operand:HI 0 "general_operand" "rR,Q")
         (match_operand:HI 1 "general_operand"  "g,g"))
  ]
  ""
  {
    if(SIBLING_CALL_P(insn))
      output_asm_insn("b    %0", operands);
    else
      output_asm_insn("bl   %0", operands);
    return("");
  }
  [(set_attr "length" "2,4")]
)


;-------------------------------------------------------------------
; Jump to a subroutine which returns a value
(define_insn "call_value"
  [(set (match_operand 0 "" "")
        (call (match_operand:HI 1 "general_operand" "rR,Q")
	      (match_operand:HI 2 "general_operand" "g,g")))
  ]
  ""
  {
    if(SIBLING_CALL_P(insn))
      output_asm_insn("b    %1", operands);
    else
      output_asm_insn("bl   %1", operands);
    return("");
  }
  [(set_attr "length" "2,4")]
)


;;-------------------------------------------------------------------
;; Define function prologue
(define_expand "prologue"
  [(const_int 0)]
  ""
{
  tms9900_expand_prologue();
  DONE;
})


;;-------------------------------------------------------------------
;; Define function epilogue
(define_expand "epilogue"
  [(return)]
  ""
{
  tms9900_expand_epilogue(false);
  DONE;
})

(define_expand "sibcall_epilogue"
  [(return)]
  ""
{
  tms9900_expand_epilogue(true);
  DONE;
})


;;-------------------------------------------------------------------
;; Define function return
(define_insn "*rt"
  [(unspec [(match_operand:HI 0 "general_operand" "")] UNSPEC_RETURN)]
  ""
  "b    *r11"
  [(set_attr "length" "2")])



