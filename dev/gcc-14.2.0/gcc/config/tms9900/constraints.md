;;- Constraint definitions for the tms9900 for GNU C compiler
;; Copyright (C) 2010-2023 Free Software Foundation, Inc.
;; Contributed by Mark Burkley (mark@burkley.net)

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

;; Description of class IDs
;;   'I' is for 32-bit value xxxx0000
;;   'J' is for 32-bit value 0000xxxx
;;   'K' is for 32-bit value xxxxxxxx
;;   'L' is for 2 or -2
;;   'M' is for -1
;;   'N' is for 1
;;   'O' is for 0
;;   'P' is for 00FF
;;   'Q' is for memory references that need an extra word after the opcode.
;;   'R' is for memory references which are encoded within the opcode.
;;   'S' is for shift register
;;   'T' Class of all registers


(define_constraint "I"
  "Integer constant that fits in 16 bits unsigned"
  (and (match_code "const_int")
       (match_test "(ival & 0xffff0000) == 0")))

(define_constraint "J"
  "Integer constant whose low 16 bits are zero"
  (and (match_code "const_int")
       (match_test "(ival & 0xffff) == 0")))

(define_constraint "K"
  "Integer constant whose lower and upper 16 bit half are both non-zero"
  (and (match_code "const_int")
       (match_test "(ival & 0xffff) != 0 && (ival & 0xffff0000) != 0")))

(define_constraint "L"
  "Integer constant 1"
  (and (match_code "const_int")
       (match_test "ival == 1")))

(define_constraint "M"
  "Integer constant -1"
  (and (match_code "const_int")
       (match_test "ival == -1")))

(define_constraint "N"
  "Integer constant 0"
  (and (match_code "const_int")
       (match_test "ival == 0")))

(define_constraint "O"
  "Integer constant 2 or -2"
  (and (match_code "const_int")
       (ior (match_test "ival == 2")
            (match_test "ival == -2"))))

(define_memory_constraint "Q"
  "Memory reference that requires an additional word after the opcode"
  (and (match_code "mem")
       (match_test "memory_address_p (GET_MODE (op), XEXP (op, 0))
                    && !simple_memory_operand (op, GET_MODE (op))")))

(define_memory_constraint "R"
  "Memory reference that is encoded within the opcode"
  (and (match_code "mem")
       (match_test "memory_address_p (GET_MODE (op), XEXP (op, 0))
                    && simple_memory_operand (op, GET_MODE (op))")))

(define_register_constraint "S" "SHIFT_REGS"
  "Shift amount register.")
