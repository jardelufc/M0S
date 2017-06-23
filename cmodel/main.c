#include <stdio.h>
#include <stdlib.h>

#define TOTAL_MEMORY 16000; //Valor variável
#define KERNEL_MEMORY_START 32;
#define MAX_KERNEL_MEMORY 512;
#define KERNEL_STACK_SIZE 256;
#define IDLE_STACK_SIZE 256;
#define FREE_MEMORY_START 1024;
#define FREE_MEMORY_END 15488;
#define idle_task 0;
#define MAX_TASKS 4;
#define MAX-MUTEX 8;
#define TASK-STACK-SIZE 128;
#define unescheduled 0;

enum states{INACTIVE, UNSCHEDULED, RUNNING, SLEEPING, SENT_TO_SLEEP, WAIT_FOR_MUTEX} estados;

typedef unsigned int uint32_t;

typedef struct{
    uint32_t R0;
    uint32_t R1;
    uint32_t R2;
    uint32_t R3;
    uint32_t R12;
    uint32_t LR;
    uint32_t PC;
    uint32_t XPSR;
}Frame;

typedef struct{
    uint32_t entry_stack;   // stack
    uint32_t pc;            // program counter
    uint32_t ipc;           // inter process communication
    uint32_t entry_state;   // state
    uint32_t unused;        // unused
    uint32_t entry_target;  // stores the target value for sleeping tasks
    uint32_t entry_nr_sched;// how many times was this thread scheduled
    uint32_t entry_mutex;   // mutex
}task;

typedef struct{
    uint32_t current_task;
    uint32_t num_tasks;
    uint32_t sys_timer;
    uint32_t idle_stack_pos;
    uint32_t idle_nr_sched;  
    uint32_t mutex_storage;
    uint32_t mutex_area;    
    uint32_t task_array;
}ofs;

ofs myofs;
frame myframe;
task mytasks[MAX_TASKS];

void scheduler(uint32_t r2, uint32_t r3, uint32_t r4, uint32_t r5, uint32_t r6){
   
    uint32_t r0=0;   //main start
    uint32_t r1= of.sys_timer;
    r1+=1;
    myofs.sys_timer=r1;

    if(r1==0)
        //scheduler_exit;
    r8 = r2;
    r9=r3;
    r10=r4;
    r11=r5;

    r6 = myofs.current_task;
    if(r6 <0)
        continue_normal_tasks();
    myofs.stack_pos=r1;
    sleeping_tasks();
}

void continue_normal_tasks(uint32_t r1, uint32_t r2){
    estado *p;
    estado  x;
    p = &x;

    r1= r1+myofs.task_array;
    r1 = r1+r0;

    r2 = r1+mytasks[???????].entry_state;
    if(r2 == p->unescheduled)   //Verificar se o apontamento está correto
        sleeping_tasks();
}

void sleeping_tasks(){
    uint32_t r1 = ofs.task_array; //Colocar em endereço ou não ?
    r1 = r1+r0;
    int r2 = MAX_TASKS-1; //Endereço de Max-tasks ?
}

void process_sleeping_tasks(uint32_t r1){
    uint32_t r3 = r3+r1;
    uint32_t r4 = r3+ task.entry_state;

    if(r4 == WAIT_FOR_MUTEX)
        maybe_is_sleeping();
}

void check_to_see_if_mutex_is_free(uint32_t r3){
    uint32_t r5 = r3+task.entry_mutex;
    uint32_t r4 = r0+ofs.
}

void maybe_is_sleeping(uint32_t r4){
    if(r4 == SLEEPING)
        adjust_sleeping_value();
    if(r4!=SENT_TO_SLEEP)
        advance-counter();
}

void adjust_sleeping_value(uint32_t r3, uint32_t r4, uint32_t r5){
    r4 = r3+task.entry_target;
    r4--;
    &(r3+task.entry_target)=r4; // é igual a str r4,[r5,taskentrytarget] ?
    if(r4!=0)
        advance_counter();
    r5 = r3+task.entry_target;

    if(r5==SENT_TO_SLEEP)
        do_not_advance_pc();
}

void set_state_to_running(uint32_t r5, uint32_t r3, uint32_t r4, uint32_t r5){
    r5 = r3+mytasks[??????].entry_stack;
    r4 = Frame.PC + //0x20 linha 171 do .asm
    r4 = r4 + 2;
    Frame.PC=r4
}

