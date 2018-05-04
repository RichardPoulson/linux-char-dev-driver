#include <stdio.h>
#include <stdlib.h> // malloc, free, exit
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <mtd/mtd-user.h>
#include <errno.h>
#include <string.h>

int main() {
	char * buffer = (char *) malloc(32);
	char * data = "abcd \n";
	FILE * device_file = fopen("/dev/simple_character_device","r+");
	if(device_file == NULL)
	{
		printf("Error!");
		exit(1);
	}
	fprintf(device_file, "%s", data);
	fscanf(device_file, "%s", buffer);
	fclose(device_file);
	printf("%s\n", buffer);
	free(buffer);

    return (0);
}
