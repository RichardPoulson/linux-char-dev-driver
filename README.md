# CSCI 3753 - Programming Assignment 2: Code a Character Device Driver
======
1. To build, simply open the root folder in your terminal window and execute the "make" command.  This will automatically do the following (note: will be built in ./simple_char_driver/):
  * build the simple_char_driver LKM
  * insert the LKM into the kernel
  * make the "test_driver" program, used to test the Driver
  * make a device file (sudo mknod -m 777 /dev/simple_character_device c 240 0)
2. Now that everything has been built and inserted, run "make test" to execute the test program.  The available commands should be displayed for you in the terminal.
3. After you are done, enter the command "make clean" and everything will be cleaned up, including:
  * deleting the "test_driver" program
  * deleting the simple_character_device device file
  * cleaning the directory where the LKM was built
  * and finally, removing the LKM from the kernel
