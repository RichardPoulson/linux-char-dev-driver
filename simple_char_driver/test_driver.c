#include <stdio.h>
#include <stdlib.h> // malloc, free, exit
//#include <unistd.h>
#include <mtd/mtd-user.h>
#include <errno.h>
#include <string.h>

void EmptyStdIn(void); // getchar() until EOF
char PromptInput(void); // ask user what they want to do
int ValidChoice(char choice); // did they choose a valid option?
void ReadFromDevice(char * buffer, FILE * device_file);
void WriteToDevice(char * buffer, FILE * device_file);
void SeekIntoDevice(char * buffer, FILE * device_file);
void CallRelatedFunction(char choice, char * buffer, FILE * device_file);

int main() {
	char * buffer = (char *) malloc(32);
	FILE * device_file = fopen("/dev/simple_character_device","r+");

	char choice;
	while (1) {
		choice = PromptInput();
		while (ValidChoice(choice) == 0) { choice = PromptInput(); }
		CallRelatedFunction(choice, buffer, device_file);
	}

    return (0);
}

char PromptInput(void) {
	char input;
	printf("\n");
	printf("Press r to read from device\n");
	printf("Press w to write to the device\n");
	printf("Press s to seek into the device\n");
	printf("Press e to exit from the device\n");
	printf("   Enter command: ");
	scanf("%c", &input);
	printf("\n");
	EmptyStdIn();
	return input;
}

int ValidChoice(char choice) {
	if (choice == 'r') return 1;
	if (choice == 'w') return 1;
	if (choice == 's') return 1;
	if (choice == 'e') return 1;
	return 0;
}

void ReadFromDevice(char * buffer, FILE * device_file) {
	int num;
	printf("How many bytes would you like to read from the file? ");
	scanf("%d", &num);
	EmptyStdIn(); // get rid of \n in stream
	buffer = (char *) realloc(buffer, num);
	printf("\n");
	fread(buffer, 1, num, device_file);
	printf("Data read from the device: %s\n", buffer);
}

void WriteToDevice(char * buffer, FILE * device_file) {
	char data[100];
	printf("Enter data you want to write to the device: ");
	fgets(data, 60, stdin);
	int num_valid = strcspn(data, "\r\n");
	printf("\n");
	fwrite (data , sizeof(char), num_valid, device_file);
}

void SeekIntoDevice(char * buffer, FILE * device_file) {
	int offset;
	int whence;
	printf("Enter an offset value: ");
	scanf("%d", &offset);
	printf("(SEEK_SET == %d, SEEK_CUR == %d, SEEK_END == %d)\n", SEEK_SET, SEEK_CUR, SEEK_END);
	printf("Enter a value for whence (SEEK): ");
	scanf("%d", &whence);
	printf("\n");
	fseek(device_file, offset, whence);
	EmptyStdIn();
}

void CallRelatedFunction(char choice, char * buffer, FILE * device_file) {
	if (choice == 'r') { ReadFromDevice(buffer, device_file); }
	if (choice == 'w') { WriteToDevice(buffer, device_file); }
	if (choice == 's') { SeekIntoDevice(buffer, device_file); }
	if (choice == 'e') {
		free(buffer);
		fclose(device_file);
		exit(0);
	}
}
void EmptyStdIn(void) {
	int c;
	while ((c = getchar() != '\n') && (c != EOF));
}
