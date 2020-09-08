#include <p33FJ128GP802.h> //target C for that spec processor
#include <timer.h> //from peripherals, to configure timing 

#define TMR1_DEFAULT_VALUE 0x13A0 //1ms resolution, ask yerko

// 1.25usec @ 12MHz  0.8us High, .425us Low
//#define LOGIC1 \
//  asm ("bset LATB,#0"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \  
//  asm ("nop"); \
//  asm ("bclr LATB,#0"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); 
//  
//  
//// 1.25usec @ 12MHz   0.4us High, .825us Low
//#define LOGIC0 \
//  asm ("bset LATB,#0"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("bclr LATB,#0"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \
//  asm ("nop"); \  
//  asm ("nop");

/*
 1.25 us @ 40MHz  0.6 us High, 0.6 us Low
 */
#define LOGIC1 \
  asm ("bset LATB,#0"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("bclr LATB,#0"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); 
  
/*
1.25us @ 12MHz   0.3us High, 0.9us Low
 */
#define LOGIC0 \
  asm ("bset LATB,#0"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("bclr LATB,#0"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \
  asm ("nop"); \ 
  asm ("nop");
/*
 //ask yerko
//needed to change
 */ 
_FOSCSEL(FNOSC_PRIPLL);
_FOSC(OSCIOFNC_OFF & POSCMD_XT);
_FWDT(FWDTEN_OFF);
_FICD(JTAGEN_OFF & ICS_PGD2);               /* Watchdog is off */


/*
research structures*
 */
typedef struct structTimer
{
   unsigned long count_tick;
   unsigned int  count_down;
//   unsigned int  serialcheck;
//   unsigned char count_down_cantx;
//   unsigned char count_down_canrx;
//   unsigned char count_down_serial;
//   unsigned int  count_down_sleep;
//   unsigned int  count_down_autoconnect;
//   unsigned int  count_down_mainperiod;
//   //unsigned int  injstoreflag;
//   unsigned char canflag;
//   unsigned char reportlivedataflag;
//   unsigned long injcounter;
//   unsigned int  count_down_to_on;

} T_TIMER;


T_TIMER isr_timer; //ask yerko

//to create a 1ms pulse
void init_timer1(void)
{
   OpenTimer1(T1_ON            &
              T1_GATE_OFF &
              T1_PS_1_8 &
              T1_SOURCE_INT,
              TMR1_DEFAULT_VALUE); //holds the period value

   ConfigIntTimer1(T1_INT_PRIOR_1 & T1_INT_ON); //for interrupts

}

//initialize PLL and increase clock to 32 MHz
void init_oscillator(void)
{
    PLLFBD=38; // M=40
    CLKDIVbits.PLLPRE=0; // N2=2
    CLKDIVbits.PLLPOST=0; // N1=2
    OSCTUN=0; // Tune FRC oscillator, if FRC is used
    RCONbits.SWDTEN=0; // Disable Watch Dog Timer
    while(OSCCONbits.LOCK!=1); // Wait for PLL to lock
   
    IFS0=0;
    IFS1=0;
    IFS2=0;
    IFS3=0;
    IFS4=0;    
   
}

//interrupt for the 1ms resolution, read timer h file for better understanding
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
   isr_timer.count_tick = isr_timer.count_tick + 1;

   if (isr_timer.count_down) {
       isr_timer.count_down--;
   }

    IFS0bits.T1IF = 0;
}

//DEFINE PINS
#define LED LATBbits.LATB0  //pin 4 of processor
#define clk_pin LATBbits.LATB8
#define data_pin LATBbits.LATB9

//to make the LEDs bright naturally to the eye
const unsigned char gamma8[] = {
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
 1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
 2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
 5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
 25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
 37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
 51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
 69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
 90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
 115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
 144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
 177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
 215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

//make LEDs light in some other form
const unsigned char sineTable[256] = {
 128,131,134,137,140,143,146,149,152,155,158,162,165,167,170,173,
 176,179,182,185,188,190,193,196,198,201,203,206,208,211,213,215,
 218,220,222,224,226,228,230,232,234,235,237,238,240,241,243,244,
 245,246,248,249,250,250,251,252,253,253,254,254,254,255,255,255,
 255,255,255,255,254,254,254,253,253,252,251,250,250,249,248,246,
 245,244,243,241,240,238,237,235,234,232,230,228,226,224,222,220,
 218,215,213,211,208,206,203,201,198,196,193,190,188,185,182,179,
 176,173,170,167,165,162,158,155,152,149,146,143,140,137,134,131,
 128,124,121,118,115,112,109,106,103,100, 97, 93, 90, 88, 85, 82,
  79, 76, 73, 70, 67, 65, 62, 59, 57, 54, 52, 49, 47, 44, 42, 40,
  37, 35, 33, 31, 29, 27, 25, 23, 21, 20, 18, 17, 15, 14, 12, 11,
  10,  9,  7,  6,  5,  5,  4,  3,  2,  2,  1,  1,  1,  0,  0,  0,
   0,  0,  0,  0,  1,  1,  1,  2,  2,  3,  4,  5,  5,  6,  7,  9,
  10, 11, 12, 14, 15, 17, 18, 20, 21, 23, 25, 27, 29, 31, 33, 35,
  37, 40, 42, 44, 47, 49, 52, 54, 57, 59, 62, 65, 67, 70, 73, 76,
  79, 82, 85, 88, 90, 93, 97,100,103,106,109,112,115,118,121,124};

//to create a delay of blink ms
void proc_sleep(unsigned int blink) {
   isr_timer.count_down = blink;
   while (isr_timer.count_down) {
   }
}

//to shift the values in the RGB LED ring
void shiftPix(unsigned int val){

    unsigned int msbit;
    unsigned char ii;
    unsigned int mask;
    mask = 0x80;
    
   for (ii=0;ii<8;ii++)
   {
       msbit = (val & mask);
       
       if (msbit)
       {
           LOGIC1;
       }
       else
       {
           LOGIC0;
       }        
       mask = (mask >>1);
   }
}

//sets the RGB color on the ring
void RGB(unsigned char red, unsigned char green, unsigned char blue){
    
    shiftPix(green); //turn ring into green
    shiftPix(red); //turn ring into red
    shiftPix(blue); //turn ring into blue 
}

/*
 research struct
 */
typedef struct struc_NEO
{
   unsigned char r;
   unsigned char g;
   unsigned char b;
} T_NEO;
T_NEO pixelIn,pixelOut; 
T_NEO ring[24];

void init_i2c(){

    I2C1CONbits.I2CEN = 0x00; //disable i2c module
    I2C1CONbits.I2CSIDL = 0x00; //continues in idle mode
    I2C1CONbits.IPMIEN = 0x00; //IPMI disabled 
    I2C1CONbits.A10M = 0x00; // 7-bit address
    I2C1CONbits.DISSLW = 0x01; // disable slew rate
    I2C1CONbits.SMEN = 0x00; //disable SMBus inputs
    I2C1BRG = 87; //master baud rate
    proc_sleep(1); //1ms for configurating 
    I2C1CONbits.I2CEN = 0x01; //enable i2c module   
}

// Send Data
void I2C1_Send(unsigned char data){
// To send byte out data needs to be copied to Transmit register
IFS1bits.MI2C1IF = 0;
I2C1TRN = data;
while(I2C1STATbits.TBF);
while(IFS1bits.MI2C1IF); //Wait for ninth clock cycle
IFS1bits.MI2C1IF = 0; //Clear interrupt flag
while(I2C1STATbits.ACKSTAT);
}
// Enable I2C Module
void I2C1_Enable(){
I2C1CONbits.I2CEN = 1;
}
// Disable I2C Module
void I2C1_Disable(){
I2C1CONbits.I2CEN = 0;
}
// Check if bus is idle
void I2C1_Idle(){
// Wait till bus is idle P = 1
while((I2C1CON & 0x001F) || I2C1STATbits.TRSTAT);
}
// Start Event
void I2C1_Start(){
// Generate Start Event
I2C1CONbits.SEN = 1;
// wait here till SEN = 0 <- Start event competed
while(I2C1CONbits.SEN);
}
// Stop Event
void I2C1_Stop(){
// Generate Start Event
I2C1CONbits.PEN = 1;
// wait here till SEN = 0 <- Start event competed
while(I2C1CONbits.PEN);
//__delay_us(150);
}
// Restart Event
void I2C1_Restart(){
// Generate Start Event
I2C1CONbits.RSEN = 1;
// wait here till SEN = 0 <- Start event competed
while(I2C1CONbits.RSEN);
}
// Acknowledge Insert
void I2C1_Ack(){
// Generate Start Event
I2C1CONbits.ACKDT = 0;
I2C1CONbits.ACKEN = 1;
while(I2C1CONbits.ACKEN);
}
// NotAcknowledge Insert
void I2C1_NAck(){
// Generate Start Event
I2C1CONbits.ACKDT = 1;
I2C1CONbits.ACKEN = 1;
while(I2C1CONbits.ACKEN);
}

// Receive Data
unsigned char I2C1_Receive(){
char result;
I2C1CONbits.RCEN = 1;
while(I2C1CONbits.RCEN);
return result = I2C1RCV;
}

int main(){
    
    unsigned int i,j,k; //loop counters
    unsigned int val,temp; //hold RGB values
    unsigned char byte;
    
    unsigned char b_flag; //flag to change colors
    
    LED = 0; //setting the pin 4 low, for rest on RGB ring
    //clk_pin = 1;
    //TRISBbits.TRISB6 = 0; //declare pin 15 is output
    TRISBbits.TRISB0 = 0; //declare pin is 4 an output
    TRISBbits.TRISB8 = 0; //declare pin is 17 an output
    TRISBbits.TRISB9 = 1; //declare pin is 18 an output
    
    init_oscillator(); //init the oscillator
    init_timer1(); //init timer1
    init_i2c(); //init i2c protocol
    
    //IEC1bits.MI2C1IE = 1;
    
    ODCBbits.ODCB8 = 1; //open drain
    //ODCBbits.ODCB9 = 1; //open drain
    
    
    
    //just do it once, setting reset on the RGB ring, data sheet
    //proc_sleep(2);
    
//    for(i = 0; i < 12 ; i++){
//            
//            RGB(0x10,0x10,0x10);
//            
//            proc_sleep(5);
//            
//    }
    
    
    proc_sleep(3); //not sure for purpose, comment out?
    for(i = 0; i < 48; i++){ //turn off all LEDs twice, just to be sure
        RGB(0x00,0x00,0x00); // turn off LEDs
    }
    proc_sleep(10); //not sure for purpose, comment out?
    
    // * part of moving LEDs
    /*
    for(j = 0; j<1; j++){ //first 12 LEDs change color
        ring[j].r = 0x00;
        ring[j].g = 0x20;
        ring[j].b = 0x00;
    }
    */
    /*
    for(j = 0; j<12; j++){ //last 12 LEDs change
        ring[j+12].r = 0x55;
        ring[j+12].g = 0x20;
        ring[j+12].b = 0x2f;
    }
    */
    /*
    RGB(0xFF,0x00,0x00);
    RGB(0xFF,0x00,0x00);
    RGB(0xFF,0x00,0x00);
    RGB(0x00,0xFF,0x00);
    RGB(0x00,0xFF,0x00);
    RGB(0x00,0xFF,0x00);
    RGB(0x00,0x00,0xFF);
    RGB(0x00,0x00,0xFF);
    RGB(0x00,0x00,0xFF);
    RGB(0xFF,0xFF,0xFF);
    RGB(0xFF,0xFF,0xFF);
    RGB(0xFF,0xFF,0xFF);
    
    RGB(0xFF,0x00,0x00);
    RGB(0xFF,0x00,0x00);
    RGB(0xFF,0x00,0x00);
    RGB(0x00,0xFF,0x00);
    RGB(0x00,0xFF,0x00);
    RGB(0x00,0xFF,0x00);
    RGB(0x00,0x00,0xFF);
    RGB(0x00,0x00,0xFF);
    RGB(0x00,0x00,0xFF);
    RGB(0xFF,0xFF,0xFF);
    RGB(0xFF,0xFF,0xFF);
    RGB(0xFF,0xFF,0xFF);
    */
    
    I2C1_Enable();
    I2C1_Idle();
    I2C1_Start();
    I2C1_Send(0x32); //address of the chip
    I2C1_Idle();
    I2C1_Send(0x20); //address of the register inside the chip
    I2C1_Idle();
    I2C1_Send(0x47); //set the register value, to generate data at 50Hz
    I2C1_Idle();
    I2C1_Stop();
    
    val = 0; //counter to change colors
    b_flag = 0;
    while(1){
        /*
        clk_pin = 1;
        //proc_sleep(10);
        for(i=0; i<10; i++);
        clk_pin = 0;
        //proc_sleep(10);
        for(i=0; i<10; i++);
        */
        

        
       // for(i=0;i<20;i++);
        
        I2C1_Enable();
        I2C1_Idle();
        I2C1_Start();
        I2C1_Send(0x32);
        I2C1_Idle();
        I2C1_Send(0x2D);
        I2C1_Idle();
        I2C1_Restart();
        I2C1_Send(0x33);
        I2C1_Idle();
        
        byte = I2C1_Receive();
        //I2C1_Idle();
        I2C1_NAck();
        I2C1_Stop();
        
        
        proc_sleep(2);
    
        /*
        IFS1bits.MI2C1IF = 0;
        I2C1CONbits.SEN = 0x01;
        //proc_sleep(50);
        while (!IFS1bits.MI2C1IF);
        IFS1bits.MI2C1IF = 0;
        I2C1TRN = 0x32; //0x32
        
        while (!IFS1bits.MI2C1IF);
        IFS1bits.MI2C1IF = 0;
        I2C1CONbits.PEN = 0x01;
        //I2C1CONbits.SEN = 0x00;
        proc_sleep(10);
        */
    
    
    
        
    
        //for(j = 0; j<12; j++){ //first 12 LEDs change color
        
        /*
        //set the first led to a color
        if (val < 24){
            ring[0].r = 0x00;
            ring[0].g = 0x20;
            ring[0].b = 0x00;
        }
        
        //set the first led to another color
        if (val < 48 && val >= 24){
            ring[0].r = 0x25;
            ring[0].g = 0x00;
            ring[0].b = 0x25;
        }
        //}
        */
        
        
        if(b_flag == 0){
            ring[0].r = 0x00;
            ring[0].g = 0x00;
            ring[0].b = 0x00;
            
            //b_flag = 1;
        }
        
        if(b_flag == 1){
            ring[0].r = 0x00;
            ring[0].g = 0x00;
            ring[0].b = 0x00;
            
            //b_flag = 0;
        }
        
        if(b_flag == 2){
            ring[0].r = 0x10;
            ring[0].g = 0x00;
            ring[0].b = 0x10;
        }
        
        // * part of moving LEDs
//        pixelOut.r = ring[24].r;
//        pixelOut.g = ring[24].g;
//        pixelOut.b = ring[24].b;
        
        //display the led colors
        for(k=0; k<24; k++){
            RGB(ring[k].r,ring[k].g,ring[k].b);
        }
        
        //shift the led values
        for(i = 0; i<23; i++){
            ring[23-i] = ring[22-i];
        }
        
//        pixelIn.r = pixelOut.r;
//        pixelIn.g = pixelOut.g;
//        pixelIn.b = pixelOut.b;
        
        if(b_flag == 0){
            b_flag = 1;
        }
        else if(b_flag == 1){
            b_flag = 2;
        }
        else{b_flag = 0;}
        
        proc_sleep(80);
        
        /*
        val++;
        
        if (val >= 48){
            val = 0;
        }
        */
        /*
        ring[0].r = 0x25;
        ring[0].g = 0x00;
        ring[0].b = 0x25;
        
        pixelOut.r = ring[24].r;
        pixelOut.g = ring[24].g;
        pixelOut.b = ring[24].b;
        
        for(i = 0; i<23; i++){
            ring[23-i] = ring[22-i];
        }
        
        pixelIn.r = pixelOut.r;
        pixelIn.g = pixelOut.g;
        pixelIn.b = pixelOut.b;
        
        for(k=0; k<24; k++){
        RGB(ring[k].r,ring[k].g,ring[k].b);
            }
        proc_sleep(100);
        */
        /*
        for(j = 0; j < 24 ; j++){

            //RGB(gamma8[val],gamma8[(val+20)&0xff],gamma8[(val+110)&0xff]);     
            RGB(gamma8[val],gamma8[0],gamma8[val]); 
        }
        
        LED = 0; //set pin4 low
        proc_sleep(3); //for RGB ring reset
        val++; //decrement val
        
//        if(val == 120){
//            temp = 120;
//            for(i = 0; i < 12; i++){
//                RGB(gamma8[0],gamma8[0],gamma8[temp]);
//                temp--;
//            }
//        } 
        if(val == 255){
            val = 0;
        }
        */
         
    
//        LED = 1;
//        proc_sleep(1000);
//        
//        LED = 0;
//        proc_sleep(1000);
        
//        LOGIC1;
//        LOGIC1;
//        LOGIC1;
//        LOGIC1;
//        LOGIC1;
//        LOGIC1;
//        LOGIC1;
//        LOGIC1;
//        LOGIC1;
//        LOGIC0;
//        LOGIC1;
//        LOGIC0;
//        LOGIC1;
//        LOGIC0;
//        LOGIC1;
//        LOGIC0;
//        LOGIC1;
//        LOGIC0;
//        LOGIC1;
//        
    }

    return 0;
}


