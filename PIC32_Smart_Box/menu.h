char waypoint_close_flag=0;
char navigation_arrival=0;
char navigation_start=0;
unsigned int waypoint_count=1;

void drawmenu(void);//Display Menu,1-9
// 1. Weather 2.Location 3.Navigation 4.Wifi Settings 5.Go to 1 Other.Wrong Press
void drawmeanu1_1(void);//Display Weather Information,10-19
// Weather Information 10.Back Other.Wrong Press
void drawmeanu2_1(void);//Display Location Information,20-29
// Location Information 20.Back Other.Wrong Press
void drawmeanu3_1_navigation(void);//Display Navigation Information
void drawmeanu4_1(void);//Display WIFI Settings,40-49



char menubuffer[60];

void drawmenu(void){
    
    if(next_status>0 && next_status<10){
        
        
        if(confirm_status>2){
            confirm_status=2;
        }
        tft_fillRect(120, 40, 190, 20, ILI9340_BLACK);//x,y,w,h,color
        tft_setCursor(120, 40);
        tft_setTextColor(ILI9340_WHITE); tft_setTextSize(2);
        sprintf(menubuffer,"MENU");
        tft_writeString(menubuffer);

        tft_fillRect(60, 60, 250, 20, ILI9340_BLACK);//x,y,w,h,color
        tft_setCursor(60, 60);
        tft_setTextColor(ILI9340_CYAN); tft_setTextSize(2);
        sprintf(menubuffer,"Temperature");
        tft_writeString(menubuffer);

        tft_fillRect(60, 80, 250, 20, ILI9340_BLACK);//x,y,w,h,color
        tft_setCursor(60, 80);
        tft_setTextColor(ILI9340_CYAN); tft_setTextSize(2);
        sprintf(menubuffer,"Location");
        tft_writeString(menubuffer);

        tft_fillRect(60, 100, 250, 20, ILI9340_BLACK);//x,y,w,h,color
        tft_setCursor(60, 100);
        tft_setTextColor(ILI9340_CYAN); tft_setTextSize(2);
        sprintf(menubuffer,"Navigation");
        tft_writeString(menubuffer);
        
        tft_fillRect(60, 120, 250, 20, ILI9340_BLACK);//x,y,w,h,color
        tft_setCursor(60, 120);
        tft_setTextColor(ILI9340_CYAN); tft_setTextSize(2);
        sprintf(menubuffer,"WIFI Settings");
        tft_writeString(menubuffer);

        tft_fillRect(10, 160, 250, 20, ILI9340_BLACK);//x,y,w,h,color
        tft_setCursor(10, 160);
        tft_setTextColor(ILI9340_YELLOW); tft_setTextSize(2);
        sprintf(menubuffer,"ECE MEng - Pizza Hot");
        tft_writeString(menubuffer);

        tft_fillRect(10, 180, 250, 20, ILI9340_BLACK);//x,y,w,h,color
        tft_setCursor(10, 180);
        tft_setTextColor(ILI9340_YELLOW); tft_setTextSize(2);
        sprintf(menubuffer,"Taihang, Junyin, Baoyue");
        tft_writeString(menubuffer);
    
        switch (next_status){

        
            case 1:
                tft_fillRect(20, 60, 20, 60, ILI9340_BLACK);//x,y,w,h,color
                tft_fillCircle(30, 70,3,ILI9340_CYAN);//x,y,r,color
                if(confirm_status==2){
                    next_status*=10;
                }
                break;
            case 2:
                tft_fillRect(20, 60, 20, 60, ILI9340_BLACK);//x,y,w,h,color
                tft_fillCircle(30, 90,3,ILI9340_CYAN);//x,y,r,color
                if(confirm_status==2){
                    next_status*=10;
                }
                break;
            case 3:
                if(confirm_status>=2){
                    //clear screen
                    tft_fillRect(2, 60, 315, 140, ILI9340_BLACK);//x,y,w,h,color 
                    next_status*=10;
                    navigation_start = 1;
                    waypoint_count = 1;
                    break;
                }
                tft_fillRect(20, 60, 20, 60, ILI9340_BLACK);//x,y,w,h,color
                tft_fillCircle(30, 110,3,ILI9340_CYAN);//x,y,r,color
                break;
            case 4:
                if(confirm_status>=2){
                    //clear screen
                    tft_fillRect(2, 60, 315, 140, ILI9340_BLACK);//x,y,w,h,color 
                    next_status*=10;
                }
                tft_fillRect(20, 60, 20, 80, ILI9340_BLACK);//x,y,w,h,color
                tft_fillCircle(30, 130,3,ILI9340_CYAN);//x,y,r,color
                break;
            case 5:
                tft_fillRect(20, 60, 20, 80, ILI9340_BLACK);//x,y,w,h,color
                next_status = 1;
                tft_fillCircle(30, 70,3,ILI9340_CYAN);//x,y,r,color
                break;
            // case 5 - 9
            default:
                tft_fillRect(0, 60, 320, 140, ILI9340_BLACK);//x,y,w,h,color
                next_status = 1;
                break;
                
        }
    }

     
    if(next_status>=10 && next_status<20){
        
        if(confirm_status>3){
            confirm_status=3;
        }
        //clear screen
        tft_fillRect(2, 60, 315, 140, ILI9340_BLACK);//x,y,w,h,color
        //build sub-menu
        drawmeanu1_1();
        //build "Back"
        tft_fillRect(270, 180, 45, 20, ILI9340_BLACK);//x,y,w,h,color
        tft_setCursor(270, 180);
        tft_setTextColor(ILI9340_WHITE); tft_setTextSize(2);
        sprintf(menubuffer,"Back");
        tft_writeString(menubuffer);
        //build cursor
        switch (next_status){
        case 10:
            tft_fillCircle(250, 190,3,ILI9340_CYAN);//x,y,r,color
            if(confirm_status==3){
                tft_fillRect(0, 60, 320, 140, ILI9340_BLACK);//x,y,w,h,color
                confirm_status=1;
                next_status=1;
            }
            break;
        //case 11-19    
        default:
            tft_fillRect(0, 60, 320, 140, ILI9340_BLACK);//x,y,w,h,color
            next_status = 10;

        }
        
    }
    
    if(next_status>=20 && next_status<30){
        
        if(confirm_status>3){
            confirm_status=3;
        }
        //clear screen
        tft_fillRect(2, 60, 315, 140, ILI9340_BLACK);//x,y,w,h,color
        //build sub-menu
        drawmeanu2_1();
        //build "Back"
        tft_setCursor(270, 180);
        tft_setTextColor(ILI9340_WHITE); tft_setTextSize(2);
        sprintf(menubuffer,"Back");
        tft_writeString(menubuffer);
        //build cursor
        switch (next_status){
        case 20:
            tft_fillCircle(250, 190,3,ILI9340_CYAN);//x,y,r,color
            if(confirm_status==3){
                tft_fillRect(0, 60, 320, 140, ILI9340_BLACK);//x,y,w,h,color
                confirm_status=1;
                next_status=1;
            }
            break;
        //case 21-29    
        default:
            tft_fillRect(0, 60, 320, 140, ILI9340_BLACK);//x,y,w,h,color
            next_status = 20;
        }       
    }
    
    if(next_status>=30 && next_status<40){
        
        if(confirm_status>3){
            confirm_status=3;
        }
        //build sub-menu
        drawmeanu3_1_navigation();
        //build "Back"
        tft_setCursor(270, 180);
        tft_setTextColor(ILI9340_WHITE); tft_setTextSize(2);
        sprintf(menubuffer,"Back");
        tft_writeString(menubuffer);
        //build cursor
        switch (next_status){

        case 30:
            tft_fillCircle(250, 190,3,ILI9340_CYAN);//x,y,r,color
            if(confirm_status==3){
                tft_fillRect(0, 60, 320, 140, ILI9340_BLACK);//x,y,w,h,color
                confirm_status=1;
                next_status=1;
            }
            break;
        case 31:
            tft_fillCircle(250, 190,3,ILI9340_BLACK);//x,y,r,color
            tft_fillRect(250, 190, 20, 60, ILI9340_BLACK);//x,y,w,h,color
            next_status = 30;
            tft_fillCircle(250, 190,3,ILI9340_CYAN);//x,y,r,color
            break;            
        //case 32-39    
        default:
            tft_fillRect(0, 60, 320, 140, ILI9340_BLACK);//x,y,w,h,color
            next_status = 30;
        }
        
    }
    if(next_status>=40 && next_status<50){
        
        if(confirm_status>3){
            confirm_status=3;
        }
        //clear screen
        tft_fillRect(2, 60, 315, 140, ILI9340_BLACK);//x,y,w,h,color
        //build sub-menu
        drawmeanu4_1();
        //build "Back"
        tft_fillRect(270, 180, 45, 20, ILI9340_BLACK);//x,y,w,h,color
        tft_setCursor(270, 180);
        tft_setTextColor(ILI9340_WHITE); tft_setTextSize(2);
        sprintf(menubuffer,"Back");
        tft_writeString(menubuffer);
        //build cursor
        switch (next_status){
        case 40:
            tft_fillCircle(250, 190,3,ILI9340_CYAN);//x,y,r,color
            if(confirm_status==3){
                tft_fillRect(0, 60, 320, 140, ILI9340_BLACK);//x,y,w,h,color
                confirm_status=1;
                next_status=1;
            }
            break;
        //case 41-49    
        default:
            tft_fillRect(0, 60, 320, 140, ILI9340_BLACK);//x,y,w,h,color
            next_status = 10;

        }
        
    }


    

}


