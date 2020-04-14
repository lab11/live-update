***The Ladder Logic Programming Pipeline***
Assuming you already know how to program in ladder logic, use LDmicro to write your program,
then compile yout program as an ANSI C file by going to 'Settings -> Microcontroller' and selecting C code.

Take note of your desired cycle time. You will need this information to call the plc function periodically.

LDmicro will autogenerate C code with function prototypes where I/O functionality is required, (buttons, LEDs, switches, I2C, etc.) which the user is required to define in a header file 'ladder.h' and an accompanying source file 'ladder.c'. We have written skeletons of these files that include required  libraries and functions, as well as the 'main.c' file that goes along with it. 

To make this process faster and easier on the ARM Musca Board, we have implemented a platform file 'plc_platform.h' that has helper functions that the user can call to implement required behavior. Function names should match the functionality required by the LDmicro function prototypes.
e.g. 'Write_U_b_Yled(v)' corrosponds to 'write_LED(color, v)', where the user chooses an LED color (r, g, b) and passes the original parameter 'v' to the helper function.

*In general, edits need only be made in 'ladder.h' and 'ladder.c'*

Application Folder structure:
- live-update/apps/
  - Application Folder/
    - src/
      - 'main.c'
      - 'ladder.c' (*EDIT*)
      - 'plc_platform.c' (may move this to a hidden folder)
    - include/
      - 'ladder.h' (*EDIT*)
      - 'plc_platform.h' (may move this to different folder)
    - 'Makefile'
    - 'README.md' (you're reading this file)


---

***Compiling and Flashing C Code to Musca***

On your local machine, in your git repo
'''bash
vagrant up
vagrant ssh
'''
This starts up the vagrant vm where you compile the code.
Now navigate to your application folder in '/vagrant'
'''bash
cd /vagrant/apps/
cd your_folder
make
'''
Make sure you have a Makefile and have all your '.c' files in 'src' and your '.h' files in 'include'
This should compile your code and the change ought to be reflected on your local machine

return to your local machine (open a new terminal window or exit vagrant using 'exit' command)

Now turn on the musca board by hitting the 'PBON' button and run
'''bash
make flash
'''
*Remember to wait for the orange 'PWR' LED to stop flashing*
This indicates that the code has finished uploading.

run 'make serial' and select the number corrosponding to 'DAPLink CMSIS-DAP' to view the stdio of the board