void do_not_advance_pc(uint32_t r4, uint32_t r3){
    r4 = RUNNING;
    task.entry_state=r4;
}

void advance_counter(uint32_t r2){
    r2 = r2 - 1;
    if(r2>=0)
        process_sleeping_tasks();
}
//-----------------------------------------------------------------------------------------
//Decidir, agora, qual tarefa deve ser escalonada para o processador

void find_next_task_init(uint32_t r5, uint32_t r3){
    r5 = MAX_TASKS;
    r3=0;
    find_next_task();
}

void  find_next_task(uint32_t r6, uint32_t r2, uint32_t r0, uint32_t r1){
    r6++;
    r2 = MAX_TASKS-1;
    //ands r6,r2 pesquisar mais acerca, aparentemente representa &&, mas o código não apresenta uma condição.

    r2 = r2+r0;
    r2 = r2+myofs.task_array;   //r2 se torna o endereço da próxima tarefa

    r1 = r2 + task.entry_state;
    if(r1 == RUNNING)
        task_found();
    if(r1== UNSCHEDULED)
        it_is_an_unscheduled_task();

    r5--;
    if(r5!=0)
        find_next_task();
}

void schedule_idle_task(uint32_t r1, uint32_t r2, uint32_t r0){
    r1 = r0 + ofs.entry_nr_sched;
    r1++;
    ofs.entry_nr_sched=r1;

    r1 =  r0 + ofs.current_task;
    r2 = 1;
    r2 = r2 + 32; // igual a lsls r2,31 ? linha 219
    r1 = r2;     // igual a orrs r1,r2 ?
    ofs.current_task = r2;

    r0 = r0+ofs.idle_stack_pos;
    r1 = idle_task+1; //linha 224, idle_stack é um bloco, contudo não há retorno de valor da função
    Frame.PC=r1;
    restore_stack();
}

void it_is_an_unscheduled_task(uint32_t r3, uint32_t r1, uint32_t r2){
    r3 = 1;
    r1 = RUNNING;
    r1 = r2 +task.entry_state;
}

void task_found(uint32_t r1, uint32_t r6, uint32_t r2, uint32_t r3, uint32_t r0){
    r1 = r6;
    myofs.current_task = r1;

    r1 = r2 + task.entry_nr_sched;
    r1++;
    task.entry_nr_sched = r1;

    r1 = r2 + task.entry_nr_sched;
    //msr PSP, r1, aparentemente não aplicável em C, uma vez q solicita o uso de um  coprocessador

    if(r3!=0)
        scheduler_exit();
}

void restore_stack(uint32_t r0, uint32_t r4, uint32_t r7, uint32_t r1, uint32_t r2, uint32_t r3, uint32_t r8,uint32_t r9,uint32_t r10, uint32_t r11){
    r1 = r0;
    r2 = r0 + 0x04;
    r3 = r0 + 0x08;
    r8 = r1;
    r9 = r2;
    r10 = r3;
    r1 = r0+0x012;
    r11 = r1;

    r0 =  r0 +0x10;
    //msr PSP,r0;
}

void scheduler_exit(){
    setbuf(stdout, 0);  // pode ser usador no lugar de isb ?
    scheduler(uint32_t r2, uint32_t r3, uint32_t r4, uint32_t r5, uint32_t r6);
}

//----------------------------------------------------------------------------------
// Para o próximo bloco, tem-se que, o  Idle task será escalonado no caso de nenhuma outra
//tarefa estar disponível para escalonamento.
// Tentará concatenar dois blocos de memória livre diferentes e então entrar em  loop.
// Também pode ser usado para colocar o sistema em repouso visando economizar energia.
//----------------------------------------------------------------------------------

void idle_task(){
    r5 =  FREE_MEMORY_START; //FREE_MEMORY_START = KERNEL_MEMORY_START + MAX_KERNEL_MEMORY +KERNEL_STACK_SIZE+IDLE_TASK_STACK_SIZE
    r4 = FREE_MEMORY_END;//FREE_MEMORY_END = KERNEL_MEMORY_START + TOTAL MEMORY-(MAX_TASKS*TASk_STACK_SIZE)
    r0 = 0;
}

int main()
{
    
    
}
