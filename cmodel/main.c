#include <stdio.h>
#include <stdlib.h>
#include "m0.h"


ofs myofs;
Frame myframe;
task mytasks[MAX_TASKS];
Systick systick;
Node node;

//enum States{INACTIVE, UNSCHEDULED, RUNNING, SLEEPING, SENT_TO_SLEEP, WAIT_FOR_MUTEX};


void scheduler(uint32_t r2, uint32_t r3, uint32_t r4, uint32_t r5, uint32_t r6, uint32_t r8, uint32_t r9, uint32_t r10, uint32_t r11){

    uint32_t r0=0;   //main start
    uint32_t r1= myofs.sys_timer;
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
    myofs.idle_stack_pos=r1;
    sleeping_tasks();
}

void continue_normal_tasks(uint32_t r0,uint32_t r6){
    uint32_t r1 = r6*32;
    r1= r1+ myofs.task_array;
    r1 = r1+r0;

    uint32_t r2 = mytasks[MAX_TASKS].entry_state;
    if(r2 <= UNSCHEDULED)
        sleeping_tasks(r0);

    //mrs r2,PSP como fazer mrs em C ?
    //str r2,[r1,TASK_ENTRY_STACK]
}

void sleeping_tasks(uint32_t r0){
    uint32_t r1 = myofs.task_array;
    r1 = r1+r0;
    int r2 = MAX_TASKS-1;
}

void process_sleeping_tasks(uint32_t r1,uint32_t r2){
    uint32_t r3 = r2*32;
    r3=r3+r1;
    uint32_t r4 = mytasks[MAX_TASKS].entry_state;

    if(r4 == WAIT_FOR_MUTEX)
        maybe_is_sleeping();
}

void check_to_see_if_mutex_is_free(uint32_t r3, uint32_t r0){
    uint32_t r5 = r3+mytasks[MAX_TASKS].entry_mutex;
    uint32_t r4 = r0+myofs.mutex_storage;
}

void maybe_is_sleeping(uint32_t r4){
    if(r4 == SLEEPING)
        adjust_sleeping_value();
    if(r4!=SENT_TO_SLEEP)
        advance_counter();
}

void adjust_sleeping_value(uint32_t r3, uint32_t r4, uint32_t r5){
    r4 = r3+mytasks[MAX_TASKS].entry_target;
    r4--;
    mytasks[MAX_TASKS].entry_target=r4;
    if(r4!=0)
        advance_counter();
    r5 = r3+mytasks[MAX_TASKS].entry_target;

    if(r5==SENT_TO_SLEEP)
        do_not_advance_pc();
}

void set_state_to_running(uint32_t r5, uint32_t r3, uint32_t r4){
    r5 = mytasks[MAX_TASKS].entry_stack;
    r4 = myframe.PC;
    r4 = r4 + 2;
    myframe.PC=r4;
}

void do_not_advance_pc(uint32_t r4, uint32_t r3){
    r4 = RUNNING;
    mytasks[MAX_TASKS].entry_state = r4; //task.entry_state=r4;
}

void advance_counter(uint32_t r2,uint32_t r1){
    r2 = r2 - 1;
    if(r2>=0)
        process_sleeping_tasks(r1,r2);
}
//-----------------------------------------------------------------------------------------
//Decidir, agora, qual tarefa deve ser escalonada para o processador

void find_next_task_init(uint32_t r5, uint32_t r3){
    r5 = MAX_TASKS;
    r3=0;
    find_next_task();
}

void  find_next_task(uint32_t r6, uint32_t r2, uint32_t r0, uint32_t r1,uint32_t r5){
    r6++;
    r2 = MAX_TASKS-1;
    r6&=r2;

    r2 = r2+r0;
    r2 = r2+myofs.task_array;   //r2 se torna o endereço da próxima tarefa

    r1 = r2 + mytasks[MAX_TASKS].entry_state;//task.entry_state;
    if(r1 == RUNNING)
        task_found();
    if(r1== UNSCHEDULED)
        it_is_an_unscheduled_task();

    r5--;
    if(r5!=0)
        find_next_task(r6,r2,r0,r1,r5);
}

