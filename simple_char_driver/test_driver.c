#include <stdio.h>
#include <stdlib.h> // malloc, free, exit
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>
#include <errno.h>
#include <string.h>

char PromptInput(void);
int ValidChoice(char choice);
void ReadFromDevice(char * buffer, FILE * device_file);
void WriteToDevice(char * buffer, FILE * device_file);
void SeekIntoDevice(char * buffer, FILE * device_file);
void CallRelatedFunction(char choice, char * buffer, FILE * device_file);

int main() {
	char * buffer = (char *) malloc(32);
	char * data = "abcd1234\n";
	FILE * device_file = fopen("/dev/simple_character_device","a+");

	char choice;
	while (1) {
		choice = PromptInput();
		while (ValidChoice(choice) == 0) { choice = PromptInput(); }
		CallRelatedFunction(choice, buffer, device_file);
	}


	fputs(data, device_file);
	fseek(device_file, 0, SEEK_SET);
	fgets(buffer, 9, device_file);
	fclose(device_file);
	printf("%s\n", buffer);
	free(buffer);

    return (0);
}

char PromptInput(void) {
	char input;
	printf("\n");
	printf("Press r to read from device\n");
	printf("Press w to write to the device\n");
	printf("Press s to seek into the device\n");
	printf("Press e to exit from the device\n");
	printf("anything else to keep reading or writing from the device\n");
	printf("Enter command: ");
	input = getchar();
	printf("\n");
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
	buffer = (char *) realloc(buffer, num);
	printf("\n");
	fgets(buffer, num, device_file);
	printf("Data read from the device: %s\n", buffer);
}

void WriteToDevice(char * buffer, FILE * device_file) {
	printf("Enter data you want to write to the device: ");
	char data[50];
	scanf("%s", data);
	printf("\n");
	fputs(&data, device_file);
}

void SeekIntoDevice(char * buffer, FILE * device_file) {
	int offset;
	int whence;
	printf("Enter an offset value: ");
	scanf("%d", &offset);
	printf("\n");
	printf("SEEK_SET == %d, SEEK_CUR == %d, SEEK_END == %d\n", SEEK_SET, SEEK_CUR, SEEK_END);
	printf("Enter a value for whence (SEEK): ");
	scanf("%d", &whence);
	printf("\n");
	fseek(device_file, offset, whence);
}

void CallRelatedFunction(char choice, char * buffer, FILE * device_file) {
	if (choice == 'r') { ReadFromDevice(buffer, device_file); }
	if (choice == 'w') { WriteToDevice(buffer, device_file); }
	if (choice == 's') { SeekIntoDevice(buffer, device_file); }
	if (choice == 'e') {
		free(buffer);
		exit(0);
	}
}
