/*
 * sys.c - Syscalls implementation
 */
#include <devices.h>

#include <utils.h>

#include <io.h>

#include <mm.h>

#include <mm_address.h>

#include <sched.h>

#include <interrupt.h>

#define LECTURA 0
#define ESCRIPTURA 1 

extern int zeos_ticks;

int check_fd(int fd, int permissions)
{
  if (fd!=1) return -9; /*EBADF*/
  if (permissions!=ESCRIPTURA) return -13; /*EACCES*/
  return 0;
}

int sys_ni_syscall()
{
	return -38; /*ENOSYS*/
}

int sys_getpid()
{
	return current()->PID;
}

int sys_fork()
{
  int PID=-1;

  // creates the child process
  
  return PID;
}

void sys_exit()
{  
}


int sys_write(int fd,char * buffer, int size){
  if(check_fd(fd,1) == -1) return -1;
  if (buffer == NULL) return -1;
  if(size <= 0) return -1;

  char bufferAux[size];
  copy_from_user(buffer,bufferAux,size);
  int escrit = sys_write_console(bufferAux,size);
  return escrit;
}

int sys_getTime() {
  return zeos_ticks; 
}

int sys_prova() {
  task_switch(idle_task);
  return 1;  
}
