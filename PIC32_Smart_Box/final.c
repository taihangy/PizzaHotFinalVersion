
////////////////////////////////////
#include "config.h"
// threading library
#include "pt_cornell_1_2_1.h"
// graphics libraries
#include "tft_master.h"
#include "tft_gfx.h"
// header file for temperature sensor
#include "ds18s20.h"
// for math operations
#include <math.h>
#include <string.h>
// for UART setup
#include "GPS_UART.h"
#include "WIFI_UART.h"
// for navigation functions
#include "GPS_Navigation.h"
// for setting up menu
#include "menu.h"
// Bruce's voice record
#include "Left_Right_sound.h"
#include <stdlib.h>

// === thread structures ============================================
// thread control structs
// note that UART input and output are threads
static struct pt pt_input1,pt_input2, pt_output, 
                pt_DMA_output, pttemp, pt_tft,pt_gps_uart,pt_wifi_uart,pt_key,
                pt_GPS_Navigation;
// system 1 second interval tick
int sys_time_seconds=0;
// for tft sprintf
char buffer[60];
//set dma channel
int dmaChn=0;
// a median function that could reduce the error rate
float median(float* temperature);


// ========= GPS Navigation Thread  ==================================
// This thread interfaces with the user and navigates the user to his dest
static PT_THREAD (protothreadGPS_Navigation(struct pt *pt))
{	////standard thread structure
    PT_BEGIN(pt);
      while(1) {
         
		//In Navigation - Engineering Quad and a destination choosen 
        if(next_status>=30 && next_status<40 && confirm_status==2){
			// load the waypoints from the arrays in routes.h
            if(navigation_start==1){
                Nav_route_lon = get_route_lon();
                Nav_route_lat = get_route_lat();
                Nav_route_size = get_route_size();
                //set the start flag back to 0
				navigation_start=0;
            }
			// //check if the user is at the destination, if so, go back to the dest choose menu
   //          if(Nav_route_size==1){
   //              confirm_status=2;
   //          }
            //a route to the dest has successfully loaded
            if((Nav_route_lon!=0) && (Nav_route_lat!=0) && (Nav_route_size!=0)){
                // check if the user gets close to the waypoint 
				cls = waypoint_close(GPS_curLon,GPS_curLat,
                        -Nav_route_lon[waypoint_count],Nav_route_lat[waypoint_count]);
                //the user is close enough to waypoint
                if(cls==1){
					//check if the waypoint is the last one. that means destination arrived 
                    if(waypoint_count==Nav_route_size-1){
                        navigation_arrival=1;
                        waypoint_close_flag=1;
						//wait for 5 sec then go back to the dest choose menu
                        PT_YIELD_TIME_msec(5000);
                        waypoint_close_flag=0;
                        waypoint_count=1;
                        navigation_arrival=0;
                        confirm_status=3;
                    }
                    // user on the way
                    else{
						//call function get_leftright to tell whether the user should turn left or right
                        GPS_lr = get_leftright(Nav_route_lat[waypoint_count-1],-Nav_route_lon[waypoint_count-1], 
                                                Nav_route_lat[waypoint_count], -Nav_route_lon[waypoint_count], 
                                                Nav_route_lat[waypoint_count+1], -Nav_route_lon[waypoint_count+1]);
						//call function get_angle to tell the angle the user should turn
                        GPS_deg = get_angle(-Nav_route_lon[waypoint_count-1],Nav_route_lat[waypoint_count-1], 
                                            -Nav_route_lon[waypoint_count], Nav_route_lat[waypoint_count],  
                                            -Nav_route_lon[waypoint_count+1], Nav_route_lat[waypoint_count+1]);

                        // display the message of next waypoint on the tft screen
                        if(GPS_lr==GPS_Left && GPS_deg > 30){
							//play the "turn left" human voice 
                            DmaChnSetTxfer(dmaChn, Turn_left, (void*)&CVRCON, sizeof(Turn_left), 1, 1);
                            DmaChnEnable(dmaChn);
							//yield until the voice play is finished 
                            PT_YIELD_TIME_msec(1306);
                            // DmaChnDisable(dmaChn);
                            DmaChnSetTxfer(dmaChn, Noise_rejection, (void*)&CVRCON, sizeof(Noise_rejection), 1, 1);
                        }
                        else if(GPS_lr==GPS_Right && GPS_deg > 30){
							//play the "turn right" human voice 
                            DmaChnSetTxfer(dmaChn, Turn_right, (void*)&CVRCON, sizeof(Turn_right), 1, 1);
                            DmaChnEnable(dmaChn);
							//yield until the voice play is finished 
                            PT_YIELD_TIME_msec(1411);
                            DmaChnSetTxfer(dmaChn, Noise_rejection, (void*)&CVRCON, sizeof(Noise_rejection), 1, 1);
                        }
                        else{
							//disable the dma channel
                            DmaChnSetTxfer(dmaChn, Noise_rejection, (void*)&CVRCON, sizeof(Noise_rejection), 1, 1);
                        }
                        //set the close flag to 1
                        waypoint_close_flag=1;
						//yield the thread until user is far away enough from the current waypoint  
                        PT_YIELD_UNTIL(pt,waypoint_close(GPS_curLon,GPS_curLat,
                                -Nav_route_lon[waypoint_count],Nav_route_lat[waypoint_count])==0);
                        //set the close flag back to 1
						waypoint_close_flag=0;
						//set next way point as the current waypoint
                        waypoint_count++;
                    }
                }
            }

        }
            PT_YIELD_TIME_msec(1000);
            // NEVER exit while
      } // END WHILE(1)
  PT_END(pt);
} // end of GPS Navigation Thread


