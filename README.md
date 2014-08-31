# Dots - simulator

Dots is a simple simulator of primitive cells (called Dots) that wander around a field, eat, reproduce and die. It was developed in C++, with GLUT for the user interface.

For more information on all the theory behind Dots, please read
the included pdf document.

## Building

A custom Makefile is available for compiling the program with GCC. The program requires GLU and freeglut to build and run. C++11 support in GCC must be available.

## Running

When running *dots*, the program will try to read the *config.txt*
file, situated in the same folder. The file contains many vital
variables, making it strictly necessary.

Dots of the Alpha type are shown in red, whereas the Beta
Dots are shown in blue. The colour, though, may vary upon the status:
- STATUS_HUNGRY: dark colour
- STATUS_EATING: "greener" colour
- STATUS_LOOKING: closer to magenta
- STATUS_GENERATING: magenta, for both types
- STATUS_DEAD: bright grey

Each time a Dot dies (transits to STATUS_DEAD), a small description
of the Dot is shown in the console.

Controls:

+ Space Bar: Pause or Resume the simulation.
It also shows some statistics through console output.

+ + / - : Increase / Decrease the simulation speed. By keeping
Shift presssed, you can increase/decrease the speed by 10
steps per second instead of 1. Initial speed is 4 steps per second.

+ Esc: Terminate the program.

## License

MIT

