pRiser.pde
-------------

This program descends from Bill Welch's original p_logger file, but
has been modified to plot traces of

 channel 0 temperature
 channel 0 rate of rise
 channel 1 temperature
 channel 1 rate of rise

The rate of rise values are exaggerated by 10X.  So a plotted value of 200
corresponds to a rise rate of 20.0 degrees F per minute.

This is the Processing half of the application that runs on the PC.

The arduino half of the application, which must be flashed to the arduino
board, is aRiser.pde.


Jim Gallt
7/1/2010
