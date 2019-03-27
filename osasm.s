;/*****************************************************************************/
; OSasm.s: low-level OS commands, written in assembly                       */
; Runs on LM4F120/TM4C123/MSP432
; Lab 2 starter file
; February 10, 2016
;


        AREA |.text|, CODE, READONLY, ALIGN=2
        THUMB
        REQUIRE8
        PRESERVE8

        EXTERN  RunPt            ; currently running thread
        EXPORT  StartOS
        EXPORT  SysTick_Handler
        IMPORT  Scheduler


SysTick_Handler ;automatically saves AAPCS portions of stack
    CPSID   I ;disable interrupt during switch
	PUSH {R4-R11} ;save the rest of the stack
	LDR R0, =RunPt
	LDR R1, [R0]
	STR SP, [R1]; update the stack pointer in TCB
	PUSH {LR}
	BL Scheduler ;updates RunPt, runs periodic tasks if necessary
	POP {LR}
	LDR R0, =RunPt
	LDR R1, [R0]
	LDR SP, [R1] ;get the new stack pointer
	POP {R4-R11} ;get non-AAPCS regs back
    CPSIE   I ;tasks run with interrupts
	BX LR ;return to thread (pops R0-3, R12, LR, PC, PSR)

StartOS
	LDR R0, =RunPt ;RunPt will be the SP of thread0
	LDR R1, [R0]
	LDR SP, [R1]
	POP {R4-R11}
	POP {R0-R3, R12}
	ADD SP, SP, #4 ;discard garbage LR
	POP {LR} ;this is thread top (stored in PC in init)
	ADD SP, SP, #4; discard null PSR
    CPSIE   I ;tasks run with interrupts
    BX LR ;to top of first thread

	
    ALIGN
    END
