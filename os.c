// os.c
// Runs on LM4F120/TM4C123/MSP432
// Lab 2 starter file.
// Daniel Valvano
// February 20, 2016

#include <stdint.h>
#include "os.h"
#include "../inc/CortexM.h"
#include "../inc/BSP.h"
#include "../inc/msp432p401r.h"//for DIO registers in edge trigger fcn


// function definitions in osasm.s
void StartOS(void);

//globals
mailType PostalService;//this is the FIFO
tcbType tcbs[NUMTHREADS];//directory of main thread TCBs
ptcbType ptcbs[NUMPTHREADS];//directory of periodic thread TCBs
tcbType *RunPt;//pointer to currently running TCB (and, underlying, its last saved SP)
uint8_t nextT, nextPT;//counters for defining new threads/periodic threads up to the limit
int32_t Stacks[NUMTHREADS][STACKSIZE];//static allocation for thread stacks
int32_t *sig0, *sig1;//semaphores to be signalled periodically
int32_t *buttonSig;//semaphore to be signalled on button press


// ******** OS_Init ************
// Initialize operating system, disable interrupts
// Initialize OS controlled I/O: systick, bus clock as fast as possible
// Initialize OS global variables
// Inputs:  none
// Outputs: none
void OS_Init(void){
  DisableInterrupts();
  BSP_Clock_InitFastest();// set processor clock to fastest speed
	OS_FIFO_Init();
	nextT = 0;
	nextPT = 0;
}

void SetInitialStack(int i, void (*initial_PC)(void)){  
  tcbs[i].sp = &Stacks[i][STACKSIZE-16]; // thread stack pointer
	Stacks[i][STACKSIZE-1] = 0x01000000; //CPSR with correct thumb bit
	Stacks[i][STACKSIZE-2] = (int32_t) initial_PC; //this is thread top (the passed fcn pointer)
	Stacks[i][STACKSIZE-3] = 0x14141414; //garbage LR for alignment purposes
	Stacks[i][STACKSIZE-4] = 0x12121212;   // R12
  Stacks[i][STACKSIZE-5] = 0x03030303;   // R3
  Stacks[i][STACKSIZE-6] = 0x02020202;   // R2
  Stacks[i][STACKSIZE-7] = 0x01010101;   // R1
  Stacks[i][STACKSIZE-8] = 0x00000000;   // R0
  Stacks[i][STACKSIZE-9] = 0x11111111;   // R11
  Stacks[i][STACKSIZE-10] = 0x10101010;  // R10
  Stacks[i][STACKSIZE-11] = 0x09090909;  // R9
  Stacks[i][STACKSIZE-12] = 0x08080808;  // R8
  Stacks[i][STACKSIZE-13] = 0x07070707;  // R7
  Stacks[i][STACKSIZE-14] = 0x06060606;  // R6
  Stacks[i][STACKSIZE-15] = 0x05050505;  // R5
	Stacks[i][STACKSIZE-16] = 0x04040404;  // R4
}

//******** OS_AddThreads ***************
// Add eight main threads to the scheduler
// Inputs: function pointers to four void/void main threads
// Outputs: 1 if successful, 0 if this thread can not be added
// This function will only be called once, after OS_Init and before OS_Launch
int OS_AddThreads(void(*thread0)(void), uint32_t p0,
                  void(*thread1)(void), uint32_t p1,
                  void(*thread2)(void), uint32_t p2,
                  void(*thread3)(void), uint32_t p3,
                  void(*thread4)(void), uint32_t p4,
                  void(*thread5)(void), uint32_t p5,
                  void(*thread6)(void), uint32_t p6,
                  void(*thread7)(void), uint32_t p7) {
	OS_AddThread(thread0, p0);
	OS_AddThread(thread1, p1);
	OS_AddThread(thread2, p2);
	OS_AddThread(thread3, p3);
	OS_AddThread(thread4, p4);	
	OS_AddThread(thread5, p5);
	OS_AddThread(thread6, p6);
  return OS_AddThread(thread7, p7);//if we fail to add the last one, we have failed
}

//Valvano's AddThreads is a wrapper to call this fcn repeatedly
int OS_AddThread(void(*thread)(void), uint32_t pri) {
	if (nextT < NUMTHREADS) {
		if (nextT == 0) {
			tcbs[nextT].next = &tcbs[nextT];//anchor the first thread
		} else {
			tcbs[nextT].next = tcbs[nextT-1].next;
			tcbs[nextT-1].next = &tcbs[nextT];
		}
		SetInitialStack(nextT, thread);
		tcbs[nextT].block = 0;
		tcbs[nextT].sleep = 0;
		tcbs[nextT].priority = pri;
		nextT++;
		return 1;
	} else {
		return 0;
	}
}

