
//includes
#include <p33FJ128GP802.h> //target C for that spec processor
#include <timer.h> //from peripherals, to configure timing 
#include<stdio.h>

//definitions
/*
 LOGIC 1: 0.6us high, 0.6us low
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
 * Logic 0: 0.3 high, 0.9 low
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
  asm ("nop");


#define TMR1_DEFAULT_VALUE 0x13A0 //1ms resolution
#define BAUDRATE 115200
#define FCY       40000000
#define BRGVAL   ((FCY/BAUDRATE)/16)-1
  
//DEFINE PINS
#define LED LATBbits.LATB0  //pin 4 of processor
#define clk_pin LATBbits.LATB8 //pin 17 of processor
#define data_pin LATBbits.LATB9   //pin 18 of processor
  
  
  //for internal clock
_FOSCSEL(FNOSC_PRIPLL);
_FOSC(OSCIOFNC_OFF & POSCMD_XT);
_FWDT(FWDTEN_OFF);
_FICD(JTAGEN_OFF & ICS_PGD2);

//functions
void init_oscillator(void);
void init_timer1(void);
void proc_sleep(unsigned int);
void shiftPix(unsigned int);
void RGB(unsigned char, unsigned char, unsigned char);
void init_i2c(void);
void I2C1_Send(unsigned char);
void I2C1_Enable();
void I2C1_Disable();
void I2C1_Start();
void I2C1_Idle();
void I2C1_Stop();
void I2C1_Restart();
void I2C1_Ack();
void I2C1_NAck();
unsigned char I2C1_Receive();
void serial_isr_rx (void);
void serial_sendbyte(unsigned char data);
void init_uart2(void);

//structs
typedef struct structTimer
{
   unsigned long count_tick;
   unsigned int  count_down;
} T_TIMER;
T_TIMER isr_timer;

typedef struct struc_NEO
{
   unsigned char r;
   unsigned char g;
   unsigned char b;
} T_NEO; 
T_NEO ring[24];






int main(){
    unsigned char b_flag, u_flag,accel_flag; //flag to change colors
    unsigned char byte, testByte; //byte to be received 
    int accelZ, accelX, accelY; //Z acceleration val
    int buf_count;
    unsigned int i,k; //counter for loop
    unsigned char buf[100];
    
    testByte = 0x55;
    
    init_oscillator(); //init the oscillator
    init_timer1(); //init timer1
    init_i2c(); //init i2c protocol
    init_uart2(); //init uart protocol 
    
    TRISBbits.TRISB0 = 0; //pin 4 output
    //ODCBbits.ODCB8 = 1; //open drain
   // ODCBbits.ODCB9 = 1; //open drain
    
    for(i = 0; i < 48; i++){ //turn off all LEDs twice, just to be sure
        RGB(0x00,0x00,0x00); // turn off LEDs
    }
    
    proc_sleep(3);
    
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
    
    b_flag = 0;
    
//    while(1){
//        LED = 1;
//        proc_sleep(100);
//        LED = 0;
//        proc_sleep(100);
//    }
    
//    while(1){
//        RGB(0xAA,0xAA,0xAA);
//    }
    
    while(1){
        
        //GET X
        I2C1_Enable();
        I2C1_Idle();
        I2C1_Start();
        I2C1_Send(0x32);
        I2C1_Idle();
        I2C1_Send(0x29);
        I2C1_Idle();
        I2C1_Restart();
        I2C1_Send(0x33);
        I2C1_Idle();
        byte = I2C1_Receive();
        I2C1_NAck();
        I2C1_Stop();
        
        accelX = byte;
        accelX = (accelX << 8);
        
        I2C1_Enable();
        I2C1_Idle();
        I2C1_Start();
        I2C1_Send(0x32);
        I2C1_Idle();
        I2C1_Send(0x28);
        I2C1_Idle();
        I2C1_Restart();
        I2C1_Send(0x33);
        I2C1_Idle();
        byte = I2C1_Receive();
        I2C1_NAck();
        I2C1_Stop();
        
        accelX = accelX | byte;
        accelX = (accelX >> 4); //12 bit value
        
        //GET Y
        I2C1_Enable();
        I2C1_Idle();
        I2C1_Start();
        I2C1_Send(0x32);
        I2C1_Idle();
        I2C1_Send(0x2B);
        I2C1_Idle();
        I2C1_Restart();
        I2C1_Send(0x33);
        I2C1_Idle();
        byte = I2C1_Receive();
        I2C1_NAck();
        I2C1_Stop();
        
        accelY = byte;
        accelY = (accelY << 8);
        
        I2C1_Enable();
        I2C1_Idle();
        I2C1_Start();
        I2C1_Send(0x32);
        I2C1_Idle();
        I2C1_Send(0x2A);
        I2C1_Idle();
        I2C1_Restart();
        I2C1_Send(0x33);
        I2C1_Idle();
        byte = I2C1_Receive();
        I2C1_NAck();
        I2C1_Stop();
        
        accelY = accelY | byte;
        accelY = (accelY >> 4); //12 bit value
        
        //GET Z
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
        I2C1_NAck();
        I2C1_Stop();
        
        accelZ = byte;
        accelZ = (accelZ << 8);
        
        I2C1_Enable();
        I2C1_Idle();
        I2C1_Start();
        I2C1_Send(0x32);
        I2C1_Idle();
        I2C1_Send(0x2C);
        I2C1_Idle();
        I2C1_Restart();
        I2C1_Send(0x33);
        I2C1_Idle();
        byte = I2C1_Receive();
        I2C1_NAck();
        I2C1_Stop();
        
        accelZ = accelZ | byte;
        accelZ = (accelZ >> 4); //12 bit value
        
        sprintf(buf,"%d\t%d\t%d\n\0",accelX,accelY,accelZ);
        u_flag = 1;
        buf_count = 0;
        while(u_flag){
            
            if(buf[buf_count] != 0){
                serial_sendbyte(buf[buf_count]);
                buf_count ++;
            }
            else{
                u_flag = 0;
            }
        }
        if((accelX>=-175 && accelX<=175)&&(accelY<=-990 && accelY>=-1100)&&(accelZ>=-250 && accelZ<=250))
        
        {
            accel_flag = 1;
        }
        else if((accelZ>=-250 && accelZ<=250)&&(accelY>=990 && accelY<=1100)&&(accelX>=-175 && accelX<=175))
        {
            accel_flag = 2;
        }
        else if((accelX<=-990 && accelX>=-1110)&&(accelY>=-175 && accelY<=175)&&(accelZ>=-250 && accelZ<=250))
        {
            accel_flag = 3;
        }
        else if((accelX>=990 && accelX<=1110)&&(accelY>=-175 && accelY<=175)&&(accelZ>=-250 && accelZ<=250))
        {
            accel_flag = 4;
        }
        else{accel_flag =0;}
         
        if(b_flag == 0)
        {
            ring[0].r = 0x00;
            ring[0].g = 0x00;
            ring[0].b = 0x00;
        }
        
        if(accel_flag == 1)
        {
            ring[0].r = 0x00;
            ring[0].g = 0xF0;
            ring[0].b = 0x00;
        }
        
        if(accel_flag == 2)
        {
            ring[0].r = 0x00;
            ring[0].g = 0x00;
            ring[0].b = 0xF0;
        }
        
        if(accel_flag == 3)
        {
            ring[0].r = 0xF0;
            ring[0].g = 0x00;
            ring[0].b = 0x00;
        }
        
        if(accel_flag == 4)
        {
            ring[0].r = 0xF0;
            ring[0].g = 0x00;
            ring[0].b = 0xF0;
        }
        
        
        //display the led colors
        for(k=0; k<24; k++){
            RGB(ring[k].r,ring[k].g,ring[k].b);
        }
        
        //shift the led values
        for(i = 0; i<23; i++){
            ring[23-i] = ring[22-i];
        }
        
//        if(b_flag == 0){
//            b_flag = 1;
//        }
//        else{b_flag = 0;}
        
        proc_sleep(80);
    }
    
    return 0;
}



//internal clock init
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

//timer init
void init_timer1(void)
{
   OpenTimer1(T1_ON            &
              T1_GATE_OFF &
              T1_PS_1_8 &
              T1_SOURCE_INT,
              TMR1_DEFAULT_VALUE); //holds the period value

   ConfigIntTimer1(T1_INT_PRIOR_1 & T1_INT_ON); //for interrupts

}

//interrupt for the 1ms resolution, read timer h file for better understanding
void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
   isr_timer.count_tick = isr_timer.count_tick + 1;

   if (isr_timer.count_down) {
       isr_timer.count_down--;
   }

    IFS0bits.T1IF = 0;
}

//to create a delay of blink ms
void proc_sleep(unsigned int blink) {
   isr_timer.count_down = blink;
   while (isr_timer.count_down) {
   }
}

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

//i2c init
void init_i2c(){

    I2C1CONbits.I2CEN = 0x00; //disable i2c module
    I2C1CONbits.I2CSIDL = 0x00; //continues in idle mode
    I2C1CONbits.IPMIEN = 0x00; //IPMI disabled 
    I2C1CONbits.A10M = 0x00; // 7-bit address
    I2C1CONbits.DISSLW = 0x01; // disable slew rate
    I2C1CONbits.SMEN = 0x00; //disable SMBus inputs
    I2C1BRG = 87; //master baud rate
    proc_sleep(1); //1ms for configurating 
    //I2C1CONbits.I2CEN = 0x01; //enable i2c module   
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

void serial_isr_rx (void)
{
   unsigned char rxdata;

   while(U2STAbits.URXDA)          /* Read all the data remaining in receive buffer which are unread */
   {
       rxdata = (unsigned char)(U2RXREG);  

       //rxd.buffer[rxd.push] = (unsigned char)(U2RXREG);
       //rxd.push = rxd.push + 1;
       //if ( rxd.push >= UART_BUFFERSIZE )
       //        rxd.push = 0;
   }
}

