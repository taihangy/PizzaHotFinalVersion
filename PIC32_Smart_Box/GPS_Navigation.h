#define GPS_Left 1
#define GPS_Right 0

int GPS_lr; //the variable to judge turn left or turn right
float GPS_deg; //the variable records the degree of GPS
static char cls;	//the variable to decide whether close to waypoint or not


float* Nav_route_lon;	//the pointer to find the road, longitude (stored as arrays in route.h)
float* Nav_route_lat;	//the pointer to find the road, latitude (stored as arrays in route.h)
int Nav_route_size=1;	//the variable to store the size of the route (number of way points in an array)
int Nav_route_size_temp=1;
float* Nav_route_lon_temp = NULL;   //the pointer to store parsed data from wifi
float* Nav_route_lat_temp = NULL;   //the pointer to store parsed data from wifi
int route_scheduling_counter = 0;

int get_leftright(float x1,float y1, float x2, float y2, float x3, float y3);	//the function to decide turn left or right, cross product
float get_angle(float x1,float y1, float x2, float y2, float x3, float y3);		//the function to calculate turning degree, dot product
char waypoint_close(float GPS_curLon,float GPS_curLat,float waypoint_lon,float waypoint_lat);	//the function to decide whether close to a waypoint
float* get_route_lon(void);	//the function to find the road, longitude (stored as arrays in route.h)
float* get_route_lat(void);	//the function to find the road, latitude (stored as arrays in route.h)
int get_route_size(void);	//the function to find the size of the route (number of way points in an array)

//the function to decide turn left or right, cross product
int get_leftright(float x1,float y1, float x2, float y2, float x3, float y3){
    float ax,ay,bx,by,c;
    ax = (x2 - x1);
    ay = y2 - y1;
    bx = (x3 - x2);
    by = y3 - y2;
    c = ax*by - ay*bx;
    if (c > 0) return 1; 
    else return 0;
}

//the function to calculate turning degree, dot product
float get_angle(float lon1,float lat1, float lon2, float lat2, float lon3, float lat3){
    float ax,ay,bx,by,abs_a,abs_b;
    float cos;
    float angle;
    float deg;
    ay = (lat2 - lat1);
    ax = lon2 - lon1;
    by = (lat3 - lat2);
    bx = lon3 - lon2;
    abs_a = sqrt(ax*ax + ay*ay);
    abs_b = sqrt(bx*bx + by*by);
    cos = (ax*bx + ay*by)/(abs_a*abs_b);
    angle = acosf(cos);
    deg = angle*180.0/3.14159;
    return deg;    
}

//the function to decide whether close to a waypoint
char waypoint_close(float GPS_curLon,float GPS_curLat,float waypoint_lon,float waypoint_lat){
    //Distance: 10m
    //No abs!! ABS returns integer!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if((GPS_curLon-waypoint_lon)<0.000045 &&
            (GPS_curLon-waypoint_lon)>-0.000045 &&            
            (GPS_curLat-waypoint_lat)<0.000061 &&
            (GPS_curLat-waypoint_lat)>-0.000061)
        return 1;
    else
        return 0;
}

//the function to find the road, longitude (stored as arrays in route.h)
float* get_route_lon(void){
    return Nav_route_lon_temp;
}

//the function to find the road, latitude (stored as arrays in route.h)
float* get_route_lat(void){
    return Nav_route_lat_temp;    
}

//the function to find the size of the route (number of way points in an array)
int get_route_size(void){
    return Nav_route_size_temp;
}