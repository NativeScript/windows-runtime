;/* -----------------------------------------------------------------------
;   sysv.S - Copyright (c) 1998, 2008, 2011 Red Hat, Inc.
;	    Copyright (c) 2011 Plausible Labs Cooperative, Inc.
;   
;   ARM Foreign Function Interface 
;
;   Permission is hereby granted, free of charge, to any person obtaining
;   a copy of this software and associated documentation files (the
;   ``Software''), to deal in the Software without restriction, including
;   without limitation the rights to use, copy, modify, merge, publish,
;   distribute, sublicense, and/or sell copies of the Software, and to
;   permit persons to whom the Software is furnished to do so, subject to
;   the following conditions:
;
;   The above copyright notice and this permission notice shall be included
;   in all copies or substantial portions of the Software.
;
;   THE SOFTWARE IS PROVIDED ``AS IS'', WITHOUT WARRANTY OF ANY KIND,
;   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
;   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
;   NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
;   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
;   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
;   DEALINGS IN THE SOFTWARE.
;   ----------------------------------------------------------------------- */

	include kxarm.h

    EXPORT	ffi_call_SYSV
    EXPORT	ffi_closure_SYSV
    EXPORT	ffi_call_VFP
    EXPORT	ffi_closure_VFP
    EXPORT	ffi_arm_trampoline
    EXPORT	ffi_thumb_trampoline
    IMPORT  ffi_prep_args
    IMPORT  ffi_closure_SYSV_inner
	
