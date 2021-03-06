;
; Copyright (C) 2015-2018 Night Dive Studios, LLC.
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
;
; $Source: r:/prj/lib/src/fix/RCS/fix.inc $
; $Revision: 1.12 $
; $Author: jaemz $
; $Date: 1994/08/29 13:29:14 $
;
; Fixed-point types and macros
; This file was hand-created, incorportating some of the output of 
; the h2inc translation of fix.h
;
; $Log: fix.inc $
; Revision 1.12  1994/08/29  13:29:14  jaemz
; Added fixmuldiv as an assembler macro, duh
; 
; Revision 1.11  1994/01/26  21:26:44  kevin
; Implemented Doug's speed fix in fixmul.  Should use old version for pentium, though.
; 
; Revision 1.10  1993/11/30  12:33:54  ept
; Changed macros f_cint and f_fint to use sar rather than shr.
; 
; Revision 1.9  1993/10/02  09:31:43  kaboom
; Took out argument check for fixdiv---didn't work for arguments of [reg].val.
; 
; Revision 1.8  1993/08/27  14:32:48  kaboom
; Added f_floor,f_ceil,f_fint,f_cint macros.
; 
; Revision 1.7  1993/04/26  13:43:14  matt
; Added some parens to fixmake() macro to keep MASM happy.
; 
; Revision 1.6  1993/04/09  10:47:04  matt
; Added externdef for fix_sqrt.
; 
; Revision 1.5  1993/04/08  19:02:12  matt
; Added externdef for fix_acos
; 
; Revision 1.4  1993/03/31  12:03:45  matt
; Fixed bug in rounding fixmul
; 
; Revision 1.3  1993/03/30  16:29:59  matt
; Added rounding version of fixmul
; 
; Revision 1.2  1993/03/29  13:56:54  matt
; Added fixmake macro and some fixed-point constants
; 
; Revision 1.1  1993/03/22  17:10:32  matt
; Initial revision
; 
;



IFNDEF	fix_inc
fix_inc	EQU	1


fix	TYPEDEF	SDWORD
fix16	TYPEDEF	fix
fixang	TYPEDEF	word

	externdef	fix_sincos:near	;takes bx, returns eax,ebx
	externdef	fix_acos_:near	;takes eax, returns ax
	externdef	fix_sqrt_:near	;takes & returns, returns eax, modifies all but ebp

f_ceil  macro   reg
        add     reg,00000ffffh
        and     reg,0ffff0000h
        endm

f_floor macro   reg
        and     reg,0ffff0000h
        endm

f_cint  macro   reg
        add     reg,00000ffffh
        sar     reg,010h
        endm

f_fint  macro   reg
        sar     reg,010h
        endm

;fixup the result of multiply, putting the result in eax
mfixup	macro
	shr	eax,16
	shl	edx,16
	or	eax,edx
	endm

;fixed-point multiply of eax times arg. trashes edx 
fixmul	macro	arg
	imul	arg
        shrd    eax,edx,16
	endm

;rounding version of fixmul
fixmul_r	macro	arg
	local	no_round
	imul	arg
	add	eax,8000h	;add in get frac bit
	adc	edx,0
	shr	eax,16
	shl	edx,16
	or	eax,edx
	endm

;fixed-point divide of eax by arg. trashes edx. arg cannot be edx
fixdiv	macro	arg
;;	if	("&arg" eq "edx") or ("&arg" eq "EDX")
;;	 echo	Arg to fixdiv cannot be edx
;;	 .err
;;	endif
	mov	edx,eax
	sar	edx,16
	shl	eax,16
	idiv	arg
	endm

;multiplies eax by arg1, divides by arg2
;keeps intermediate result in edx
fixmuldiv macro arg1,arg2
        imul    arg1
        idiv    arg2
        endm


;makes a fixed point number with integral part a and fractional part b.
;works for immediates
fixmake	macro	a,b
	ifb	<b>
	 exitm	<(a) SHL 16>
	else
	 exitm	<((a) SHL 16) + b>
	endif
	endm


;here's some handy constants

f1_0	= fixmake(1)
f0_5	= fixmake(0,8000h)
f0_25	= fixmake(0,4000h)
	
ENDIF

