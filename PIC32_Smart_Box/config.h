/*
 * File:   config.h
 * Author: Junyin Chen and Haonan Liu
 * Created on October 10, 2015
 */

#ifndef CONFIG_H
#define	CONFIG_H
#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>
// serial stuff
#include <stdio.h>

//=============================================================
// 40 MHz
#pragma config FNOSC = FRCPLL, POSCMOD = OFF
#pragma config FPLLIDIV = DIV_2, FPLLMUL = MUL_20 //40 MHz
#pragma config FPBDIV = DIV_1, FPLLODIV = DIV_2 // PB 40 MHz
#pragma config FWDTEN = OFF,  FSOSCEN = OFF, JTAGEN = OFF

//==============================================================
// Protothreads configure

// IF use_vref_debug IS defined, pin 25 is Vref output
//#define use_vref_debug

// IF use_uart_serial IS defined, pin 21 and pin 22 are used by the uart
#define use_uart_serial
#define BAUDRATE 9600 // must match PC terminal emulator setting

/////////////////////////////////
// set up clock parameters
// system cpu clock
#define sys_clock 40000000

// sys_clock/FPBDIV
#define pb_clock sys_clock // divide by one in this case

#endif	/* CONFIG_H */



// PORT B
#define EnablePullDownB(bits) CNPUBCLR=bits; CNPDBSET=bits;
#define DisablePullDownB(bits) CNPDBCLR=bits;
#define EnablePullUpB(bits) CNPDBCLR=bits; CNPUBSET=bits;
#define DisablePullUpB(bits) CNPUBCLR=bits;
//PORT A
#define EnablePullDownA(bits) CNPUACLR=bits; CNPDASET=bits;
#define DisablePullDownA(bits) CNPDACLR=bits;
#define EnablePullUpA(bits) CNPDACLR=bits; CNPUASET=bits;
#define DisablePullUpA(bits) CNPUACLR=bits;

//  a flag that registers if any button has been pushed
char pushed;
//  registers the status of 'confirm' button
unsigned int next_status=1;
//  registers the status of 'confirm' button
unsigned int confirm_status=1;