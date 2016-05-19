#define backspace 0x7f // make sure your backspace matches this!
#define max_chars1 256 // for input/output buffer


//====================================================================
// build a string from the UART1 /////////////
//////////////////////////////////////////////
char PT_term_buffer1[max_chars1];
int num_char1;
int wifi_temp=0;

char IP_addr_string[32];
char Gateway_string[32];
char net_mask_string[32];
char wifi_buffer[256];
char rubbish_space[256];


int PT_GetWifiBuffer(struct pt *pt)
{
    static char character1;
    // mark the beginnning of the input thread
    PT_BEGIN(pt);

    num_char1 = 0;
    //memset(term_buffer, 0, max_chars1);

    while(num_char1 < max_chars1)
    {
        //if frame work happens, just throw that away
        if(UART1GetErrors() & 0x02){
            UART1ClearAllErrors();
            PT_YIELD_TIME_msec(100);
            break;
        }
        // get the character1
        // yield until there is a valid character1 so that other
        // threads can execute
        PT_YIELD_UNTIL(pt, UARTReceivedDataIsAvailable(UART1));
       // while(!UARTReceivedDataIsAvailable(UART1)){};
        character1 = UARTGetDataByte(UART1);
        if(character1 > 127 && character1 < 32){
            character1 = 0;
            break;
        }
        PT_YIELD_UNTIL(pt, UARTTransmitterIsReady(UART1));
//        UARTSendDataByte(UART1, character1);

        // unomment to check backspace character1!!!
        //printf("--%x--",character1 );

        // end line
        if(character1 == '\n' || character1 == 0){ 
            PT_term_buffer1[num_char1] = 0; // zero terminate the string
            //crlf; // send a new line
            PT_YIELD_UNTIL(pt, UARTTransmitterIsReady(UART1));
//            UARTSendDataByte(UART1, '\n');
//            wifi_temp = wifi_temp + 10;
            break;
        }
        // backspace
        else if (character1 == backspace){
            PT_YIELD_UNTIL(pt, UARTTransmitterIsReady(UART1));
//            UARTSendDataByte(UART1, ' ');
            PT_YIELD_UNTIL(pt, UARTTransmitterIsReady(UART1));
//            UARTSendDataByte(UART1, backspace);
            num_char1--;
            // check for buffer underflow
            if (num_char1<0) {num_char1 = 0 ;}
        }
        else  {PT_term_buffer1[num_char1++] = character1 ;}
         //if (character1 == backspace)

    } //end while(num_char1 < max_size)

    // kill this input thread, to allow spawning thread to execute
    PT_EXIT(pt);
    // and indicate the end of the thread
    PT_END(pt);
}

char PT_send_buffer[max_chars];
int num_send_chars ;
//====================================================================
// === DMA send string to the UART1 ==================================
int PT_DMA_PutSerialBuffer(struct pt *pt)
{
    PT_BEGIN(pt);
    //mPORTBSetBits(BIT_0);
    // check for null string
    if (PT_send_buffer[0]==0)PT_EXIT(pt);
    // sent the first character1
    PT_YIELD_UNTIL(pt, UARTTransmitterIsReady(UART1));
    UARTSendDataByte(UART1, PT_send_buffer[0]);
    //DmaChnStartTxfer(DMA_CHANNEL1, DMA_WAIT_NOT, 0);
    // start the DMA
    DmaChnEnable(DMA_CHANNEL1);
    // wait for DMA done
    //mPORTBClearBits(BIT_0);
    PT_YIELD_UNTIL(pt, DmaChnGetEvFlags(DMA_CHANNEL1) & DMA_EV_BLOCK_DONE);
    //wait until the transmit buffer is empty
    PT_YIELD_UNTIL(pt, U1STA&0x100);
    
    // kill this output thread, to allow spawning thread to execute
    PT_EXIT(pt);
    // and indicate the end of the thread
    PT_END(pt);
}