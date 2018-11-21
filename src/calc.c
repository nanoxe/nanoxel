void exit(const int status_code){
	asm("mov rax, 60");
	asm("syscall");
}

void _start(void) {
	exit(0);
}
