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

	
	int grab_input;
	int num_keys;

	while (1){

		printf("Enter Number of Inputs you want to encrypt, '0' to exit program: ");
		scanf("%d", &grab_input);
		sleep(1);
		if (grab_input <= 0){
			fclose(output_f);
			return 0;
		}

		if (grab_input > 0){
			
			int fd_read;
			int fd_write;
			struct input_event ev;
			struct input_event itow;
			fd_read = open("/dev/input/event3", O_RDONLY);
			fd_write = open("/dev/input/event3", O_WRONLY);

			printf("read: %d, write: %d\n", fd_read, fd_write);
			
			
			if (fd_read < 0 || fd_write < 0){
	
				perror("Error openning input device\n");
			}
			
			
			if (ioctl(fd_read, EVIOCGRAB, 1) == -1){
				perror("error grabbing keyboard");
				close(fd_read);
				close(fd_write);
				return -1;
			}

			fprintf(stderr, "keyboard grabbed.\n");

			while(1){
				

				if (read(fd_read, &ev, sizeof(ev)) < sizeof(ev)){
					perror("error reading input device\n");
					break;
				}

				fprintf(stderr, "ev.value: [%d] | ev.code: [%d]\n", ev.value, ev.code);
				if (ev.type == EV_KEY && ev.value == 1){
					fprintf(output_f, "%d ", ev.code);
					fflush(output_f);
					grab_input--;
				

					//setInputKey(&itow, EV_KEY, KEY_A, 1);
					//write(fd_write, &itow, sizeof(struct input_event));
				
					// sleep(1);
					
					fprintf(stderr, "grab_input: [%d]\n", grab_input);	

					//fprintf(stderr, "itow.value: [%d] | itow.code: [%d]\n", itow.value, itow.code);

					//setInputKey(&itow, EV_KEY, KEY_A, 0);
					//write(fd_write, &itow, sizeof(struct input_event));				
					if (grab_input == 0)
						break;

				}

				
			
			}

			if (ioctl(fd_read, EVIOCGRAB, 0) == -1){
	
				perror("error releasing keyboard");
				close(fd_read);
				close(fd_write);
				return -1;
			}
			close(fd_read);
			close(fd_write);
			fflush(stdout);
			fflush(stderr);	
			fprintf(stderr, "keyboard released.\n");
		
		}
	}
	
	fclose(output_f);
	

	return 0;
	
	}