FFI_TYPE_VOID              EQU    0    
FFI_TYPE_INT               EQU    1
FFI_TYPE_FLOAT             EQU    2    
FFI_TYPE_DOUBLE            EQU    3
FFI_TYPE_LONGDOUBLE        EQU    4
FFI_TYPE_UINT8             EQU    5   
FFI_TYPE_SINT8             EQU    6
FFI_TYPE_UINT16            EQU    7 
FFI_TYPE_SINT16            EQU    8
FFI_TYPE_UINT32            EQU    9
FFI_TYPE_SINT32            EQU    10
FFI_TYPE_UINT64            EQU    11
FFI_TYPE_SINT64            EQU    12
FFI_TYPE_STRUCT            EQU    13
FFI_TYPE_POINTER           EQU    14
FFI_TYPE_STRUCT_VFP_FLOAT  EQU    15
FFI_TYPE_STRUCT_VFP_DOUBLE EQU    16


	TEXTAREA
	
	ALIGN 4
	LEAF_ENTRY	ffi_thumb_trampoline
	
		stmfd	sp!, {r0-r3}
	
		; pc = current instruction's address + 4 in thumb mode
	
		; Load *codeloc to r0
		ldr		r0, [pc, #8] ; 2 bytes
		mov		r0, r0		 ; 2 bytes padding
		; Clear the last bit that was set in thumb to get 
		; closure's address
		sub		r0, r0, #1   ; 4 bytes
	
		; Jump to *closure_func
		ldr		pc, [pc, #4] ; 4 bytes
		;DCD *codeloc		 ; 4 bytes
		;DCD *closure_func: ffi_closure_VFP/SYSV
	
	LEAF_END	ffi_thumb_trampoline


	LEAF_ENTRY	ffi_thumb_trampoline_use_ip

		; try this tramp later
		mov		ip, pc
		sub		ip, ip, #5
		ldr		pc, [pc]

	LEAF_END	ffi_thumb_trampoline_use_ip

	LEAF_ENTRY	ffi_arm_trampoline
	
		stmfd	sp!, {r0-r3}
	
		; pc = current instruction's address + 8 in arm mode
	
		; Load *codeloc to r0
		ldr		r0, [pc]
		
		; Jump to *closure_func
		ldr		pc, [pc] ; 4 bytes

		;DCD *codeloc
		;DCD *closure_func: ffi_closure_VFP/SYSV

	LEAF_END	ffi_arm_trampoline

;extern int 
;ffi_call_SYSV (void (*fn)(void), 
;				extended_cif *, 
;				unsigned bytes, 
;				unsigned flags, 
;				unsigned *rvalue);    
;@ r0:   fn
;@ r1:   &ecif
;@ r2:   cif->bytes
;@ r3:   cif->flags
;@ sp+0: ecif.rvalue
	NESTED_ENTRY	ffi_call_SYSV
		;@ Save registers
		PROLOG_PUSH	{r0-r3}
		PROLOG_PUSH {r4, r5, r11, lr}
		PROLOG_STACK_SAVE	r11
    
		;@ r0:   fn             : r11 + 16
		;@ r1:   &ecif          : r11 + 20
		;@ r2:   cif->bytes     : r11 + 24
		;@ r3:   cif->flags     : r11 + 28
		;@ sp+0: ecif.rvalue    : r11 + 32

		;@ Make room for all of the new args.
		sub     sp, sp, r2

		;@ Place all of the ffi_prep_args in position
		mov	    r0, sp
		;@     r1 already set

		;@ Call ffi_prep_args(stack, &ecif)
		bl	    ffi_prep_args

		;@ move first 4 parameters in registers
		ldmia	sp, {r0-r3}
    
		;@ and adjust stack, i.e., clearing argument we loaded to regs
		;@ cif->bytes == r11 - sp
		mov     r4, sp
		sub     r5, r11, r4
		cmp	    r5, #16
		movhs	r5, #16
		add	    sp, sp, r5
    
		;@ load address of fn()
		ldr	    lr, [r11, #16]	
		;@ call (fn) (...)
		blx     lr
	
		;@ Load r2 with the pointer to storage for the return value
		ldr	    r2, [r11, #32]

		;@ Load r3 with the return type code 
		ldr	    r3, [r11, #28]

		;@ If the return value pointer is NULL, assume no return value.
		cmp	    r2, #0
		beq	    |$Lepilogue@ffi_call_SYSV|
    
		cmp     r3, #FFI_TYPE_INT
		beq     |$LReturnInt@ffi_call_SYSV|
    
		cmp     r3, #FFI_TYPE_FLOAT
		beq     |$LReturnFloat@ffi_call_SYSV|
    
		cmp     r3, #FFI_TYPE_DOUBLE
		beq     |$LReturnDouble@ffi_call_SYSV|
    
		cmp     r3, #FFI_TYPE_SINT64
		beq     |$LReturnSInt64@ffi_call_SYSV|
    
		b       |$Lepilogue@ffi_call_SYSV| 

|$LReturnInt@ffi_call_SYSV|
		str     r0, [r2]
		b       |$Lepilogue@ffi_call_SYSV| 
    
|$LReturnFloat@ffi_call_SYSV|
		str     r0, [r2]
		b       |$Lepilogue@ffi_call_SYSV|
    
|$LReturnDouble@ffi_call_SYSV|
		stmia   r2, {r0, r1}
		b       |$Lepilogue@ffi_call_SYSV| 
    
|$LReturnSInt64@ffi_call_SYSV|
		stmia   r2, {r0, r1}
		b       |$Lepilogue@ffi_call_SYSV| 

|$Lepilogue@ffi_call_SYSV| 
		; clean local variable(s)
		EPILOG_STACK_RESTORE	r11
    
		; restore saved regs
		EPILOG_POP	{r4, r5, r11, lr}
		EPILOG_POP	{r0-r3}
	
		; CTYPES check the return value of this function whether it cleans the stack properly
		; Tell CTYPES it's alright because we have removed the space we pushed for the args
		;@ Remove the space we pushed for the args
		; EPILOG_NOP	mov     r0, #0
	
		EPILOG_RETURN
    
	NESTED_END		ffi_call_SYSV


;Import:
;unsigned int FFI_HIDDEN
;ffi_closure_SYSV_inner (closure, respp, args)
;     ffi_closure *closure;
;     void **respp;
;     void *args;

;The trampoline will jump to this function
	NESTED_ENTRY 	ffi_closure_SYSV
		; Stuff from ffi_thumb_trampoline
		;-----------------------------------------------------------;
		;stmfd   sp!, {r0-r3}      ; Save args
		;
		; pc = current instruction's address + 4 in thumb mode
		;
		; Load *codeloc to r0;
		;ldr     r0, [pc, #4]        ; 2 bytes
		;
		; 2 bytes padding so I wont need to change the C code
		;mov     r0, r0              ; 2 bytes
		;
		; Jump to ffi_closure_SYSV
		;ldr     pc, [pc, #4]        ; 4 bytes
		;DCD *codeloc
		;DCD *closure_func: ffi_closure_SYSV
  		;-----------------------------------------------------------;



		PROLOG_PUSH			{lr}
		; r2 stores location of args, +4 to skip saved lr
		PROLOG_NOP			add	r2,	sp,	#4
		; revert 4 words
		PROLOG_STACK_ALLOC	16
	
		; save sp to *(sp+8)
		str		sp,	[sp, #8]
		; r1 is the pointer to saved sp
		; basically, pointer to pointer to a location that stores results
		add		r1, sp, #8
		bl		ffi_closure_SYSV_inner
	
		cmp	    r0, #FFI_TYPE_INT
		beq	    |$LReturnInt@ffi_closure_SYSV|
    
		cmp	    r0, #FFI_TYPE_FLOAT
		beq	    |$LReturnFloat@ffi_closure_SYSV|
    
		cmp	    r0, #FFI_TYPE_DOUBLE
		beq	    |$LReturnDouble@ffi_closure_SYSV|
    
		cmp	    r0, #FFI_TYPE_LONGDOUBLE
		beq	    |$LReturnLongDouble@ffi_closure_SYSV|
    
		cmp	    r0, #FFI_TYPE_SINT64
		beq	    |$LReturnSInt64@ffi_closure_SYSV|
    
		b       |$Lepilogue@ffi_closure_SYSV|

|$LReturnInt@ffi_closure_SYSV|
		ldr     r0, [sp]
		b       |$Lepilogue@ffi_closure_SYSV|
    
|$LReturnFloat@ffi_closure_SYSV|
		ldr     r0, [sp] 
		b       |$Lepilogue@ffi_closure_SYSV|
    
|$LReturnDouble@ffi_closure_SYSV|
		ldr     r0, [sp]
		ldr     r1, [sp, #4]  
		b       |$Lepilogue@ffi_closure_SYSV|

|$LReturnLongDouble@ffi_closure_SYSV|
		ldr     r0, [sp]
		ldr     r1, [sp, #4] 
		b       |$Lepilogue@ffi_closure_SYSV|

|$LReturnSInt64@ffi_closure_SYSV|
		ldr     r0, [sp]
		ldr     r1, [sp, #4]
		b       |$Lepilogue@ffi_closure_SYSV|
    
|$Lepilogue@ffi_closure_SYSV|
		; clean local variable
		EPILOG_STACK_FREE	16
		; clean memory and restore SP
		; return
		EPILOG_POP		{lr}
		; free the values pushed by trampoline
		EPILOG_NOP		add	sp, sp, #16	
		EPILOG_RETURN
	
	NESTED_END		ffi_closure_SYSV
	

;extern int 
;ffi_call_VFP (void (*fn)(void), 
;				extended_cif *, 
;				unsigned bytes, 
;				unsigned flags, 
;				unsigned *rvalue);    
;@ r0:   fn
;@ r1:   &ecif
;@ r2:   cif->bytes
;@ r3:   cif->flags
;@ sp+0: ecif.rvalue
	NESTED_ENTRY	ffi_call_VFP
		;@ Save registers
		PROLOG_PUSH	{r0-r3}
		PROLOG_PUSH	{r4, r5, r11, lr}
		PROLOG_STACK_SAVE	r11

		;@ r0:   fn             : r11 + 16
		;@ r1:   &ecif          : r11 + 20
		;@ r2:   cif->bytes     : r11 + 24
		;@ r3:   cif->flags     : r11 + 28
		;@ sp+0: ecif.rvalue    : r11 + 32    
    
		;@ Make room for loading VFP args
		sub	    sp, sp, #64    
    
		;@ Make room for all of the new args.
		sub	    sp, sp, r2

		;@ Place all of the ffi_prep_args in position
		mov	    r0, sp
		;@     r1 already set
		sub	    r2, r11, #64   ;@ VFP scratch space

		;@ Call ffi_prep_args(stack, &ecif, &vfp_space)
		bl	    ffi_prep_args

		;@ VFP scratch space pointer
		sub	    r4, r11, #64    
    
		;@ Load VFP register args if needed
		cmp	    r0, #0
		beq	    |$Lbase_args@ffi_call_VFP|

		;@ r0: word bitset of VFP regs used for passing arguments. 
		; Load only d0 if possible
		cmp	    r0, #3          ; 3 == 11b 
		vldrle	d0, [r4]
		vldmiagt	r4, {d0-d7}

|$Lbase_args@ffi_call_VFP|
		;@ move first 4 parameters in registers
		ldmia	sp, {r0-r3}

		;@ and adjust stack
		;@ cif->bytes == (r11 - 64) - sp = r4 - sp
		mov     r5, sp
		sub     r4, r4, r5  

		; clearing variables we moved to regs
		cmp	    r4, #16
		movhs	r4, #16
		add	    sp, sp, r4

		;@ load address of fn() 
		ldr	    lr, [r11, #16]	
		;@ call (fn) (...)
		blx     lr

		;@ Load r2 with the pointer to storage for
		;@ the return value
		ldr	    r2, [r11, #32]

		;@ Load r3 with the return type code 
		ldr	    r3, [r11, #28]

		;@ If the return value pointer is NULL,
		;@ assume no return value.
		cmp	    r2, #0
		beq	    |$Lepilogue_vfp@ffi_call_VFP|

		cmp	    r3, #FFI_TYPE_INT
		streq	r0, [r2]
		beq	    |$Lepilogue_vfp@ffi_call_VFP|

		cmp	    r3, #FFI_TYPE_SINT64
		stmiaeq	r2, {r0, r1}
		beq	    |$Lepilogue_vfp@ffi_call_VFP|

		cmp	    r3, #FFI_TYPE_FLOAT
		vstreq	s0, [r2]
		beq	    |$Lepilogue_vfp@ffi_call_VFP|
	
		cmp	    r3, #FFI_TYPE_DOUBLE
		vstreq	d0, [r2]
		beq	    |$Lepilogue_vfp@ffi_call_VFP|

		cmp	    r3, #FFI_TYPE_STRUCT_VFP_FLOAT
		cmpne	r3, #FFI_TYPE_STRUCT_VFP_DOUBLE
		vstmiaeq	r2, {d0-d3}

|$Lepilogue_vfp@ffi_call_VFP|	
		; clean local variable(s)
		EPILOG_STACK_RESTORE	r11
    
		; restore saved regs
		EPILOG_POP				{r4, r5, r11, lr}
		EPILOG_POP				{r0-r3}
	
		; CTYPES check the return value of this function whether it cleans the stack properly
		; Tell CTYPES it's alright because we have removed the space we pushed for the args
		;@ Remove the space we pushed for the args
		; EPILOG_NOP				mov     r0, #0
	
		EPILOG_RETURN
	NESTED_END		ffi_call_VFP

;unsigned int
;	ffi_closure_SYSV_inner (ffi_closure *closure, 
;	void **respp, 
;	void *args,
;	void *vfp_args)
	NESTED_ENTRY	ffi_closure_VFP	
		; Stuff from ffi_thumb_trampoline
		;-----------------------------------------------------------;
		;stmfd   sp!, {r0-r3}      ; Save args
		;
		; pc = current instruction's address + 4 in thumb mode
		;
		; Load *codeloc to r0;
		;ldr     r0, [pc, #4]        ; 2 bytes
		;
		; 2 bytes padding so I wont need to change the C code
		;mov     r0, r0              ; 2 bytes
		;
		; Jump to ffi_closure_VFP
		;ldr     pc, [pc, #4]        ; 4 bytes
		;DCD *codeloc
		;DCD *closure_func: ffi_closure_VFP
  		;-----------------------------------------------------------;
		PROLOG_VPUSH		{d0-d7}
		PROLOG_PUSH			{r11, lr}
		PROLOG_NOP			add	    r2, sp, #72
		PROLOG_NOP			add	    r3, sp, #8
		PROLOG_STACK_ALLOC	72

		str	    sp, [sp, #64]
		add	    r1, sp, #64
		bl	    ffi_closure_SYSV_inner 

		cmp	    r0, #FFI_TYPE_INT
		beq	    |$Lretint_vfp@ffi_closure_VFP|

		cmp	    r0, #FFI_TYPE_FLOAT
		beq	    |$Lretfloat_vfp@ffi_closure_VFP|

		cmp	    r0, #FFI_TYPE_DOUBLE
		cmpne	r0, #FFI_TYPE_LONGDOUBLE
		beq	    |$Lretdouble_vfp@ffi_closure_VFP|

		cmp	    r0, #FFI_TYPE_SINT64
		beq	    |$Lretlonglong_vfp@ffi_closure_VFP|

		cmp	    r0, #FFI_TYPE_STRUCT_VFP_FLOAT
		beq	    |$Lretfloat_struct_vfp@ffi_closure_VFP|

		cmp	    r0, #FFI_TYPE_STRUCT_VFP_DOUBLE
		beq	    |$Lretdouble_struct_vfp@ffi_closure_VFP|
	
		b		|$Lclosure_epilogue_vfp@ffi_closure_VFP|

|$Lretfloat_vfp@ffi_closure_VFP|
		vldr	s0, [sp]
		b	    |$Lclosure_epilogue_vfp@ffi_closure_VFP|
|$Lretdouble_vfp@ffi_closure_VFP|
		vldr	d0, [sp]
		b	    |$Lclosure_epilogue_vfp@ffi_closure_VFP|
|$Lretint_vfp@ffi_closure_VFP|
		ldr	    r0, [sp]
		b	    |$Lclosure_epilogue_vfp@ffi_closure_VFP|
|$Lretlonglong_vfp@ffi_closure_VFP|
		ldmia	sp, {r0, r1}
		b	    |$Lclosure_epilogue_vfp@ffi_closure_VFP|
|$Lretfloat_struct_vfp@ffi_closure_VFP|
		vldmia	sp, {d0-d1}
		b	    |$Lclosure_epilogue_vfp@ffi_closure_VFP|
|$Lretdouble_struct_vfp@ffi_closure_VFP|
		vldmia	sp, {d0-d3}
		b	    |$Lclosure_epilogue_vfp@ffi_closure_VFP| 
	
|$Lclosure_epilogue_vfp@ffi_closure_VFP|
		EPILOG_STACK_FREE	72
		EPILOG_POP			{r11, lr}
		EPILOG_VPOP			{d0-d7}
		EPILOG_NOP			add sp, sp, #16 ; need to fix
		EPILOG_RETURN
	NESTED_END		ffi_closure_VFP		
	
	END
	
	


