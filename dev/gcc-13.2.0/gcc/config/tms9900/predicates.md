;;-------------------------------------------------------------------
;;  Predicate Definitions
;;-------------------------------------------------------------------

(define_predicate "shift_count_operand"
  (ior (match_code "const_int")
       (match_code "reg")))

;; Return true if OP is either a register or the constant 0.
; (define_predicate "reg_or_0_operand"
;   (ior (match_operand 0 "register_operand")
;        (match_operand 0 "const0_operand")))

