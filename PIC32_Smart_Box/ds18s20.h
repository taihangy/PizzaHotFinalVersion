char Reset_1820(void); //function to reset the sensor
void Write_1820(unsigned char cmd); //function to write 8 bits to sensor
unsigned char Read_1820(void); //function to read 8 bits from sensor

unsigned short tempL,tempH;	//variable to store low 8 bits and high 8 bits of the sensor
float tempf;	//convert the raw data into real temperature in Celsius