void drawmeanu1_1(void){
    
    tft_setCursor(2, 60);
    tft_setTextColor(ILI9340_YELLOW); tft_setTextSize(2);
    if(tempf == 1000){
        sprintf(menubuffer,"No Temp Sensor");
    }
    else{
        sprintf(menubuffer,"Temp:%.1f(C) %.1f(F)",tempf,32.0 + tempf*1.8);
    }
    tft_writeString(menubuffer);
}

void drawmeanu2_1(void){
            if(GPS_quality==0){
                tft_setCursor(20, 60);
                tft_setTextColor(ILI9340_RED); tft_setTextSize(3);
                sprintf(menubuffer,"No GPS Signal...");
                tft_writeString(menubuffer);
            }
            else{
                tft_fillRect(20, 60, 295, 20, ILI9340_BLACK);//x,y,w,h,color
                tft_setCursor(20, 60);
                tft_setTextColor(ILI9340_BLUE); tft_setTextSize(2);
                sprintf(menubuffer,"EST: %d:%d:%d",GPS_time_h,GPS_time_m,GPS_time_s);
                tft_writeString(menubuffer);

                tft_fillRect(20, 80, 295, 20, ILI9340_BLACK);//x,y,w,h,color
                tft_setCursor(20, 80);
                tft_setTextColor(ILI9340_GREEN); tft_setTextSize(2);
                sprintf(menubuffer,"Lat:%.6f,%c",GPS_curLat,GPS_NS);
                tft_writeString(menubuffer);


                tft_fillRect(20, 100, 295, 20, ILI9340_BLACK);//x,y,w,h,color
                tft_setCursor(20, 100);
                tft_setTextColor(ILI9340_GREEN); tft_setTextSize(2);
                sprintf(menubuffer,"Lon:%.6f,%c",GPS_curLon,GPS_WE);
                tft_writeString(menubuffer);
            }

            if (GPS_region!=NoGPSMapData){
                tft_fillRect(2, 160, 315, 20, ILI9340_BLACK);//x,y,w,h,color
                tft_setCursor(2, 160);
                tft_setTextColor(ILI9340_CYAN); tft_setTextSize(2);
                tft_writeString(GPS_region_str);
            }
            
            
            else{
                tft_fillRect(2, 160, 315, 20, ILI9340_BLACK);//x,y,w,h,color
                tft_setCursor(2, 160);
                tft_setTextColor(ILI9340_CYAN); tft_setTextSize(2);
                sprintf(menubuffer,"NoGPSMapData");
                tft_writeString(menubuffer);
            }
}


