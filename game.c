#include "TimeDelay.h"
#include "IOs.h"
#include "UART2.h"

extern int CN0flag;
extern int CN1flag;
extern int CN2flag;

int game_over_flag = 0;
int end_program_flag = 0;
int seq[16] = {2, 1, 1, 0};
int max_level = 4; // change these two later
void flash_single(int led_no, uint16_t time_ms_on, uint16_t time_ms_off){
    switch(led_no){
    case 0:
        LED0 = 1;
        delay_ms(time_ms_on);
        LED0 = 0;
        break;
     case 1:
        LED1 = 1;
        delay_ms(time_ms_on);
        LED1 = 0;
        break;
    case 2:
        LED2 = 1;
        delay_ms(time_ms_on);
        LED2 = 0;
    default:
        return; //display to string should be called?
    }
    delay_ms(time_ms_off);
    return;
}
void demo(int level, uint16_t end_delay_ms){
    int temp = 0;
    Display2String(" Sequence Is:                          \r ", 1);
    delay_ms(1000);
    while(temp < level){
        flash_single(seq[temp],500,500); //the flash interval
        //changes with difficulty
        temp++;
    }
    delay_ms(end_delay_ms);
    return;
}

int get_button_pressed(){
    if(CN0flag == 1){
        return(0);
    }
    else if(CN1flag == 1){
        return(1);
    }
    else if(CN2flag == 1){
        return(2);
    }
    else{
        return(-1);
    }
}

void listen(int level, uint16_t sleep_delay_ms, int difficulty){
    Display2String("Your Turn:                               \r ", 1);
    int counter = 0;
    while(counter < level){
        int time = sleep_delay_ms / difficulty;
        delay_ms(time);
        if(TMR2_FLAG == 1){
            Display2String("Time's Up!                               \r", 1);
            game_over_flag = 1;
            break;
        }
        int button_pressed = get_button_pressed();
        
        if(button_pressed == -1){ // this catches button bounces and helps with menu return
            continue;
        }
  
        if(button_pressed == seq[counter]){
            counter++;
        }
        else{
            Display2String("Incorrect Sequence - Game Over                 \r", 1);
            game_over_flag = 1;
            break;
        }
        CN0flag = 0;
        CN1flag = 0;
        CN2flag = 0; 
    }
    
    if(game_over_flag == 0){
        Display2String("Correct Sequence - Level Complete                   \r", 1);
        delay_ms(2000);
    }
}
void play(int difficulty){
    int level = 1;
    game_over_flag = 0;
    while((level <= max_level) && (game_over_flag == 0)){
        //seq[level] = rand() % 4; 
        //Display2Dec(seq[level]);
        //delay_ms(1500);
            //rand is the random number function
            //%2 limits the value to be in range [0,2).
        demo(level, 500);
        listen(level, 10000, difficulty);
        level++;
    }
    if(game_over_flag == 1){
        delay_ms(2000);
        return;
    }
    else{
        Display2String("YOU WIN                                  \r", 1);
        delay_ms(2000);
    }
    game_over_flag = 0;
    return;
}
int set_difficulty(){
    //unimplemented;
    return 1;
}