int OS_AddPeriodicEventThread(void(*thread)(void), uint32_t period) {
	if (nextPT < NUMPTHREADS) {
		if (nextPT == 0) {
			ptcbs[nextPT].next = &ptcbs[nextPT];
		} else {
			ptcbs[nextPT].next = ptcbs[nextPT-1].next;
			ptcbs[nextPT-1].next = &ptcbs[nextPT];
		}
		ptcbs[nextPT].task = thread;
		ptcbs[nextPT].period = period-1;//so, a thread with period 1 will run every ms, etc.
		ptcbs[nextPT].sleep = period-1;
		nextPT++;
		return 1;
	}
	else {
		return 0;
	}
}
//******** OS_AddPeriodicEventThreads ***************
// Add two background periodic event threads
// Typically this function receives the highest priority
// Inputs: pointers to a void/void event thread function2
//         periods given in units of OS_Launch (Lab 2 this will be msec)
// Outputs: 1 if successful, 0 if this thread cannot be added
// It is assumed that the event threads will run to completion and return
// It is assumed the time to run these event threads is short compared to 1 msec
// These threads cannot spin, block, loop, sleep, or kill
// These threads can call OS_Signal
int OS_AddPeriodicEventThreads(void(*thread1)(void), uint32_t period1, void(*thread2)(void), uint32_t period2) {
	OS_AddPeriodicEventThread(thread1, period1);
	OS_AddPeriodicEventThread(thread2, period2);
	return 1;
}

//this function will be called periodically as initialized in OS_Launch
void static runperiodicevents(void){
	DisableInterrupts();

	//decrement periodic thread sleep counters and run the thread and reset when sleep hits 0
	for (int n = 0; n < nextPT; n++) {
		if (ptcbs[n].sleep) {
			ptcbs[n].sleep--;
		} else {
			(*ptcbs[n].task)();
			ptcbs[n].sleep = ptcbs[n].period;
		}
	}
	EnableInterrupts();
}

//wrappers to implement semaphore signaling as a periodic task
static void snglr0() {
	OS_Signal(sig0);
}

static void snglr1() {
	OS_Signal(sig1);
}

//initializing those wrappers
void OS_PeriodTrigger0_Init(int32_t *semaPt, uint32_t period) {
	sig0 = semaPt;
	OS_AddPeriodicEventThread(snglr0, period);//call the wrapper function as a periodic task with the given period
}

void OS_PeriodTrigger1_Init(int32_t *semaPt, uint32_t period) {
	sig1 = semaPt;
	OS_AddPeriodicEventThread(snglr1, period);
}
//******** OS_Launch ***************
// Start the scheduler, enable interrupts
// Inputs: number of clock cycles for each time slice
// Outputs: none (does not return)
// Errors: theTimeSlice must be less than 16,777,216
void OS_Launch(uint32_t theTimeSlice){
  RunPt = &tcbs[0];
	BSP_PeriodicTask_Init(runperiodicevents, 1000, 0);//start the periodic event spinner
  STCTRL = 0;                  // disable SysTick during setup
  STCURRENT = 0;               // any write to current clears it
  SYSPRI3 =(SYSPRI3&0x00FFFFFF)|0xE0000000; // priority 7
  STRELOAD = theTimeSlice - 1; // reload value
  STCTRL = 0x00000007;         // enable, core clock and interrupt arm
  StartOS();                   // start on the first task
}


void Scheduler(void){
	for (int n = 0; n < nextT; n++) {
		if (tcbs[n].sleep) {
			tcbs[n].sleep--;
		}
	}
	tcbType *next_pt;//current choice for next thread
	tcbType *pt = RunPt->next;//start one past runpt, and go back to runpt (since it's a do-while)
	uint32_t mpri = 254;//lower number is higher priority, so we're finding the min here
	do {
		if (!(pt->block) && !(pt->sleep) && pt->priority < mpri) {
			mpri = pt->priority;
			next_pt = pt;
		}
		pt = pt->next;
	} while (pt != RunPt);//should go through every thread
	RunPt = next_pt;
	return;
}

// ******** OS_InitSemaphore ************
// Initialize counting semaphore
// Inputs:  pointer to a semaphore
//          initial value of semaphore
// Outputs: none
void OS_InitSemaphore(int32_t *semaPt, int32_t value){
  *semaPt = value;
}

