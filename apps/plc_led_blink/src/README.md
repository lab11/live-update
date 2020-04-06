***Using plc_platform***

'plc_platform.h' defines all of the GPIO pins on the musca board, as well as functions to write to them.

The structure of the platform is such that all reading and writing to the gpio pins is done all at once outside the context of the 'PlcCycle()' function.

The auto-generated c code from LDmicro ('LDmicro.c' from now on) requires that the user write 'ladder.h' and 'ladder.c' and define the functions specified in 'LDmicro.c'. 

The given files 'ladder.*' in the 'src' folder have comments that indicate what needs to be user defined.
Namely:
-any undefined functions from 'LDmicro.c'
-an array of gpio pins that the user would like to use as outputs
-the number of output-enabled gpio pins
-whether the user will use the on-board LED

When defining the functions from 'LDmicro.c', there are corrosponding functions in plc_platform.h. 
For instance, if LDmicro generates a function called 'Write_U_b_Yled(BOOL v)' The user would add the declaration to 'ladder.h' and define it in 'ladder.c' using the 'write_LED(LED color)' function from 'plc_platform.h' (assuming the on-board leds are being used here) with a color specified.

the platform is still incomplete and does not have corrosponding functions for every feature of LDmicro yet. 