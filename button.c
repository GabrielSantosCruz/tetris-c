
#include <intelfpgaup/KEY.h>
#include "button.h"

int buttonPressed(){
    int *data;
    if(KEY_open()){
        if(KEY_read(&data)){
            if(data == 1){
                printf("botao pressionado\n");
                KEY_close();
                return 1;
            }
        }
    }
}