// ******** OS_Wait ************
// Decrement semaphore
// Lab2 spinlock (does not suspend while spinning)
// Lab3 block if less than zero
// Inputs:  pointer to a counting semaphore
// Outputs: none
void OS_Wait(int32_t *semaPt){
	DisableInterrupts();
	if (*semaPt == 0) {
		RunPt->block = semaPt;
		OS_Sleep(0);
	}
	(*semaPt)--;
	EnableInterrupts();
}

// ******** OS_Signal ************
// Increment semaphore
// Lab2 spinlock
// Lab3 wakeup blocked thread if appropriate
// Inputs:  pointer to a counting semaphore
// Outputs: none
void OS_Signal(int32_t *semaPt){
	DisableInterrupts();
	++(*semaPt);
	for (int n = 0; n < nextT; n++) {
		if (tcbs[n].block == semaPt) {
			tcbs[n].block = 0;
		}
	}
	EnableInterrupts();
}

//******** OS_Suspend ***************
// Called by main thread to cooperatively suspend operation
// Inputs: none
// Outputs: none
// Will be run again depending on sleep/block status
void OS_Suspend(void){
  STCURRENT = 0;        // any write to current clears it
  INTCTRL = 0x04000000; // trigger SysTick
// next thread gets a full time slice
}

void OS_Sleep (uint32_t tosl) {
	RunPt->sleep = tosl;
	OS_Suspend();
}
// ******** OS_FIFO_Init ************
// Initialize FIFO.  
// One event thread producer, one main thread consumer
// Inputs:  none
// Outputs: none
void OS_FIFO_Init(void){
	for (int n = 0; n < FSIZE; n++) {
		PostalService.data[n] = n;
	}
	PostalService.size = 0;
	PostalService.nextIn = 0;
	PostalService.nextOut = 0;
	PostalService.poops = 0;
}

// ******** OS_FIFO_Put ************
// Put an entry in the FIFO.  
// Exactly one event thread puts,
// do not block or spin if full
// Inputs:  data to be stored
// Outputs: 0 if successful, -1 if the FIFO is full
//note that our FIFO does not rely on the binary semaphore functions, except for OS_Signal,
//but instead uses its own internal counters
int8_t OS_FIFO_Put(uint32_t data){
	if (PostalService.size < FSIZE) {
		int s = StartCritical();//access to FIFO must be atomic
		PostalService.data[PostalService.nextIn] = data;
		PostalService.nextIn = (PostalService.nextIn == FSIZE-1) ? 0 : PostalService.nextIn+1;//wraparound FIFO
		if (PostalService.size == 0) {
			OS_Signal(&PostalService.size);//note: this increments the semaphore as well as waking threads
		} else {
			PostalService.size++;
		}
		EndCritical(s);
		return 0;
	} else {
		++PostalService.poops;
		return -1;
	}
}

// ******** OS_FIFO_Get ************
// Get an entry from the FIFO.   
// Exactly one main thread get,
// do block if empty
// Inputs:  none
// Outputs: data retrieved
uint32_t OS_FIFO_Get(void){
		int s = StartCritical();//access to FIFO must be atomic
		if (PostalService.size==0) {
			RunPt->block = &PostalService.size;
			EndCritical(s);//don't want to disable interrupts for everyone, ofc
			OS_Sleep(0);
			s = StartCritical();
		}
		int32_t ret = PostalService.data[PostalService.nextOut];
		PostalService.nextOut = (PostalService.nextOut == FSIZE-1) ? 0 : PostalService.nextOut+1;//wraparound FIFO
		PostalService.size--;
		EndCritical(s);
		return ret;
}

//trigger on pin 3.5
void OS_EdgeTrigger_Init(int32_t *semaPt, uint8_t priority) {
	buttonSig = semaPt;//this semaphore will be signalled by the DIO interrupt
	P3DIR &= ~(0x20);//set pin to input
	P3SEL0 &= ~(0x20);//set pin to general IO
	P3SEL1 &= ~(0x20);
	P3IES |= 0x20;//set pin to falling edge interrupt (negative logic)
	P3IFG &= ~0x20;
	P3IE |= 0x20;//enable interrupt on pin
	NVIC_IPR9 |= (priority << 13);//set priority in NVIC
	NVIC_ISER1 |= 0x20;//enable interrupt in NVIC
}

void PORT3_IRQHandler (void) {
	OS_Signal(buttonSig);//signal the chosen semaphore
	P3IFG &= ~0x20;
	NVIC_ICER1 |= 0x20;//disable interrupt until EdgeTrigger_Restart is called
	return;
}

void OS_EdgeTrigger_Restart (void) {
	NVIC_ICPR1 |= 0x20;//clear any pending interrupts on the pin
	NVIC_ISER1 |= 0x20;//re-enable pin interrupt
}