void drawmeanu3_1_navigation(void){
    if(Nav_route_size==1){
        tft_fillRect(2, 60, 315, 20, ILI9340_BLACK);//x,y,w,h,color
        tft_setCursor(2, 60);
        tft_setTextColor(ILI9340_WHITE); tft_setTextSize(2);
        sprintf(menubuffer,"You Are Right Here");
        tft_writeString(menubuffer);
    }
    
    if(waypoint_count==1 && navigation_arrival==0 && Nav_route_size!=1){
        tft_fillRect(2, 60, 315, 20, ILI9340_BLACK);//x,y,w,h,color
        tft_setCursor(2, 60);
        tft_setTextColor(ILI9340_WHITE); tft_setTextSize(2);
        sprintf(menubuffer,"Navigation Start");
        tft_writeString(menubuffer);
    }
    if(cls==1){
        
        if(navigation_arrival==1){
            tft_fillRect(2, 60, 315, 40, ILI9340_BLACK);
            tft_setCursor(0, 80);
            tft_setTextColor(ILI9340_WHITE); tft_setTextSize(2);
            sprintf(menubuffer,"Arrived");
            tft_writeString(menubuffer);
        }
        
        else{
            tft_setCursor(2, 80);
            tft_setTextColor(ILI9340_WHITE); tft_setTextSize(2);
            tft_fillRect(2, 60, 315, 40, ILI9340_BLACK);//x,y,w,h,color
            if(GPS_lr==GPS_Left){
                sprintf(menubuffer,"turn left %.2f deg",GPS_deg);
                tft_writeString(menubuffer);
            }
            else{
                sprintf(menubuffer,"turn right %.2f deg",GPS_deg);
                tft_writeString(menubuffer);
            }
        }
    }
    else{
        if(waypoint_count!=1){
            tft_fillRect(2, 60, 315, 40, ILI9340_BLACK);//x,y,w,h,color
            tft_setCursor(2, 80);
            tft_setTextColor(ILI9340_WHITE); tft_setTextSize(2);
            sprintf(menubuffer,"Keep Going");
            tft_writeString(menubuffer);
        }
    }
    
    tft_fillRect(2, 100, 315, 20, ILI9340_BLACK);//x,y,w,h,color
    tft_setCursor(2, 100);
    tft_setTextColor(ILI9340_GREEN); tft_setTextSize(2);
    sprintf(menubuffer,"Lat:%.6f,%c",GPS_curLat,GPS_NS);
    tft_writeString(menubuffer);


    tft_fillRect(2, 120, 315, 20, ILI9340_BLACK);//x,y,w,h,color
    tft_setCursor(2, 120);
    tft_setTextColor(ILI9340_GREEN); tft_setTextSize(2);
    sprintf(menubuffer,"Lon:%.6f,%c",GPS_curLon,GPS_WE);
    tft_writeString(menubuffer);

    tft_fillRect(2, 140, 315, 20, ILI9340_BLACK);//x,y,w,h,color
    tft_setCursor(2, 140);
    tft_setTextColor(ILI9340_GREEN); tft_setTextSize(2);
    if(Nav_route_lat!=0 && Nav_route_size!=1){
        sprintf(menubuffer,"WayLat:%.6f,wc=%d",
            Nav_route_lat[waypoint_count],waypoint_count);
    }
    tft_writeString(menubuffer);

    tft_fillRect(2, 160, 315, 20, ILI9340_BLACK);//x,y,w,h,color
    tft_setCursor(2, 160);
    tft_setTextColor(ILI9340_GREEN); tft_setTextSize(2);
    if(Nav_route_lon!=0 && Nav_route_size!=1){
        sprintf(menubuffer,"WayLon:%.6f",-Nav_route_lon[waypoint_count]);
    }
    tft_writeString(menubuffer);        
}