// ========= GPS UART Thread  =======================================================
// This thread communicates with the GPS receivor module through UART
static PT_THREAD (protothreadgps_uart(struct pt *pt))
{   //standard thread structure
    PT_BEGIN(pt);
      while(1) {
		    // Spawn until a valid NMEA sentence received
            PT_SPAWN(pt,&pt_input2,PT_GetSerialBuffer(&pt_input2));
			// if the  sentence has a GPGGA format 
            if(GPGGA==1){   
                GPGGA=0;
				//parse the GPGGA sentence
                parse(PT_term_buffer);
            }
			// get the location and the nearby landmark
            gps_get_region();
            PT_YIELD(pt);
            // NEVER exit while
      } // END WHILE(1)
  PT_END(pt);
} // End of GPS UART Thread


// ========= WIFI UART Thread  =======================================================
// This thread communicates with the WIFI module through UART1
static PT_THREAD (protothreadwifi_uart(struct pt *pt))
{   //standard thread structure
    static int wifi_counter = 0;
    PT_BEGIN(pt);
      while(1) {
		    // Spawn until a valid NMEA sentence received
            PT_SPAWN(pt,&pt_input1,PT_GetWifiBuffer(&pt_input1));
            if(PT_term_buffer1[0]=='I'&&PT_term_buffer1[1]=='P'&&PT_term_buffer1[3]=='a'){
                sprintf(IP_addr_string,PT_term_buffer1);
            }

            else if(PT_term_buffer1[0]=='s'&&PT_term_buffer1[1]=='i'&&PT_term_buffer1[2]=='z'&&PT_term_buffer1[3]=='e'){
                sscanf(PT_term_buffer1, "%4c,%d", wifi_buffer, &Nav_route_size_temp);
                // float* Nav_route_lon_temp;   //the pointer to store parsed data from wifi
                // float* Nav_route_lat_temp;   //the pointer to store parsed data from wifi
                free(Nav_route_lat_temp);
                free(Nav_route_lon_temp);
                Nav_route_lat_temp = malloc(Nav_route_size_temp * sizeof(float));
                Nav_route_lon_temp = malloc(Nav_route_size_temp * sizeof(float));
                route_scheduling_counter = 0;
            }
            else if (PT_term_buffer1[0]=='l'&&PT_term_buffer1[1]=='a'&&PT_term_buffer1[2]=='t')
            {
                sscanf(PT_term_buffer1, "%3c,%f,%3c,%f", 
                    wifi_buffer, &Nav_route_lat_temp[route_scheduling_counter], 
                    rubbish_space, &Nav_route_lon_temp[route_scheduling_counter]);
                    route_scheduling_counter++;
            }
            else if(PT_term_buffer1[0]=='d'&&PT_term_buffer1[1]=='o'&&PT_term_buffer1[2]=='n'&&PT_term_buffer1[3]=='e'){
                route_scheduling_counter = 0;
            }
            sprintf(wifi_buffer,PT_term_buffer1);

            PT_YIELD(pt);
            // NEVER exit while
      } // END WHILE(1)
  PT_END(pt);
} // End of GPS UART Thread






