/* Mason Pitcher Michael Schmitt ECE498 Spring 2024
 * Vulnerable Program
 *
 * This will take user input and save "employee" info into the "database"
 */

#include <stdio.h>
#include <string.h>

struct Employee {
	char name[50];
	int id;
	int time_in;
	int time_out;
};

int add_new_employee(struct Employee *database, int employee_count);

int add_new_employee(struct Employee *database, int employee_count) {
	char name_input[100];

	printf("enter employee name: ");
	gets(name_input);
	printf("welcome to work, %s! :3\n", name_input);

	database[employee_count].id = employee_count+1;
	strcpy(database[employee_count].name, name_input);

	return 0;
}

int main(void) {
	int i;
	int count = 0;
	struct Employee database[5];

	while (count < 3) {
		add_new_employee(database, count);
		count += 1;
	}

	printf("employees: \n");
	for (i = 0; i<3; ++i) {
		printf("%d: %s\n", database[i].id, database[i].name);
	}

	return 0;
}