void schedule_idle_task(uint32_t r1, uint32_t r2, uint32_t r0){
    r1 = myofs.idle_nr_sched;
    r1++;
    myofs.idle_nr_sched=r1;

    r1 =  r0 + myofs.current_task;
    r2 = 1;
    r2 = r2*32;
    r1 |= r2;     // igual a orrs r1,r2 ?
    myofs.current_task = r2;

    r0 = r0+myofs.idle_stack_pos;
    r1 = idle_task()+1; //linha 224, idle_stack é um bloco, contudo não há retorno de valor da função
    myframe.PC=r1;
    restore_stack();
}

void it_is_an_unscheduled_task(uint32_t r3, uint32_t r1, uint32_t r2){
    r3 = 1;
    r1 = RUNNING;
    r1 = r2 +mytasks[MAX_TASKS].entry_state;//task.entry_state;
}

void task_found(uint32_t r1, uint32_t r6, uint32_t r2, uint32_t r3, uint32_t r0){
    r1 = r6;
    myofs.current_task = r1;

    r1 = r2 + mytasks[MAX_TASKS].entry_nr_sched;//task.entry_nr_sched;
    r1++;
    mytasks[MAX_TASKS].entry_nr_sched=r1;//task.entry_nr_sched = r1;

    r1 = r2 + mytasks[MAX_TASKS].entry_nr_sched;//task.entry_nr_sched;
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

void scheduler_exit(uint32_t r2,uint32_t r3,uint32_t r4,uint32_t r5,uint32_t r6,uint32_t r8,uint32_t r9,uint32_t r10,uint32_t r11){
    setbuf(stdout, 0);  // pode ser usador no lugar de isb ?
    scheduler(r2, r3, r4, r5, r6,r8,r9,r10,r11);
}

//----------------------------------------------------------------------------------
// Para o próximo bloco, tem-se que, o  Idle task será escalonado no caso de nenhuma outra
//tarefa estar disponível para escalonamento.
// Tentará concatenar dois blocos de memória livre diferentes e então entrar em  loop.
// Também pode ser usado para colocar o sistema em repouso visando economizar energia.
//----------------------------------------------------------------------------------

void idle_task(){
    uint32_t r5 =  FREE_MEMORY_START; //FREE_MEMORY_START = KERNEL_MEMORY_START + MAX_KERNEL_MEMORY +KERNEL_STACK_SIZE+IDLE_TASK_STACK_SIZE
    uint32_t r4 = FREE_MEMORY_END;//FREE_MEMORY_END = KERNEL_MEMORY_START + TOTAL MEMORY-(MAX_TASKS*TASk_STACK_SIZE)
    uint32_t r0 = 0;
    mutex_try_lock();
    r0|=r0;
    if(r0!=0)
        enable_ints();
}

void idle_loop(uint32_t r5, uint32_t r1, uint32_t r4){
    void __disable_irq(void);
    //disable_interrupts();
    r1 =  r5;
    if(r1<0)
        check_next_block();
    if(r1==0)
        idle_release_locks();

    uint32_t r2 = r1+4;
    r2 = r2 + r5;
    if(r2>r4)
        idle_release_locks;

    uint32_t r3 = r2;
    if(r3<0)
        check_next_block();
    if(r3>0)
        combine_zones(r1,r3,r5);

    r4 = r4 - r2; //subs r3,r4,r2 pode ser implementado
    r3 = r3- r4; // dessa forma ?

}

void combine_zones(uint32_t r1, uint32_t r3, uint32_t r5){
    r1 = r1 + r3;
    r1 = r1+4;
    r5 = r1;
}

void check_next_block(uint32_t r1, uint32_t r5, uint32_t r4){
    r1 = r1+4;
    r5 = r5+r1;

    if(r5<r4)
        idle_loop(r5, r1,r4);
}

void idle_release_locks(uint32_t r0){
    r0 = 0;
    mutex_unlock();
}

void enable_ints(){
    void __enable_irq(void);
    //enable_interrupts();
}

// Ponto de entrada principal
// Inicia memória, pilhas e o relógio do sistema e chama init()

void start(uint32_t r1, uint32_t r2){
    void __disable_irq(void);
    //disable_interrupts();
    uint32_t r0 = KERNEL_STACK_TOP;
    // msr MSP, r0,msr, PSP, r0
    // mrs r0, CONTROL não encontrei CONTROL

    r1 = 2;
    r0|=r1;

    //msr CONTROL,r0;
    //isb

    r1=0;
    r2=1;
    r2=r2*(2^9);
    memset(r2,r1,r0);

    r0= systick.ctrl;
    r1 = systick.load;
    r1=1000;

    systick.load=r1;
    r1=1000;
    systick.load=r1;
    r1=systick.val;
    r1 = 0;
    systick.val=r1;

    r1=7;
    r0=r1;

    init_idle_task();

    init_task_area();

    void __enable_irq(void);
    //enable_interrupts();

}

void infinite_loop(uint32_t r0){
    infinite_loop(r0);
}
//Envia uma palavra  (1...2^32-1) para outra tarefa por meio de IPC
//r0 = id da tarefa
//r1 = a palavra a ser enviada

void ipc_send(uint32_t r1){
    uint32_t r2 = 0x00000800;
    uint32_t r0 = r0*2;
    r0 =  r0+myofs.task_array;
    r0= r0+r2;

    mytasks[MAX_TASKS].ipc=r1;
    return;
}

//Função para ler o valor atual de IPC
void ipc_read(){
    //PUSH LR, POSSIVELMENTE SERÁ NECESSÁRIO IMPLEMENTAR UMA PILA, ATÉ MESMO PARA
    //IMPLEMENTAR AS FUNÇÕES QUE LIDAM COM A PILHA DO SISTEMA E DO USUÁRIO
    //push{Lr}
    uint32_t r1 = 0x00000800;
    uint32_t r0=get_current_task_id();
    r0 = r0*2;
    r0 = r0+myofs.task_array;
    r0 = r0 + r1;
    mytasks[MAX_TASKS].ipc=r0;
    //pop {pc}
}

// ORGANIZA A PILHA PARA O PRIMEIRO ESCALONADOR DO IDLE TASK
// SIMULANDO COMO SE JA TIVESSE SIDO ESCALONADO ANTES

void init_idle_task(uint32_t r2, uint32_t r0, uint32_t r1){
    r0 = IDLE_TASK_STACK_TOP;
    r1 = 25; //193?

    //rev r1,r1 transforma r1 em um registro de 4 bytes, mas o r1 já é desse tamanho
    //uint16_t r1 =r1; possível solução para rev ?

    r0 = r0 - 8;
    r0  = r0 - r1 + 4;
    //r1 = idle_task()+1;
    r2 = r2-4;
    r0 = r1; //stored
    r0 = r0-56;
    r1 = 0x00000800; //32bytes
    myofs.idle_stack_pos = r0;
    return myofs.idle_stack_pos;
}

//Inicializa o vetor de tarefas com os valores padrão e os ponteiros de pilha corretos
//Padrão PC é setado para init
//Nenhuma tarefa será escalonada por essa rotina

void init_task_area(uint32_t r0){
    //push{r0-r4,lr} Usar quando implementar a lista
    r0 =  r0 + myofs.task_array;
    uint32_t r1 = MAX_TASKS-1;
}

void init_loop(uint32_t r0, uint32_t r1, uint32_t r3){
    uint32_t r2 = r1*32;; //significa r2,r1, TASK_ENTRY_SHIFT_L?
    r2 = r2 + r0;

    r3 = INACTIVE;
    mytasks[MAX_TASKS].entry_state = r3;
    infinite_loop(r3); //INFINITE_LOOP NÃO É UMA FUNÇÃO QUE RETORNA VALOR, COMO PODE SER ADICIONADA DE 1 ?
    mytasks[MAX_TASKS].pc = r3;
    r3 = r1*32;
    uint32_t r4 = 0x00001000; //16 bytes?
    r4 = r4-r3;
    r4 = r4 - 0x20; //0x100 == 4 bytes ?
    mytasks[MAX_TASKS].entry_state = r4;

    r1 = r1-1;
    if(r1 >= 0)
        init_loop(r0, r1,r3);
    //pop{r0-r4,pc} usar quando implementar a pilha
}

//Cria uma tarefa
//entrada: r0 = endereço de tarefa
// retorna o id da tarefa ou -1 se der erro

void create_task(uint32_t r2){
    //push{r4,lr}  implementar quando a pilha for criada
    void __disable_irq(void);
    //disable_interrupts();
    uint32_t r1;//800 —> 100000  —>32 ?
    r2 = MAX_TASKS-1;
}

void create_loop(uint32_t r0,uint32_t r1, uint32_t r2){
    uint32_t r3 = r2*32;
    r3 = r3 + r1;
    r3 = r3 + myofs.task_array;

    uint32_t r4 = r3*32;
    if(r4 != INACTIVE)
        create_next();

    r0 = r0 + 1;
    mytasks[MAX_TASKS].pc = r0;
    r1 = mytasks[MAX_TASKS].entry_stack;
    myframe.PC = r0;

    r0 = UNSCHEDULED;
    mytasks[MAX_TASKS].entry_state = r0;

    r1 = 0x00000800; //32 bytes ?
    r4 = r1 + myofs.num_tasks;
    r4 = r4+1;
    myofs.num_tasks = r4;

    r0 = r2;
    create_exit();
}

void create_next(uint32_t r0, uint32_t r1,uint32_t r2){
    r2 = r2 - 1;
    if(r2 >= 0)
        create_loop(r0,r1,r2);

    r0 = 0;
    r0 = ~r0;
}

void create_exit(){
    void __enable_irq(void);
    //enable_interrupts();
    //pop{r4,pc}
}

//Coloca uma tarefa que vai ser identificada por seu numero
//de identificação para dormir por um numero especifico de tiques
//Se a tarefa já estiver dormindo, sobrescreve o contador
//ms com o novo valor
//Entrada: r0 = identificação da tarefa
//         r1 = quantos milisegundos para esperar

void sleep_task(uint32_t r0,uint32_t r1){
    void __disable_irq(void);
    //disable_interrupts();

    uint32_t r2 = 0;
    r0 = r0*32;
    r2 = r2 + myofs.task_array;
    r2 = r2 + r0;

    mytasks[MAX_TASKS].entry_target = r1;
    r1 = mytasks[MAX_TASKS].entry_state;
    if(r1 == SLEEPING)
        sleeping();

    r1 = SENT_TO_SLEEP;
    mytasks[MAX_TASKS].entry_state = r1;
}

void sleeping(){
    void __enable_irq(void);
    //enable_interrupts();
    return;
}

//Colocar a atual tarefa para dormir por um numero específico de tiques
// entrada: r0 = esperar por quantos milisegundos

void sleep(uint32_t r1, uint32_t r0){
    uint32_t r2 = 0x00000800;// 32bytes
    r1 = r0;
    r0 = myofs.current_task;

    r0 = r0*32;
    r2 = r2 + myofs.task_array;
    r2 = r2 + r0;

    mytasks[MAX_TASKS].entry_target = r1;
    r1 = SLEEPING;
    mytasks[MAX_TASKS].entry_state = r1;

    //b.

    return(r1); //  PROVISÓRIO
}

// Achar o identificador da tarefa usando o endereço de sua subrotina
// Input:  r0 = iniciar a subrotina
// Return: identificador da tarefa ou -1 caso dê errado
// AVISO; retorna o primeiro id que ele encontra que bate com o endereço
// se criar duas tarefas com o mesmo endereço, boa sorte

void get_task_id(uint32_t r0, uint32_t r1){
    r0 =  r0+ 1;
    r1 = MAX_TASKS-1;
}

void get_task_loop(uint32_t r1, uint32_t r0){
    uint32_t r3=0;
    uint32_t r2 = r1*32;
    r2 = r2 + myofs.task_array;
    r2 = r2+ r3;

    r3 = r3 + mytasks[MAX_TASKS].pc;
    if(r0 == r3)
        found_it();

    r1 = r1-1;
    if(r1>=0)
        get_task_loop(r1,r0);
}

void found_it(uint32_t r0, uint32_t r1){
        r0 = r1;
        return r0;
}

//Retorna o numero de tarefas executando

void get_number_of_tasks(){
    uint32_t r0;
    r0 = r0 +myofs.num_tasks;
    return r0;
}

//Retorna o identificador da tarefa atual

void get_current_task_id(uint32_t r0){
    r0 = r0 +  myofs.current_task;
    return r0;
}

//Retorna o timer do sistema (contador jiffie ou contador de tiques)

void get_system_timer(){
    uint32_t r0;
    r0 =  r0 + myofs.sys_timer;
    return r0;
}

//Define o valor do tempo de sistema a um valor especifico
//Input:  r0=value
//Output:  valor de tempo antigo

void set_system_timer(uint32_t r0,uint32_t r2){
    uint32_t r1;
    r2 = r0*32;
    myofs.sys_timer = r0;
    r0 = r2;
    return r0;
}

//Remove a tarefa de um vetor de escalonamento
//input: r0 =  id de thread

void kill(){
    uint32_t r0;
    uint32_t r1;
    uint32_t r3;
    uint32_t r2 = r0*32;
    r2 = r2 + myofs.task_array;
    r2 = r2 + r1;

    void __disable_irq(void);
    //disable_interrupts();
    r3 = INACTIVE;
    mytasks[MAX_TASKS].entry_state = r3;

    r3 = r0*32;
    uint32_t r4 = 0x00001000;

    r4 = r4 - r3;
    r4 = r4 - 0x20;

    mytasks[MAX_TASKS].entry_stack = r4;

    //r3 = infinite_loop()+1; //infinite não é uma função que retorna nada, como pode ser somada ?
    infinite_loop(r3);
    mytasks[MAX_TASKS].pc = r3;

    r3 = myofs.num_tasks;
    r3 = r3-1;
    if(r3>=0)
        keep_going();
}

void keep_going(uint32_t r3){
    myofs.num_tasks = r3;
    void __enable_irq(void);
    //enable_interrupts();
    return r3;
}

//Vai ser chamado por uma thread de forma a parar de executar

void exit_m0(uint32_t r0){
    get_current_task_id(r0); //call
    kill(); //call
}

//Preenche o buffer com os valores passados
//Input: r0 = endereço do buffer
//       r1 = valor a ser usado para o preenchimento
//       r2 = tamanho do buffer em tamanho word

void memset(uint32_t r2, uint32_t r1, uint32_t r0){
    //stmia r0!,{r1}
    r0=r1;
    r2 = r2 - 1;
    if(r2!=0)
        memset(r2,r1,r0);
}

// Aloca memória
//input r0: tamanho em bytes (máximo 65532 ou FFFC)
//Retorna o endereço da memoria alocada mais recentemente ou 0 caso falhe
//
void malloc_arm(uint32_t r4, uint32_t r5, uint32_t r0, uint32_t r1, uint32_t r3){
    //push {r4-r5,lr} implementar quando implementar a pilha
    r4 =  r0;
    r0 = 0;
    mutex_lock();

    r0 = r4;
    r1 = 3;
    r0 = r0+3;

    //bics r0,r1;
    r0=r1&r1;

    r1 = FREE_MEMORY_START;
    r3 = FREE_MEMORY_END;
}

void check_block(uint32_t r2, uint32_t r1){
    r2 = r2 + r1;
    if(r2<0)
        next_block();
    if(r2>0)
        found_a_block();
}

void it_was_never_allocated(uint32_t r2){
    // se for 0, nunca foi alocado, bloco deve ser: tamanho máximo - control_word
    r2 = FREE_MEMORY_END - FREE_MEMORY_START -4;
}

void found_a_block(uint32_t r0, uint32_t r1,uint32_t r2){
    if(r0>r2)
        next_block();

    uint32_t r5 = r2 - r0;
    uint32_t r4 = 4;
    if(r5<=r4)
        next_block();

    r5 = r5-r4;
    r4 = 0x80;
    //rev r4,r4
    r4=r4/2; ;
    r0=r1;
    r1 = r1+4;

    //uxth r0,r0;
    r0 = r0+r1;
    r5 = r0;

    exit_malloc();
}

void next_block(uint32_t r1, uint32_t r2, uint32_t r3){
    //uxth r2.r2;
    r1 = r1 + 4;
    r1 = r1+r2;
    if(r1<r3)
        check_block(r3,r1);
}

void exit_with_null_pointer(uint32_t r1){
    r1 = 0;
}

void exit_malloc(uint32_t r4, uint32_t r1, uint32_t r0){
    r4 = r1;
    r0=0;
    mutex_unlock();

    r0=r4;
    //pop(r4-r5,pc)
}

void mutex_unlock(uint32_t r0,uint32_t r2,uint32_t r3){
    uint32_t r1;
    r3=1;
    r3 = r0*2;
    r3 = ~r3;

    void __disable_irq(void);
    //disable_interrupts();

    r2 = myofs.mutex_storage;
    r2&=r3;
    myofs.mutex_storage=r2;

    void __enable_irq(void);
    //enable_interrupts();
}

//Tenta travar um mutex. Se falhar, retorna sem bloqueio
//Input: r0=mutex id (0-7)
//Return: r0=0 se mutex estava bloqueado e 1 se não puder ser bloqueado
//AVISO! Não há validação no parametro de input.(arm)

void mutex_try_lock(uint32_t r0, uint32_t r2){
    uint32_t r1;
    uint32_t r3 = 1;
    r3 = r0*2;

    void __disable_irq(void);
    //disable_interrupts();

    r1= myofs.mutex_storage;
    r0=r2;
    r0&=r3;
    if(r0==0)
        mutex_is_free();

    void __enable_irq(void);
    //enable_interrupts();

    r0 = 1;
}

//Tenta bloquear um mutex. Se falhar, espera o mutex ser liberado
//Essa função bloqueia e apenas retorna se o mutex for bloqueado
//Input: r0 = mutex id (0-6) porque usamos mutex 7 para alocações de memória
//Não precisa de retorno

void mutex_lock(uint32_t r0,uint32_t r3){
    uint32_t r1;
    r3 = 1;
    r3 = r0*2;

    void __disable_irq(void);
    //disable_interrupts();

    uint32_t r2 = myofs.mutex_storage;
    r1 &=r3;
    r1|=r3;
    if(r2 ==0)
        mutex_is_free();

    void __enable_irq(void);
    //enable_interrupts();

    r2 = r1;
    r2 = r2*32;
    r2 = r2+myofs.task_array;
    r2 = r2+r1;

    mytasks[MAX_TASKS].entry_mutex=r0;
    r0=WAIT_FOR_MUTEX;
    mytasks[MAX_TASKS].entry_state=r0;
    //b.
}

void mutex_is_free(uint32_t r0, uint32_t r3){

    uint32_t r2 = myofs.mutex_storage;
    r2|=r3;
    myofs.mutex_storage=r2;

    void __enable_irq(void);
    //enable_interrupts();
    r0 =0;
}

//Libera memória alocada
//Input: r0= endereço da memória alocada
//Retorna 0 caso erro
//Não há necessidade de mutexes aqui, já que a operação é atômica

void free_m0(uint32_t r0){
    r0=r0-4;
    uint32_t r1=FREE_MEMORY_START;
    if(r0<r1)
        error_freeing();
    r1 = r0;
    r1 = r1*2;
    //uxth --> transforma 16 bits em 32 bits, aparentemente, desnecessário
    r0=r1;
}

void error_freeing(uint32_t r0){

    r0=0;

}

//FUNÇÕES PARA A IMPLEMENTEÇÃO DA PILHA
void inicia(Node *PILHA)
{
    PILHA->prox = NULL;
    PILHA->tam=0;
}

/*
Node *aloca(Node *PILHA){
    Node *novo=(PILHA->tam)*malloc(sizeof(Node));
    if(!novo){
        printf("Sem memoria disponivel!\n");
        exit(1);
    }
    else{
        printf("Novo elemento: ");
        scanf("%d", &novo->num);
    return novo;
 }
}
*/

void push(Node *PILHA, uint32_t r0){
Node *novo//=aloca();
novo->prox = NULL;

if(PILHA->prox == NULL)
  PILHA->prox=r0;
else{
  Node *tmp = PILHA->prox;

  while(tmp->prox != NULL)
      tmp = tmp->prox;

  tmp->prox = r0;
}
 PILHA->tam++;
}


Node *pop(Node *PILHA)
{
 if(PILHA->prox == NULL){
  printf("PILHA ja vazia\n\n");
  return NULL;
 }else{
  Node *ultimo = PILHA->prox,
  Node *penultimo = PILHA;

  while(ultimo->prox != NULL){
   penultimo = ultimo;
   ultimo = ultimo->prox;
  }

  penultimo->prox = NULL;
  PILHA -> tam--;
  return ultimo;
 }
}

//int main()
//{

//}
