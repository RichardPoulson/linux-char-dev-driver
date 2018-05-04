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
	char input = 'c';
	//while (input != 'e') {
	//	input = getchar();
	//}
	char * data = "abcd1234\n";
	FILE * device_file = fopen("/dev/simple_character_device","a+");
	fputs(data, device_file);
	fseek(device_file, 0, SEEK_SET);
	fgets(buffer, 9, device_file);
	fclose(device_file);
	printf("%s\n", buffer);
	free(buffer);

    return (0);
}
