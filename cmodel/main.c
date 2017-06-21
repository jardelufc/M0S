#include <stdio.h>
#include <stdlib.h>

#define MAX_TASKS 4;
#define MAX-MUTEX 8;
#define TASK-STACK-SIZE 128;
#define unescheduled 0;

enum states{INACTIVE, UNSCHEDULED, RUNNING, SLEEPING, SENT_TO_SLEEP, WAIT_FOR_MUTEX}estados;

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
    uint32_t entry_stack;
    uint32_t pc;
    uint32_t ipc;
    uint32_t entry_state;
    uint32_t unused;
    uint32_t entry_target;
    uint32_t entry_nr_sched;
    uint32_t entry_mutex;
}task;

typedef struct{
    uint32_t current_task;
    uint32_t num_tasks;
    uint32_t sys_timer;
    uint32_t stack_pos;
    uint32_t task_array;
    uint32_t mutex_storage;
}ofs;

typedef struct{
   // uint8_t unescheduled;
}estado;

void escalonador(int r2, int r3, int r4, int r5, int r6){
    ofs of;
    int r0=0;   //main start
    int r1= of.sys_timer;
    r1+=1;
    of.sys_timer=r1;

    if(r1==0)
        //scheduler_exit;
    r8 = r2;
    r9=r3;
    r10=r4;
    r11=r5;

    int r6 = of.current_task;
    if(r6 <0)
        continue_normal_tasks();
    of.stack_pos=r1;
    sleeping_tasks();
}

void continue_normal_tasks(uint32_t r1, uint32_t r2){
    estado *p;
    estado  x;
    p = &x;

    r1= r1+ofs.task_array;
    r1 = r1+r0;

    r2 = r1+task.entry_state;
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
    r5 = r3+task.entry_stack;
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
    r2 = r2+ofs.task_array;

}

void

int main()
{
}
