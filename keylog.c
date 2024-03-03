#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <string.h>



void setInputKey(struct input_event *event, int type, int code, int value){


	memset(event, 0, sizeof(struct input_event));

	event->type = type;
	event->code = code;
	event->value = value;

}

int main(){

	FILE *output_f;	
	output_f = fopen("log.txt", "w");
	fprintf(output_f, "Keys Pressed: \n");

	int num_input;

	while (1){

		printf("Enter Number of Inputs you want to encrypt, '0' to exit program: ");
		scanf("%d", &num_input);

		// struct input_event *input_arr = (struct input_event *)malloc(sizeof(struct input_event) * num_input);

		int *input_arr = (int *)malloc(sizeof(int) * num_input);


		sleep(1);
		if (num_input <= 0){
			fclose(output_f);
			return 0;
		}

		int idx = 0;

		if (num_input > 0){
			
			int fd;
			struct input_event ev;
			fd = open("/dev/input/event3", O_RDONLY);
			
			
			if (fd < 0){
	
				perror("Error openning input device\n");
			}
			
			
			if (ioctl(fd, EVIOCGRAB, 1) == -1){
				perror("error grabbing keyboard");
				close(fd);
				return -1;
			}

			fprintf(stderr, "keyboard grabbed.\n");

			while(1){
				
				if (read(fd, &ev, sizeof(ev)) < sizeof(ev)){
					perror("error reading input device\n");
					break;
				}

				// fprintf(stderr, "ev.value: [%d] | ev.code: [%d]\n", ev.value, ev.code);

				if (ev.type == EV_KEY && ev.value == 1){
					fprintf(output_f, "%d ", ev.code);
					fflush(output_f);
					num_input--;

					input_arr[idx] = ev.code;
					idx++;
					// sleep(1);
					//fprintf(stderr, "itow.value: [%d] | itow.code: [%d]\n", itow.value, itow.code);
					if (num_input == 0)
						break;

				}

			}

			if (ioctl(fd, EVIOCGRAB, 0) == -1){
	
				perror("error releasing keyboard");
				close(fd);
				return -1;
			}
			close(fd);
			fflush(stdout);
			fflush(stderr);	
			fprintf(stderr, "keyboard released.\n");
		
		}
		
		struct input_event itow;
		int fd_write;
		fd_write = open("/dev/input/event3", O_WRONLY);
		for (int i = 0; i < idx; ++i){
	
			// TODO: REPLACE 'KEY_A' WITH THE ENCRYPTED KEY
				// key_a is an int, with value 30 on my machine
				// the values are a bit weird, for some reason its not ascii values
				// q through p is 16 - 25
				// a through l is 30 - 38
				// z through m is 44 - 50

			// So the idea should be to make some algorithm that changes the key value to some other value within that range
				// Special characters, [[\;',./ , make up the values in between, so we should consider those cases as well
				// and allow those values in the range, i just didnt put them in the above ranges for clarity

		     	// The user inputted key values are saved in the array 'input_arr'
			
			// so loop through the input arr, run the encryption function on the values 
				// and then place that value where 'KEY_A' is in setinputkey.
			
			// if you test it rn, once you type in your input, x number of the letter 'a' will appear
			
			// An issue I noticed:
				// Won't work with capital letters
					// CAPS LOCK/SHIFT have their specific values however, so it would really just be handled
					// in the decryption phase, so we shouldnt worry about it now?	
			
			setInputKey(&itow, EV_KEY, KEY_A, 1);
			write(fd_write, &itow, sizeof(struct input_event));
			setInputKey(&itow, EV_KEY, KEY_A, 0);
			write(fd_write, &itow, sizeof(struct input_event));

		
		}
		close(fd_write);
		free(input_arr);

	}
	
	fclose(output_f);
	

	return 0;
	
	}

