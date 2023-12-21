(define_constants
  [
   ;; Register numbers
   (R0_REGNUM     	  0)
   (R1_REGNUM     	  1)
   (R2_REGNUM     	  2)
   (R3_REGNUM     	  3)
   (R4_REGNUM     	  4)
   (R5_REGNUM     	  5)
   (R6_REGNUM     	  6)
   (R7_REGNUM     	  7)
   (R8_REGNUM     	  8)
   (R9_REGNUM     	  9)
   (R10_REGNUM     	  10)
   (R11_REGNUM     	  11)
   (R12_REGNUM     	  12)
   (R13_REGNUM     	  13)
   (R14_REGNUM     	  14)
   (R15_REGNUM     	  15)

   ;; ST, WP and PC are given pseudo reg numbers here too
   ;; Status register
   (ST_REGNUM	   	  16)
   ;; Workspace pointer
   (WP_REGNUM		  17)
   ;; Workspace pointer
   (PC_REGNUM		  18)

   ;; Several regs have alternate uses, define them here
   ;; Shift count register
   (SC_REGNUM     	  0)
   (RETVAL_REGNUM     	  1)
   (ARG_POINTER_REGNUM	  8)
   (FRAME_POINTER_REGNUM  9)
   (STACK_POINTER_REGNUM  10)
   ;; Old PC after BL instruction
   (LR_REGNUM		  11)
   ;; CRU base address
   (CB_REGNUM		  12)
   ;; Old workspace after BLWP instruction
   (LW_REGNUM		  13)
   ;; Old PC after BLWP instruction
   (LP_REGNUM		  14)
   ;; Old status register after BLWP instruction
   (LS_REGNUM		  15)
   ;; Condition code registers
   (CC_REGNUM             16)
   ;; End of hard registers
   (FIRST_PSEUDO_REGISTER 19)
   
   ;; Branch offset limits, as byte offsets from (pc).
   ;; TODO check these are valid for tms9900
   (MIN_BRANCH            -254)
   (MAX_BRANCH            254)])

;; SI is 32 bit
;; HI is 16 bit
;; QI is 8 bit 

;;- See file "rtl.def" for documentation on define_insn, match_*, et. al.

;;- cpp macro #define NOTICE_UPDATE_CC in file tm.h handles condition code
;;- updates for most instructions.

;;- Operand classes for the register allocator:

;; Compare instructions.


;; define attributes
;; currently type is only fpu or arith or unknown, maybe branch later ?
;; default is arith
(define_attr "type" "unknown,arith,fp" (const_string "arith"))

;; length default is 1 word each
;; MGB TODO is length bytes or words?
(define_attr "length" "" (const_int 1))

;; a user's asm statement
(define_asm_attributes
  [(set_attr "type" "unknown")
; all bets are off how long it is - make it 256, forces long jumps 
; whenever jumping around it !!!
   (set_attr "length" "256")])

;; MGB To address the issue where tms9900 stores bytes in the MSB position, all
;; QI loads to registers are right-shifted 8 bits before storage as a HI value

;;-------------------------------------------------------------------
;;  UNSPEC Definitions
;;-------------------------------------------------------------------
(define_constants
  [(UNSPEC_RETURN  0)])


;;-------------------------------------------------------------------
;;  Predicate Definitions
;;-------------------------------------------------------------------

(define_predicate "shift_count_operand"
  (ior (match_code "const_int")
       (match_code "reg")))

