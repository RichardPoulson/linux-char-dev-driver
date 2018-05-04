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
void ReadFromDevice();
void WriteToDevice();
void SeekIntoDevice();
void CallRelatedFunction(char choice);

int main() {
	char * buffer = (char *) malloc(32);
	char * data = "abcd1234\n";
	FILE * device_file = fopen("/dev/simple_character_device","a+");

	char choice;
	while (1) {
		choice = PromptInput();
		while (ValidChoice(choice) == 0) { choice = PromptInput(); }
		CallRelatedFunction(choice);
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

void ReadFromDevice() {
	printf("How many bytes would you like to read from the file? ");
	int num = getint();
	buffer = (char *) realloc(buffer, num);
	printf("\n");
}
void WriteToDevice() {
	printf("Enter data you want to write to the device: ");
	char a_word[50];
	scanf ("%s", a_word);
	printf("\n");
    printf ("You entered: %s\n", a_word);
}
void SeekIntoDevice() {

}
void CallRelatedFunction(char choice) {
	if (choice == 'r') { ReadFromDevice(); }
	if (choice == 'w') { WriteToDevice(); }
	if (choice == 's') { SeekIntoDevice(); }
	if (choice == 'e') {
		free(buffer);
		exit(0);
	}
}