// ========= Key Scanning Thread  ======================================
// This Thread reads the input of buttons and does the debounce 
static PT_THREAD (protothreadkey(struct pt *pt))
{ 	//standard Thread structure
    PT_BEGIN(pt);
      while(1) {
            PT_YIELD(pt);
			// check if any of the button is pressed
            if(mPORTBReadBits(BIT_8 | BIT_9)){
				// just wait 30 msec and check again to see if it is noise or a real button press 
                PT_YIELD_TIME_msec(30);
                if(mPORTBReadBits(BIT_8 | BIT_9)){
					// not noise, then set the pushed falg to 1 to tell the program a button has been pushed
                    pushed=1;
                }                
            }
            // if a button is pushed
            if(pushed==1){
				// set the  pushed falg back to 0 
                pushed=0;
				// check if the "next" button is pressed
                if(mPORTBReadBits(BIT_8)){
					// increase next_status by 1
                    next_status++;
					//yield the thread until the button is released
                    PT_YIELD_UNTIL(pt,!mPORTBReadBits(BIT_8));
                }
				//check if the "confirm" button is pressed
                if(mPORTBReadBits(BIT_9)){
					// increase confirm_status by 1
                    confirm_status++;
					//yield the thread until the button is released
                    PT_YIELD_UNTIL(pt,!mPORTBReadBits(BIT_9));
                }
            } // NEVER exit while
      } // END WHILE(1)
  PT_END(pt);
} // End of GPS UART Thread



// ========= Temperature Sensor Thread  ================================
// this thread communicates with temperature sensor and reads the temp
static PT_THREAD (protothreadtemp(struct pt *pt))
{ 
    PT_BEGIN(pt);
    static int i,m;
    static float temperature[3];
      while(1) {
            PT_YIELD(pt);
                //first reset the sensor
				if(Reset_1820()==1){

                    Write_1820(0xcc);   //0xcc -- Skip ROM
                    Write_1820(0x44);   //0x44 -- Converting Temperature
                    PT_YIELD_TIME_msec(750);

                    Reset_1820();
                    Write_1820(0xcc);   //0xcc -- Skip ROM
                    Write_1820(0xbe);   //0xbe -- Reading Temperature
                    tempL = Read_1820();//read the lower bits of the temp
                    tempH = Read_1820();//read the higher bits of the temp
					//parse the temp 
                    temperature[m] = (tempL>>1)+(tempL&0x01)*0.5;
                    if(tempH!=0)    temperature[m] = -temperature[m];
                    m++;
                    tempf = median(temperature);
                    if(m==3)    m=0;          
                }
                else{
                    tempf = 1000;
                }
            // NEVER exit while
      } // END WHILE(1)
  PT_END(pt);
} // Temperature Sensor Thread

// ========================== TFT Thread ================================
// displays UI and information on the TFT screen
// Very straight forward, easy to understand
static PT_THREAD (protothread_tft(struct pt *pt))
{
    PT_BEGIN(pt);
      while(1) {
            // yield time 1 second
            PT_YIELD_TIME_msec(1000) ;
            
            sys_time_seconds++ ;
            
            tft_setCursor(0, 0);
            tft_setTextColor(ILI9340_WHITE); tft_setTextSize(1);
            sprintf(buffer,"Pizza Hot: ty322,jc2954,bw476");
            tft_writeString(buffer);  
            
            tft_fillRect(0, 20, 320, 20, ILI9340_BLACK);//x,y,w,h,color
            tft_setCursor(0, 20);
            tft_setTextColor(ILI9340_YELLOW); tft_setTextSize(2);
            sprintf(buffer,"Time since boot: %d",sys_time_seconds);
            tft_writeString(buffer);  
  
                       
            tft_drawRect(0,38,320,180,ILI9340_YELLOW);
            drawmenu();

//            if(next_status>=40 && next_status<50 && confirm_status==2){

//            } 

       
            // NEVER exit while                                                                   
      } // END WHILE(1)
  PT_END(pt);
} // end of thread 

