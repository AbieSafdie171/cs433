# CS 433 Project - Encrypted Keylogging

## GROUP MEMBERS
	Abie Safdie, Jaren Alexander, Cynthia 

## How to Run:
	
In terminal:

`make all`

`sudo ./keylog`

`sudo ./keylog -encrypt`

Note: This works by grabbing and capturing inputs by an IO input device. The keyboard on the machine we were worked on was: /dev/input/event3

This will most likely vary machine by machine. So this value will have to changed.

Enter the number of inputs you want to be encrypted and then start typing
After you complete all of your inputs, will the encrypted letters appear wherever you started typing.


After the completion of the program, a log of all keys inputted will be saved as `log.txt` in the projects directory.	

The values sent to the application will be stored in `encrypted.txt`


## How it works:


Our keylogging program grabs control of the keyboard, so our program has exclusive access. 
This way, no intermediate keyloger, or any other applicaiton, can read input devices before we encrypt the input. 
The encryptino is based on the system clock, so values are random. This will make decryption impossible by the intermediate keylogger. 
To decrypt, the trusted application will ask the OS to look into its table to receive the plain text. 
This decryption aspect was not explicitly implemented because it would need the application side as well, something we did not focus on.

The user decides how many keys they want to encrypt, this way it reduces overhead of encyrpting every keystroke the user types.
Therefore, only when a user wants to input sensitive data does the encryption take place.