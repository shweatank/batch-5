#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <stdlib.h>

#define DEVICE_PATH "/dev/input/event6"
#define LED_GPIO "/sys/class/gpio/gpio17/value"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void write_led(int value)
{
    FILE *f = fopen(LED_GPIO, "w");
    if (!f) return;
    fprintf(f,"%d",value);
    fclose(f);
}

int main()
{
    int fd = open(DEVICE_PATH, O_RDONLY);
    if(fd < 0) {
        perror("Cannot open input device");
        return 1;
    }

    struct input_event ev;
    int x=SCREEN_WIDTH/2, y=SCREEN_HEIGHT/2;

    while(read(fd, &ev, sizeof(ev)) == sizeof(ev)) {

        if(ev.type == EV_REL){
            if(ev.code == REL_X) x+=ev.value;
            if(ev.code == REL_Y) y+=ev.value;
        }

        if(ev.type == EV_KEY && ev.code == BTN_LEFT && ev.value==1){
            printf("Click at X:%d Y:%d\n",x,y);

            if(x>SCREEN_WIDTH/2 && y<SCREEN_HEIGHT/2){
                printf("RIGHT TOP → LED ON\n");
                write_led(1);
            } else if(x>SCREEN_WIDTH/2 && y>SCREEN_HEIGHT/2){
                printf("RIGHT BOTTOM → LED BLINK\n");
                for(int i=0;i<5;i++){
                    write_led(1);
                    usleep(200000);
                    write_led(0);
                    usleep(200000);
                }
            } else if(x<SCREEN_WIDTH/2 && y<SCREEN_HEIGHT/2){
                printf("LEFT TOP → UART MESSAGE\n");
            } else {
                printf("LEFT BOTTOM → LED OFF\n");
                write_led(0);
            }
        }
    }

    close(fd);
    return 0;
}
