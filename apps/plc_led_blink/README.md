NOTE: In order to compile this folder, you must add the function 'tfm_gpio_read()' to 'tfm_gpio_veneers.h' and 'tfm_gpio_veneers.c'
I have created this function in a separate branch from 'MASTER' and I have not merged the branches.
The updated files are in the 'Live Updates' Google Drive

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