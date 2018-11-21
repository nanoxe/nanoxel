#define MAX_CALC_CODE_LENGTH 0x100
#define ACTUAL_CALC_CODE_LENGTH 0xff
#define PROT_READ 0x1
#define PROT_WRITE 0x2
#define PROT_EXEC 0x4
#define PROT_NONE 0x0
#define MAP_ANONYMOUS 0x8
typedef unsigned long long u64;
typedef long long i64;

i64 read(u64 fd, char* bytes, u64 length) {
	asm("mov rax, 0");
	asm("syscall");
}

i64 write(u64 fd, char* bytes, u64 length) {
	asm("mov rax, 1");
	asm("syscall");
}

void exit(int status_code){
	asm("mov rax, 60");
	asm("syscall");
	__builtin_unreachable();
}

void *mmap(u64 addr, u64 length, u64 prot, u64 flags, u64 fd, u64 offset) {
	asm("mov r10, rcx");
	asm("mov rax, 9");
	asm("syscall");
}

i64 open(char *path, i64 flags) {
	asm("mov rax, 2");
	asm("syscall");
}

char note[MAX_CALC_CODE_LENGTH];

i64 calc() {
	char *function = (char *)mmap(0, 0x20000, PROT_EXEC|PROT_WRITE, 0x22, -1, 0);
	u64 counter = 0;
	function[0] = 0xc3;
	(*(void (*)())function)();
}

void reverse(u64 last_index) {
	for (u64 index = 0; index < last_index >> 1; ++index) {
		u64 tmp = note[index];
		note[index] = note[last_index - index - 1];
		note[last_index - index - 1] = tmp;
	}
}

u64 i64_to_str(i64 number) {
	u64 index = 0;
	if (number < 0) {
		note[index] = '-';
		++index;
		number = ~number;
	}
	do {
		char digit = number % 10;
		note[index] = '0' + digit;
		++index;
		number /= 10;
	} while(number != 0);
	reverse(index);
	note[index] = 0x0a;
	++index;
	note[index] = 0x00;
	return index;
}

void _start(void) {
	while(1) {
		i64 code_length = read(0, note, ACTUAL_CALC_CODE_LENGTH);
		note[code_length] = 0;
		write(1, note, i64_to_str(calc()));
	}
}
