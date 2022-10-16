#include <asm.h>

ENTRY(write);
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx
	pushl %edi;

	movl 8(%ebp), %ebx; 
	movl 12(%ebp), %ecx; 
	movl 16(%ebp), %edx; 
	movl $0x04, %eax;

	pushl %ecx;
	pushl %edx;
	leal ret, %edi;
	pushl %edi;
	pushl %ebp; 
	movl %esp, %ebp; 
	
	sysenter;

ret:	
	popl %ebp;
	popl %edi;
	popl %edx; 
	popl %ecx; 

	cmpl $0, %eax; 
	jge end;
	neg %eax;
	movl %eax,errno;
	movl $-1,%eax;

end:
	popl %edi;
	popl %ebx; 
	popl %ebp;
	ret;

ENTRY(get_time);
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx;
	pushl %edi;

	movl $0x0a, %eax;

	pushl %ecx;
	pushl %edx;
	leal ret2, %edi;
	pushl %edi;
	pushl %ebp; 
	movl %esp, %ebp; 
	
	sysenter;

ret2:	
	popl %ebp;
	popl %edi;
	popl %edx; 
	popl %ecx; 

	cmpl $0, %eax; 
	jge end2;
	neg %eax;
	movl %eax, errno;
	movl $-1, %eax;

end2:
	popl %edi;
	popl %ebx;
	popl %ebp;
	ret;

ENTRY(prova);
	pushl %ebp
	movl %esp, %ebp
	pushl %ebx;
	pushl %edi;

	movl $0x0b, %eax;

	pushl %ecx;
	pushl %edx;
	leal ret3, %edi;
	pushl %edi;
	pushl %ebp; 
	movl %esp, %ebp; 
	
	sysenter;

ret3:	
	popl %ebp;
	popl %edi;
	popl %edx; 
	popl %ecx; 

	cmpl $0, %eax; 
	jge end3;
	neg %eax;
	movl %eax, errno;
	movl $-1, %eax;

end3:
	popl %edi;
	popl %ebx;
	popl %ebp;
	ret;