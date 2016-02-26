;++
;
; Copyright (c) Microsoft Corporation.  All rights reserved.
;
;
; Module:
;
;   kxarm.w
;
; Abstract:
;
;   Contains ARM architecture constants and assembly macros.
;
;--

;
; The ARM assembler uses a baroque syntax that is documented as part
; of the online Windows CE documentation.  The syntax derives from
; ARM's own assembler and was chosen to allow the migration of
; specific assembly code bases, namely ARM's floating point runtime.
; While this compatibility is no longer strictly necessary, the
; syntax lives on....
;
; Highlights:
;      * Assembler is white space sensitive.  Symbols are defined by putting
;        them in the first column
;      * The macro definition mechanism is very primitive
;
; To augment the assembler, assembly files are run through CPP (as they are
; on IA64).  This works well for constants but not structural components due
; to the white space sensitivity.
;
; For now, we use a mix of native assembler and CPP macros.
;


        include kxarmunw.h


        ;
        ; Global variables
        ;

        ; Current function names and labels
        GBLS    __FuncStartLabel
        GBLS    __FuncEpilog1StartLabel
        GBLS    __FuncEpilog2StartLabel
        GBLS    __FuncEpilog3StartLabel
        GBLS    __FuncEpilog4StartLabel
        GBLS    __FuncXDataLabel
        GBLS    __FuncXDataPrologLabel
        GBLS    __FuncXDataEpilog1Label
        GBLS    __FuncXDataEpilog2Label
        GBLS    __FuncXDataEpilog3Label
        GBLS    __FuncXDataEpilog4Label
        GBLS    __FuncXDataEndLabel
        GBLS    __FuncEndLabel

        ; other globals relating to the current function
        GBLS    __FuncArea
        GBLS    __FuncExceptionHandler


        ;
        ; Helper macro: generate the various labels we will use internally
        ; for a function
        ;
        ; Output is placed in the various __Func*Label globals
        ;

        MACRO
        __DeriveFunctionLabels $FuncName

__FuncStartLabel        SETS "|$FuncName|"
__FuncEndLabel          SETS "|$FuncName._end|"
__FuncEpilog1StartLabel SETS "|$FuncName._epilog1_start|"
__FuncEpilog2StartLabel SETS "|$FuncName._epilog2_start|"
__FuncEpilog3StartLabel SETS "|$FuncName._epilog3_start|"
__FuncEpilog4StartLabel SETS "|$FuncName._epilog4_start|"
__FuncXDataLabel        SETS "|$FuncName._xdata|"
__FuncXDataPrologLabel  SETS "|$FuncName._xdata_prolog|"
__FuncXDataEpilog1Label SETS "|$FuncName._xdata_epilog1|"
__FuncXDataEpilog2Label SETS "|$FuncName._xdata_epilog2|"
__FuncXDataEpilog3Label SETS "|$FuncName._xdata_epilog3|"
__FuncXDataEpilog4Label SETS "|$FuncName._xdata_epilog4|"
__FuncXDataEndLabel     SETS "|$FuncName._xdata_end|"

        MEND


        ;
        ; Helper macro: create a global label for the given name,
        ; decorate it, and export it for external consumption.
        ;

        MACRO
        __ExportName $FuncName

        LCLS    Name
Name    SETS    "|$FuncName|"
        ALIGN   4
        EXPORT  $Name
$Name
        MEND

        MACRO
        __ExportProc $FuncName

        LCLS    Name
Name    SETS    "|$FuncName|"
        ALIGN   4
        EXPORT  $Name
$Name   PROC
        MEND


        ;
        ; Declare that all following code/data is to be put in the .text segment
        ;

        MACRO
        TEXTAREA
    if {CONFIG} == 32
        AREA    |.text|,ALIGN=2,CODE,READONLY, ARM
    else
        AREA    |.text|,ALIGN=2,CODE,READONLY, THUMB
    endif
        MEND


        MACRO
        STARTUPTEXT
        TEXTAREA
        MEND

        ;
        ; Declare that all following code/data is to be put in the .data segment
        ;

        MACRO
        DATAAREA
        AREA    |.data|,DATA
        MEND


        ;
        ; Declare that all following code/data is to be put in the .rdata segment
        ;

        MACRO
        RODATAAREA
        AREA    |.rdata|,DATA,READONLY
        MEND


        ;
        ; Macro for indicating the start of a nested function. Nested functions
        ; imply a prolog, epilog, and unwind codes.
        ;

        MACRO
        NESTED_ENTRY $FuncName, $AreaName, $ExceptHandler

        ; compute the function's labels
        __DeriveFunctionLabels $FuncName

        ; determine the area we will put the function into
