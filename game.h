/* 
 * File:   game.h
 * Author: cartermarcelo
 *
 * Created on November 21, 2022, 3:25 PM
 */
#include "IOs.h"
#include "TimeDelay.h"
#include <stdlib.h>
#ifndef GAME_H
#define	GAME_H
#endif	/* GAME_H */
extern int game_over_flag; //Triggers game over routine when set to 1;
extern int end_program_flag; //Prompts the entire program to stop
//since exit(-1); doesn't exist
void demo(int level, uint16_t end_delay_ms);
void listen(int level, uint16_t sleep_delay_ms);
void flash_single(int led_no, uint16_t time_ms_on, uint16_t time_ms_off);
void play(int difficulty);
int set_difficulty();