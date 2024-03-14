#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

// Function to set input event properties
void setInputKey(struct input_event *event, __u16 type, __u16 code, __s32 value) {
    // Clear the memory of the input_event structure
    memset(event, 0, sizeof(struct input_event));
    // Set the type, code, and value of the event
    event->type = type;
    event->code = code;
    event->value = value;
}

int main(int argc, char *argv[]) {
    // File pointer for output file
    FILE *output_f; 
    // Open output file for writing
    output_f = fopen("log.txt", "w");
    // Write initial message to the output file
    fprintf(output_f, "Keys Pressed: \n");

    FILE *encrypted_f;
    encrypted_f = fopen("encrypted_log.txt", "w");
    fprintf(encrypted_f, "Encrypted Keys: \n");

    // Variables to store number of inputs and encryption flags
    int num_input;
    int ne = 0;
    int en = 0;

    if (argv[1] == NULL){
    
    	goto start;
    }

    // Check the command line argument to determine encryption mode
    if (strcmp(argv[1], "-noencrypt") == 0) {
        ne = 1; // Set no encryption flag
    } else if (strcmp(argv[1], "-encrypt") == 0) {
        en = 1; // Set no decryption flag
    }
    start:
    // printf("ne = %d, en = %d\n", ne, en);
    // Main loop to capture user input
    while (1) {
        // Prompt user to enter number of inputs
        printf("Enter Number of Inputs you want to encrypt, '0' to exit program: ");
        // Read number of inputs from user
        scanf("%d", &num_input);
        // Consume remaining characters in input buffer
        while (getchar() != '\n'); // Consume characters until newline is encountered

        // Check if the number of inputs is non-positive
        if (num_input <= 0) {
            // Close output file and exit program if number of inputs is 0 or negative
            fclose(output_f);
            return 0;
        }

        // Allocate memory for input array based on number of inputs
        __u16 *input_arr = (__u16 *)malloc(sizeof(__u16) * num_input);
        // Check if memory allocation was successful
        if (input_arr == NULL) {
            // Print error message if memory allocation fails
            perror("Memory allocation error");
            fclose(output_f);
            return 1;
        }

	sleep(1);


        // Variable to keep track of number of inputs captured
        int idx = 0;

        // Capture key events from input device
        if (num_input > 0) {
            // File descriptor for input device
            int fd;
            // Structure to hold input event
            struct input_event ev;
            // Open input device for reading
            fd = open("/dev/input/event3", O_RDONLY);
            
            // Check if input device is opened successfully
            if (fd < 0) {
                // Print error message if opening input device fails
                perror("Error opening input device");
                free(input_arr);
                fclose(output_f);
                return -1;
            }

            // Grab the keyboard device
            if (ioctl(fd, EVIOCGRAB, 1) == -1) {
                // Print error message if grabbing keyboard fails
                perror("Error grabbing keyboard");
                close(fd);
                free(input_arr);
                fclose(output_f);
                return -1;
            }

            // Inform user that keyboard is grabbed
            fprintf(stderr, "Keyboard grabbed.\n");

            // Loop to capture key events
            while (1) {
                // Read input event from device
                if (read(fd, &ev, sizeof(ev)) < sizeof(ev)) {
                    // Print error message if reading input device fails
                    perror("Error reading input device");
                    break;
                }

                // Check if input event is a key press event
                if (ev.type == EV_KEY && ev.value == 1) {
                    // Write key code to output file
                    fprintf(output_f, "%d ", ev.code);
                    // Flush output buffer to ensure data is written immediately
                    fflush(output_f);
                    // Decrement number of inputs remaining
                    num_input--;

                    // Save key code to input array
                    input_arr[idx] = ev.code;
                    // Increment input array index
                    idx++;
                    // Exit loop if all inputs are captured
                    if (num_input == 0)
                        break;
                }
            }

            // Release the keyboard device
            if (ioctl(fd, EVIOCGRAB, 0) == -1) {
                // Print error message if releasing keyboard fails
                perror("Error releasing keyboard");
                close(fd);
                free(input_arr);
                fclose(output_f);
                return -1;
            }

            // Close the input device
            close(fd);
            // Inform user that keyboard is released
            fprintf(stderr, "Keyboard released.\n");
        }

        // File descriptor for write device
        int fd_write;
        // Open write device for writing
        fd_write = open("/dev/input/event3", O_RDWR);
        // Check if write device is opened successfully
        if (fd_write < 0) {
            // Print error message if opening write device fails
            perror("Error opening write device");
            free(input_arr);
            fclose(output_f);
            return -1;
        }

	srand(time(NULL));
        // Loop to encrypt and write input events
        for (int i = 0; i < idx; ++i) {

		unsigned int min_value = 16;
		unsigned int max_value = 50;
		unsigned int random = 0;
		


		random = (rand() % (max_value - min_value + 1)) + min_value;

		
            // Check if no encryption is enabled
            if (ne == 1) {
                // Structure to hold input event for writing
                struct input_event itow;
                // Set input event properties
                setInputKey(&itow, EV_KEY, input_arr[i], 1);
                // Write input event to device
                write(fd_write, &itow, sizeof(struct input_event));
                // Set input event release state
                setInputKey(&itow, EV_KEY, input_arr[i], 0);
                // Write input event release to device
                write(fd_write, &itow, sizeof(struct input_event));
            } else if (en == 1){
	    	fprintf(encrypted_f, "%u ", random);
                // Structure to hold input event for writing
                struct input_event itow;
                // Set input event properties
                setInputKey(&itow, EV_KEY, random, 1);
                // Write input event to device
                write(fd_write, &itow, sizeof(struct input_event));
                // Set input event release state	
                setInputKey(&itow, EV_KEY, random, 0);
                // Write input event release to device
                write(fd_write, &itow, sizeof(struct input_event)); 
	    }

	    else {
	    
	    	fprintf(encrypted_f, "%u ", random);	
                struct input_event itow;
                // Set input event properties
                setInputKey(&itow, EV_KEY, input_arr[i], 1);
                // Write input event to device
                write(fd_write, &itow, sizeof(struct input_event));
                // Set input event release state
                setInputKey(&itow, EV_KEY, input_arr[i], 0); 
                // Write input event release to device
                write(fd_write, &itow, sizeof(struct input_event)); 
	    }


        }

        // Close the write device
        close(fd_write);
        // Flush standard input buffer
        fflush(stdin);
        // Free memory allocated for input array
        free(input_arr);
    }

    // Close output file
    fclose(output_f);
    fclose(encrypted_f);
    // Return 0 to indicate successful execution
    return 0;
}
