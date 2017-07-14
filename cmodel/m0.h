#ifndef M0_H_INCLUDED
#define M0_H_INCLUDED


#define TOTAL_MEMORY 16000 //Valor variável
#define KERNEL_MEMORY_START 32
#define MAX_KERNEL_MEMORY 512
#define KERNEL_STACK_SIZE 256
#define KERNEL_STACK_TOP 48
#define IDLE_TASK_STACK_TOP 44
#define IDLE_STACK_SIZE 256
#define FREE_MEMORY_START 1024
#define FREE_MEMORY_END 15488
#define IDLE_TASK 0
#define MAX_TASKS 4
#define MAX_MUTEX 8
#define TASK_STACK_SIZE 128
#define TASK_ENTRY_SHIFT_L 5
#define UNSCHEDULED 0
#define ALL_MEMORY_SHITF_L 11

enum states{INACTIVE, UNSCHEDULED, RUNNING, SLEEPING, SENT_TO_SLEEP, WAIT_FOR_MUTEX} estados;

typedef unsigned int uint32_t;

typedef struct{
    uint32_t ctrl;
    uint32_t load;
    uint32_t val;
    uint32_t calib;
}Systick;

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

#endif // M0_H_INCLUDED
