/*
 * sched.c - initializes struct for task 0 anda task 1
 */

#include <sched.h>
#include <mm.h>
#include <io.h>

union task_union task[NR_TASKS]
  __attribute__((__section__(".data.task")));

#if 0
struct task_struct *list_head_to_task_struct(struct list_head *l)
{
  return list_entry( l, struct task_struct, list);
}
#endif

extern struct list_head blocked;

void writeMSR(int msr_reg, int value); 


/* get_DIR - Returns the Page Directory address for task 't' */
page_table_entry * get_DIR (struct task_struct *t)
{
	return t->dir_pages_baseAddr;
}


struct task_struct *list_head_to_task_struct(struct list_head *l){
	return (struct task_struct*)((int)l&0xfffff000);
}


/* get_PT - Returns the Page Table address for task 't' */
page_table_entry * get_PT (struct task_struct *t) 
{
	return (page_table_entry *)(((unsigned int)(t->dir_pages_baseAddr->bits.pbase_addr))<<12);
}


int allocate_DIR(struct task_struct *t) 
{
	int pos;

	pos = ((int)t-(int)task)/sizeof(union task_union);

	t->dir_pages_baseAddr = (page_table_entry*) &dir_pages[pos]; 

	return 1;
}

void cpu_idle(void)
{
	__asm__ __volatile__("sti": : :"memory");

	while(1)
	{
	;
	}
}

static void init_freeQueue() {
	INIT_LIST_HEAD(freeQueue); 
	for(int i = 0; i < NR_TASKS; ++i) {
		list_add_tail(&task[i].task.list, freeQueue); 
	} 
}

static void init_readyQueue() {
	INIT_LIST_HEAD(readyQueue);
}

void init_idle (void)
{
	struct task_struct *actual_PCB = list_head_to_task_struct(list_first(freeQueue));
	list_del(list_first(freeQueue));
	actual_PCB->PID = 0;

	allocate_DIR(actual_PCB);

	union task_union *idle_pcb = (union task_union*) actual_PCB; 

	idle_pcb->stack[KERNEL_STACK_SIZE - 1] = (unsigned long)&cpu_idle;
	idle_pcb->stack[KERNEL_STACK_SIZE - 2] = 0;
	actual_PCB->kernelEsp = (int)&(idle_pcb->stack[KERNEL_STACK_SIZE - 2]);
	idle_task = actual_PCB; 

}

void init_task1(void) //task_switch
{
	struct task_struct *actual_PCB = list_head_to_task_struct(list_first(freeQueue));
	list_del(list_first(freeQueue));
	actual_PCB->PID = 1;
	union task_union *init_pcb = (union task_union*) actual_PCB;

	allocate_DIR(actual_PCB);

	set_user_pages(actual_PCB);


	tss.esp0 = &init_pcb->stack[KERNEL_STACK_SIZE];
	writeMSR(0x175,&init_pcb->stack);
	set_cr3(init_pcb->task.dir_pages_baseAddr);

}


void init_sched()
{
	init_freeQueue(); 
	init_readyQueue();
}

struct task_struct* current()
{
  int ret_value;
  
  __asm__ __volatile__(
  	"movl %%esp, %0"
	: "=g" (ret_value)
  );
  return (struct task_struct*)(ret_value&0xfffff000);
}

void inner_task_switch(union task_union *new){

	tss.esp0 = &(new->stack[KERNEL_STACK_SIZE]);
	writeMSR(0x175,&(new->stack[KERNEL_STACK_SIZE]));
	set_cr3(get_DIR(&new->task));

	switch_stack(&current()->kernelEsp, new->task.kernelEsp);
	/*current()->kernelEsp = get_ebp();

	set_esp(new->task.kernelEsp);*/
}