__FuncArea   SETS    "|.text|"
        IF "$AreaName" != ""
__FuncArea   SETS    "$AreaName"
        ENDIF

        ; set up the exception handler itself
__FuncExceptionHandler SETS ""
        IF "$ExceptHandler" != ""
__FuncExceptionHandler SETS    "|$ExceptHandler|"
        ENDIF

        ; switch to the specified area
    if {CONFIG} == 32
        AREA    $__FuncArea,CODE,READONLY,ARM
    else
        AREA    $__FuncArea,CODE,READONLY,THUMB
    endif

        ; export the function name
        __ExportProc $FuncName

        ; flush any pending literal pool stuff
        ROUT

        ; reset the state of the unwind code tracking
        __ResetUnwindState

        MEND


        ;
        ; Macro for indicating the end of a nested function. We generate the
        ; .pdata and .xdata records here as necessary.
        ;

        MACRO
        NESTED_END $FuncName

        ; mark the end of the function
$__FuncEndLabel
        LTORG
        ENDP

    if {CONFIG} <> 32
        ; generate .pdata
        AREA    |.pdata|,ALIGN=2,READONLY
        DCD     $__FuncStartLabel
        RELOC   2                                       ; make this relative to image base

        DCD     $__FuncXDataLabel
        RELOC   2                                       ; make this relative to image base

        ; generate .xdata
        __EmitUnwindXData

        ; back to the original area
        AREA    $__FuncArea,CODE,READONLY
    endif
        ; reset the labels
__FuncStartLabel SETS    ""
__FuncEndLabel  SETS    ""

        MEND


        ;
        ; Macro for indicating the start of a leaf function.
        ;

        MACRO
        LEAF_ENTRY $FuncName, $AreaName

        ; compute the function's labels
        __DeriveFunctionLabels $FuncName

        ; determine the area we will put the function into
__FuncArea   SETS    "|.text|"
        IF "$AreaName" != ""
__FuncArea   SETS    "$AreaName"
        ENDIF

        ; switch to the specified area
        AREA    $__FuncArea,CODE,READONLY

        ; export the function name
        __ExportProc $FuncName

        ; flush any pending literal pool stuff
        ROUT

        MEND


        ;
        ; Macro for indicating the end of a leaf function.
        ;

        MACRO
        LEAF_END $FuncName

        ; mark the end of the function
$__FuncEndLabel
        LTORG
        ENDP

        ; reset the labels
__FuncStartLabel SETS    ""
__FuncEndLabel  SETS    ""

        MEND

    if {CONFIG} == 32
        ;
        ; ARM assembly: Macro for indicating the end of a function (no pdata/xdata).
        ;

        MACRO
        ENTRY_END $FuncName

        ; mark the end of the function
$__FuncEndLabel
        LTORG
        ENDP

        ; reset the labels
__FuncStartLabel SETS    ""
__FuncEndLabel  SETS    ""

        MEND

        MACRO
        PROLOG_END $FuncName
        MEND

    endif



        ;
        ; Macro for indicating an alternate entry point into a function.
        ;

        MACRO
        ALTERNATE_ENTRY $FuncName

        ; export the entry point's name
        __ExportName $FuncName

        ; flush any pending literal pool stuff
        ROUT

        MEND


        MACRO
        CAPSTART $arg1, $arg2
        MEND

        MACRO
        CAPEND $arg1
        MEND


        ;
        ; Macros to read/write coprocessor registers. These macros are preferred over
        ; raw mrc/mcr because they put the register parameter first and strip the
        ; prefixes which allow them to use the same C preprocessor macros as the C
        ; code.
        ;

        MACRO
        CP_READ $rd, $coproc, $op1, $crn, $crm, $op2
        mrc     p$coproc, $op1, $rd, c$crn, c$crm, $op2 ; just shuffle params and add prefixes
        MEND


        MACRO
        CP_WRITE $rd, $coproc, $op1, $crn, $crm, $op2
        mcr     p$coproc, $op1, $rd, c$crn, c$crm, $op2 ; just shuffle params and add prefixes
        MEND


        ;
        ; Macros to output special undefined opcodes that indicate breakpoints
        ; and debug services.
        ;


; break point definition
THUMB_BREAKPOINT        EQU     0xdefe

; CE specific macros used by OAL
CE_NEW_MAPPING_TABLE    EQU    0x87654321

        MACRO
        EMIT_BREAKPOINT
        DCW     THUMB_BREAKPOINT                        ; undefined per ARM ARM
        MEND


        END
