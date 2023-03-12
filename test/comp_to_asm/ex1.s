	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 13, 0	sdk_version 13, 1
	.globl	__Z3funlP7context               ; -- Begin function _Z3funlP7context
	.p2align	2
__Z3funlP7context:                      ; @_Z3funlP7context
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	str	x0, [sp, #24]
	str	x1, [sp, #16]
	; InlineAsm Start
in_fun:
	; InlineAsm End
	ldr	x8, [sp, #16]
	str	x8, [sp, #8]
	ldr	x8, [sp, #24]
	ldr	x9, [sp, #8]
                                        ; kill: def $w8 killed $w8 killed $x8
	str	w8, [x9]
	mov	x8, #1
	str	x8, [sp, #24]
	ldr	x8, [sp, #24]
	ldr	x9, [sp, #8]
                                        ; kill: def $w8 killed $w8 killed $x8
	str	w8, [x9, #4]
	ldr	x8, [sp, #24]
	add	x0, x8, #14
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:
	sub	sp, sp, #80
	stp	x29, x30, [sp, #64]             ; 16-byte Folded Spill
	add	x29, sp, #64
	.cfi_def_cfa w29, 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	adrp	x8, ___stack_chk_guard@GOTPAGE
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x8, [x8]
	stur	x8, [x29, #-8]
	str	wzr, [sp, #28]
	; InlineAsm Start
start_main:
	; InlineAsm End
	mov	x8, #12
	str	x8, [sp, #16]
	; InlineAsm Start
before_context:
	; InlineAsm End
	; InlineAsm Start
before_call:
	; InlineAsm End
	mov	x0, #13
	add	x1, sp, #32
	bl	__Z3funlP7context
	str	x0, [sp, #8]
	; InlineAsm Start
after_call:
	; InlineAsm End
	ldr	x8, [sp, #8]
	mov	x9, sp
	str	x8, [x9]
	adrp	x0, l_.str@PAGE
	add	x0, x0, l_.str@PAGEOFF
	bl	_printf
	ldr	x8, [sp, #16]
	add	x8, x8, #1
	str	x8, [sp, #16]
	ldur	x9, [x29, #-8]
	adrp	x8, ___stack_chk_guard@GOTPAGE
	ldr	x8, [x8, ___stack_chk_guard@GOTPAGEOFF]
	ldr	x8, [x8]
	subs	x8, x8, x9
	b.eq	LBB1_2
	b	LBB1_1
LBB1_1:
	bl	___stack_chk_fail
LBB1_2:
	mov	w0, #0
	ldp	x29, x30, [sp, #64]             ; 16-byte Folded Reload
	add	sp, sp, #80
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__DATA,__data
	.globl	_z                              ; @z
	.p2align	3
_z:
	.quad	11                              ; 0xb

	.section	__TEXT,__cstring,cstring_literals
l_.str:                                 ; @.str
	.asciz	"%d\n"

.subsections_via_symbols