void drawmeanu4_1(void){
    
    tft_setCursor(2, 60);
    tft_setTextColor(ILI9340_YELLOW); tft_setTextSize(2);
    sprintf(menubuffer,"WIFI Settings");
    tft_writeString(menubuffer);

    tft_setCursor(2, 80);
    tft_setTextColor(ILI9340_YELLOW); tft_setTextSize(2);
    sprintf(menubuffer,IP_addr_string);
    tft_writeString(menubuffer);
    
    tft_fillRect(0, 100, 320, 20, ILI9340_BLACK);//x,y,w,h,color
    tft_setCursor(2, 100);
    tft_setTextColor(ILI9340_YELLOW); tft_setTextSize(2);
    sprintf(menubuffer,wifi_buffer);
    tft_writeString(menubuffer);

    tft_fillRect(0, 120, 320, 20, ILI9340_BLACK);//x,y,w,h,color
    tft_setCursor(2, 120);
    tft_setTextColor(ILI9340_YELLOW); tft_setTextSize(2);
    sprintf(menubuffer,"route_size = %d",Nav_route_size_temp);
    tft_writeString(menubuffer);

    if(Nav_route_lat_temp == NULL){
        tft_fillRect(0, 140, 320, 20, ILI9340_BLACK);//x,y,w,h,color
        tft_setCursor(2, 140);
        tft_setTextColor(ILI9340_YELLOW); tft_setTextSize(2);
        sprintf(menubuffer,"Please assign a route");
        tft_writeString(menubuffer);  
    } else{
        tft_fillRect(0, 140, 320, 20, ILI9340_BLACK);//x,y,w,h,color
        tft_setCursor(2, 140);
        tft_setTextColor(ILI9340_YELLOW); tft_setTextSize(2);
        sprintf(menubuffer,"Route loaded");
        tft_writeString(menubuffer);      
    }
          
}