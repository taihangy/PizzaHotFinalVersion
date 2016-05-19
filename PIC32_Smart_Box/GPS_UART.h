#define backspace 0x7f // make sure your backspace matches this!
#define max_chars 256 // for input/output buffer

// define region numbers for Location method
#define NoGPSMapData 0
#define PhillipsHall 1
#define DuffieldHall 2
#define CarpenterLib 3
#define HollisterHall 4
#define ThurstonHall 5
#define KimballHall 6


//////////////////parsing////////////////////////////////////////////
float GPS_curLat;	//the variable to store current latitude
float GPS_curLon;	//the variable to store current longitude
char GPS_NS;		//the variable to decide North or South
char GPS_WE;		//the variable to decide West or East
char GPS_type[6];	
int  GPS_quality;	
char GPS_valid; // A=active, V=void
float GPS_time;		//the variable to store time
int GPS_time_h,GPS_time_m,GPS_time_s;	//helper variables to convert time
char GPS_region_str[30];	//string to store the location string
char GPS_region;	//region ID, as defined above

//====================================================================
// build a string from the UART2 /////////////
//////////////////////////////////////////////
char PT_term_buffer[max_chars];	//buffer for storing string from UART2
int num_char;					//number of characters in the string of UART2
int num_send_chars;				
static unsigned char character;	//the incoming character in the string
unsigned char GPGGA;			//to decide whether the incoming string begins with GPGGA

void parse(char* PT_term_buffer);	//the function to parse the string
void gps_get_region(void);		//the function to get the region ID, as well as update GPS_region_str[30]



//the child thread to get serial buffer
int PT_GetSerialBuffer(struct pt *pt)
{
    // mark the beginnning of the input thread
    PT_BEGIN(pt);

    num_char = 0;
    //memset(term_buffer, 0, max_chars);

    while(num_char < max_chars)
    {	//if frame work happens, just throw that away
        if(UART2GetErrors() & 0x02){
            UART2ClearAllErrors();
            PT_YIELD_TIME_msec(100);
            break;
        }
        // get the character
        // yield until there is a valid character so that other
        // threads can execute
        PT_YIELD_UNTIL(pt, UARTReceivedDataIsAvailable(UART2));
        character = UARTGetDataByte(UART2);

        //if the incoming character is '\n', that means the end of the string
        if(character == '\n'){        
            PT_term_buffer[num_char] = 0; // zero terminate the string
            break;
        }
        // backspace
        else if (character == backspace){
            PT_YIELD_UNTIL(pt, UARTTransmitterIsReady(UART2));
            UARTSendDataByte(UART2, ' ');
            PT_YIELD_UNTIL(pt, UARTTransmitterIsReady(UART2));
            UARTSendDataByte(UART2, backspace);
            num_char--;
            // check for buffer underflow
            if (num_char<0) {num_char = 0 ;}
        }
        else  {PT_term_buffer[num_char++] = character ;}
        
    } //end while(num_char < max_size)
    
		//If this string begins with $GPGGA
        if (PT_term_buffer[0] == '$' &&
        PT_term_buffer[1] == 'G' &&
        PT_term_buffer[2] == 'P' &&
        PT_term_buffer[3] == 'G' &&
        PT_term_buffer[4] == 'G' &&
        PT_term_buffer[5] == 'A'   ){
			//flag this variable	
            GPGGA=1;
			//and clear the string
            num_send_chars = 0;
            

        }
    

    // kill this input thread, to allow spawning thread to execute
    PT_EXIT(pt);
    // and indicate the end of the thread
    PT_END(pt);
}

//the function to parse the string
void parse(char* PT_term_buffer){
    int GPS_curLat_dec, GPS_curLon_dec;
    float GPS_curLat_frag, GPS_curLon_frag;
    sscanf(PT_term_buffer, "%6c,%f,%f,%c,%f,%c,%d", 
            GPS_type, &GPS_time, &GPS_curLat, &GPS_NS, &GPS_curLon,
            &GPS_WE, &GPS_quality);
//----------converting format degree,minute,second to degree----------
    
    GPS_curLat /= 100;
    GPS_curLon /= 100;
    GPS_curLat_dec = (int) GPS_curLat;
    GPS_curLon_dec = (int) GPS_curLon;
    GPS_curLat_frag = GPS_curLat - ((float) GPS_curLat_dec);
    GPS_curLon_frag = GPS_curLon - ((float) GPS_curLon_dec);
    GPS_curLat = ((float) GPS_curLat_dec) + GPS_curLat_frag/0.6;
    GPS_curLon = ((float) GPS_curLon_dec) + GPS_curLon_frag/0.6;

//------converting time to hour,min,sec, then convert UTC to EST------   
    
    GPS_time_h = ((int) GPS_time)/10000;
    GPS_time_m = (((int) GPS_time)%10000)/100;
    GPS_time_s = ((int) GPS_time)%100;
    if(GPS_time_h>=5)
        GPS_time_h -= 5;
    else
        GPS_time_h += 19;
}

//the function to get the region ID, as well as update GPS_region_str[30]
void gps_get_region(void){
    if (GPS_curLat<=42.445264 && GPS_curLat>=42.444603
                    && GPS_curLon<=76.483083 && GPS_curLon>=76.481516 ){
        sprintf(GPS_region_str,"Phillips Hall");
        GPS_region=PhillipsHall;
    }
    else if(GPS_curLat<=42.444551 && GPS_curLat>=42.444080
                    && GPS_curLon<=76.483083 && GPS_curLon>=76.481516 ){
        sprintf(GPS_region_str,"Duffield Hall");
        GPS_region=DuffieldHall;    
    }
    else if(GPS_curLat<=42.445115 && GPS_curLat>=42.444495
                    && GPS_curLon<=76.484560 && GPS_curLon>=76.483674 ){
        sprintf(GPS_region_str,"Carpenter Lib");
        GPS_region=CarpenterLib;
    }
    else if(GPS_curLat<=42.444474 && GPS_curLat>=42.443869
                    && GPS_curLon<=76.484980 && GPS_curLon>=76.484231 ){
        sprintf(GPS_region_str,"Hollister Hall");
        GPS_region=HollisterHall;
    }
    else if(GPS_curLat<=42.444214 && GPS_curLat>=42.443614
                    && GPS_curLon<=76.484191 && GPS_curLon>=76.483468 ){
        sprintf(GPS_region_str,"Thurston Hall");
        GPS_region=ThurstonHall;
    }
    else if(GPS_curLat<=42.444214 && GPS_curLat>=42.443614
                    && GPS_curLon<=76.483468 && GPS_curLon>=76.483084 ){
        sprintf(GPS_region_str,"Kimball Hall");
        GPS_region=KimballHall;
    }
    
    else{
        GPS_region=NoGPSMapData;
    }
}