void serial_sendbyte(unsigned char data)
{
   while(!U2STAbits.TRMT)  /* wait until empty */
   {
   }

   U2TXREG = data;

}

void init_uart2(void)
{
   //--------Pin 7 : Bluetooth Reset, Active Low--------//
   //LATBbits.LATB3 = 0;
   //TRISBbits.TRISB3 = 1;



   //--------Pin 18 : Bluetooth Reset, Active Low--------//
   //LATBbits.LATB9 = 1;
   //TRISBbits.TRISB9 = 0;

   //--------Pin 17 : Bluetooth DTR, Active High--------//
   //LATBbits.LATB8 = 0;
   //TRISBbits.TRISB8 = 0;

        //--------Pin 25: UART Rx--------//
   PORTBbits.RB14 = 1;    
   TRISBbits.TRISB14 = 1; // UART2 RX set pin as input
    
    //--------Pin 26: UART Tx--------//
   LATBbits.LATB15 = 1;
   TRISBbits.TRISB15 = 0;   

   //RPOR6bits.RP13R   = 0x00;       // UART2 CTS on RP13
   //RPOR6bits.RP12R   = 0x00;       // UART2 RTS on RP12

   RPOR7bits.RP15R   = 0x05;       // UART2 TX on RP15
   RPINR19bits.U2RXR = 14;         // UART2 RX on RP14

   TRISBbits.TRISB14 = 1;          // set pin as input
   AD1PCFGL = 0xFFFF;              // Disable Analog

   // configure U2MODE
   U2MODEbits.UARTEN = 0;  // Bit15 TX, RX DISABLED, ENABLE at end of func
   //U2MODEbits.notimplemented;    // Bit14
   U2MODEbits.USIDL = 0;   // Bit13 Continue in Idle
   U2MODEbits.IREN = 0;    // Bit12 No IR translation
   U2MODEbits.RTSMD = 0;   // Bit11 Simplex Mode
   //U2MODEbits.notimplemented;    // Bit10
   U2MODEbits.UEN = 0;     // Bits8,9 TX,RX enabled, CTS,RTS not
   U2MODEbits.WAKE = 0;    // Bit7 No Wake up (since we don't sleep here)
   U2MODEbits.LPBACK = 0;  // Bit6 No Loop Back
   U2MODEbits.ABAUD = 0;   // Bit5 No Autobaud (would require sending '55')
   U2MODEbits.URXINV = 0;  // Bit4 IdleState = 1  (for dsPIC)
   U2MODEbits.BRGH = 0;    // Bit3 16 clocks per bit period
   U2MODEbits.PDSEL = 0;   // Bits1,2 8bit, No Parity
   U2MODEbits.STSEL = 0;   // Bit0 One Stop Bit

   // Load a value into Baud Rate Generator
   U2BRG = BRGVAL; // 40Mhz osc, 115200 Baud

   // Load all values in for U1STA SFR
   U2STAbits.UTXISEL1 = 0; //Bit15 Int when Char is transferred (1/2 config!)
   U2STAbits.UTXINV = 0;   //Bit14 N/A, IRDA config
   U2STAbits.UTXISEL0 = 0; //Bit13 Other half of Bit15
   //U2STAbits.notimplemented = 0; //Bit12
   U2STAbits.UTXBRK = 0;   //Bit11 Disabled
   U2STAbits.UTXEN = 0;    //Bit10 TX pins controlled by periph
   U2STAbits.UTXBF = 0;    //Bit9 *Read Only Bit*
   U2STAbits.TRMT = 0;     //Bit8 *Read Only bit*
   U2STAbits.URXISEL = 0;  //Bits6,7 Int. on character recieved
   U2STAbits.ADDEN = 0;    //Bit5 Address Detect Disabled
   U2STAbits.RIDLE = 0;    //Bit4 *Read Only Bit*
   U2STAbits.PERR = 0;     //Bit3 *Read Only Bit*
   U2STAbits.FERR = 0;     //Bit2 *Read Only Bit*
   U2STAbits.OERR = 0;     //Bit1 *Read Only Bit*
   U2STAbits.URXDA = 0;    //Bit0 *Read Only Bit*

   IPC7 = 0x4400;// Mid Range Interrupt Priority level, no urgent reason

   IFS1bits.U2TXIF = 0;    // Clear the Transmit Interrupt Flag
   IEC1bits.U2TXIE = 0;    // Enable Transmit Interrupts
   IFS1bits.U2RXIF = 0;    // Clear the Recieve Interrupt Flag
   IEC1bits.U2RXIE = 0;    // Enable Recieve Interrupts
   
   U2MODEbits.UARTEN = 1;  // And turn the peripheral on
   U2STAbits.UTXEN = 1;

   IEC4bits.U2EIE = 0;
}

void __attribute__((interrupt, auto_psv)) _U2RXInterrupt(void)
{
   serial_isr_rx();
   IFS1bits.U2RXIF = 0;
}

void __attribute__((interrupt, auto_psv)) _U2TXInterrupt(void)
{
   IFS1bits.U2TXIF = 0;
}

void __attribute__((interrupt, auto_psv)) _U2ErrInterrupt(void)
{
   IFS4bits.U2EIF = 0; /* Clear the UART2 Error Interrupt Flag */
}