// === Main  ======================================================
// set up UART, timer2, threads
// then schedule them as fast as possible
int main(void)
{   
    // === config the uart, DMA, vref, timer5 ISR =============
    PT_setup();

    // === setup system wide interrupts  ====================
    INTEnableSystemMultiVectoredInt();
    
    // init the display
    tft_init_hw();
    tft_begin();
    tft_fillScreen(ILI9340_BLACK);
	// Use tft_setRotation(3) for 320x240
    tft_setRotation(3); 
    //Set RPB0 as digital output    
    mPORTASetPinsDigitalOut(BIT_0);
    //Set RPB8,BPB9 as digital input  
    mPORTBSetPinsDigitalIn(BIT_8 | BIT_9);
    //enable the pull down circuit inside the circuit for port pin 8&9 
	EnablePullDownB( BIT_8 | BIT_9);
    
    CVRCON = 0x8060;

    // Open the desired DMA channel.
    // We enable the AUTO option, we'll keep repeating the sam transfer over and over.
    DmaChnOpen(dmaChn, 0, DMA_OPEN_AUTO);

    // set the transfer event control: what event is to start the DMA transfer
    // In this case, timer3 
    DmaChnSetEventControl(dmaChn, DMA_EV_START_IRQ(_TIMER_3_IRQ));
    DmaChnSetTxfer(dmaChn, Noise_rejection, (void*)&CVRCON, sizeof(Noise_rejection), 1, 1);
    DmaChnEnable(dmaChn);

    // === set up DMA for UART output =========
    // configure the channel and enable end-on-match
    DmaChnOpen(DMA_CHANNEL1, DMA_CHN_PRI2, DMA_OPEN_MATCH);
    // trigger a byte everytime the UART is empty
    DmaChnSetEventControl(DMA_CHANNEL1, DMA_EV_START_IRQ_EN|DMA_EV_MATCH_EN|DMA_EV_START_IRQ(_UART1_TX_IRQ));
    // source and destination
    DmaChnSetTxfer(DMA_CHANNEL1, PT_send_buffer+1, (void*)&U1TXREG, max_chars, 1, 1);
    // signal when done
    DmaChnSetEvEnableFlags(DMA_CHANNEL1, DMA_EV_BLOCK_DONE);
    // set null as ending character (of a string)
    DmaChnSetMatchPattern(DMA_CHANNEL1, 0x00);
    
    
    
    

    // now use the 32 bit timer to generate an interrupt to start the
    // DMA burst ever 125 ticks
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_8, 610); //125
    
    // init the threads
    PT_INIT(&pttemp);
    PT_INIT(&pt_tft);
    PT_INIT(&pt_gps_uart);
    PT_INIT(&pt_wifi_uart);
    PT_INIT(&pt_key);
    PT_INIT(&pt_GPS_Navigation);

    // schedule the threads
    while(1) {                
      // round robin
      PT_SCHEDULE(protothreadtemp(&pttemp));
      PT_SCHEDULE(protothread_tft(&pt_tft));   
      PT_SCHEDULE(protothreadgps_uart(&pt_gps_uart));
      PT_SCHEDULE(protothreadwifi_uart(&pt_wifi_uart));
      PT_SCHEDULE(protothreadkey(&pt_key));
      PT_SCHEDULE(protothreadGPS_Navigation(&pt_GPS_Navigation)); 
    }
} // main

// a median function that could reduce the error rate of the temp
float median(float* temperature){
    float temp[3],temp1;
    int j,k;
    for(j=0;j<3;j++){
        temp[j]=temperature[j];
    }
    for(j=0;j<3;j++){
        for(k=j;k<3;k++){
            if(temp[j]>temp[k]){
                temp1=temp[j];
                temp[j]=temp[k];
                temp[k]=temp1;
            }
        }
    }
    return temp[1];
}
