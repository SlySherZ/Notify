# Notify
C++ command line tool that displays multiple configurable notifications on the screen for Windows.

## Compile
Open 'Notify.sln' file with Visual Studio and build either Debug or Release version.
I tested it with Visual Studio 2015.

## Run
You can now display notifications by running 'Notify.exe this is my test notification'.
The 'settings.txt' file should be on the same folder as the executable, and you can edit it to modify how the notifications appear.

The program creates a 'notify_slots.log' file that is used to keep track of how many notifications are already on the screen. 
