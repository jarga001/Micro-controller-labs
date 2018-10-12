/*
 * File:   final_project_v3.c
 * Author: baska
 *
 * Created on April 28, 2018, 8:42 PM
 */

#include "xc.h"

uint8_t address[5]= {0xE7,0xE7,0xE7,0xE7,0xE7};

void delay_x_amount_10us(int x) { //approximate delay for x micro seconds
    long delay = 10L * x;
    while (delay--);
}

//------------------------------------------------------------
// write a byte to 24L01 and read one byte
uint8_t SPI_RW(uint8_t byte)
{
    uint8_t data_in; 
    SPI1BUF=byte;  // sending a code to transmitter through SPI
    while (_SPI1IF == 0) ;
    _SPI1IF=0;    // reset flag variable
    data_in=SPI1BUF; // data that is received 
    return data_in;
}

// write one byte to register reg and return status byte
uint8_t SPI_RW_Reg(uint8_t reg, uint8_t value)
{
    uint8_t status;
    LATBbits.LATB8=0; // CSN low, init SPI transaction
    status = SPI_RW(reg); // select register
    SPI_RW(value); // ..and write value to it..
    LATBbits.LATB8 = 1; // CSN high again
    return(status); // return nRF24L01 status byte
}

// read data
uint8_t SPI_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t bytes)
{
    uint8_t status,byte_ctr;
    LATBbits.LATB8 = 0; // Set CSN low, init SPI transaction
    status = SPI_RW(reg); // Select register to write to and read status byte
    for(byte_ctr=0; byte_ctr<bytes; byte_ctr++){
        pBuf[byte_ctr] = SPI_RW(NOP); //send NOP values to read values
        LATBbits.LATB8 = 1;
    }
return(status); // return nRF24L01 status byte       
} 

// write data
uint8_t SPI_Write_Buf(uint8_t reg, uint8_t  *pBuf, uint8_t bytes)
{
    uint8_t status, byte_ctr; 
    LATBbits.LATB8 = 0;   
    status = SPI_RW(reg); // get status bit
    for(byte_ctr=0; byte_ctr<bytes; byte_ctr++){ 
        SPI_RW(pBuf[byte_ctr]); //send buffer values to buffer of radio transmitter 
    }
    LATBbits.LATB8 = 1; // Set CSN high again
return(status); 
}

// receiving function, return 1 while receiving is done
uint8_t nRF24L01_RxPacket(uint8_t *rx_buf)
{
    uint8_t revale=0;
    uint8_t sta;
// set in RX mode
    SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f); // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..
    LATBbits.LATB5 = 1; // Set CE pin high to enable RX device
    delay_x_amount_10us(13);
    sta=SPI_RW_Reg(STATUS,0x0e); // read register STATUS's value
    if((sta&=0b000000100)!=0) // if receive data ready 
    {
        LATBbits.LATB5 = 0; // stand by mode
        SPI_Read_Buf(RD_RX_PLOAD,rx_buf,1);// read receive payload from RX_FIFO buffer
        revale =1;
    }
    SPI_RW_Reg(WRITE_REG+STATUS,0x0e);// clear RX_DR or TX_DS or MAX_RT interrupt flag
    return revale;
}

// sending function
void nRF24L01_TxPacket(uint8_t *tx_buf)
{
    LATBbits.LATB5=0;
    SPI_Write_Buf(WR_TX_PLOAD,tx_buf, 1); // Writes data to TX payload
    SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e); // Set PWR_UP bit, enable CRC(2 bytes) & Prim:TX. MAX_RT & TX_DS enabled..
    LATBbits.LATB5=1; // give pulse to send a data over radio transmitter
    delay_x_amount_10us(1);
    LATBbits.LATB5=0;
}

// setting function
void nRF24L01_Config(int x)
{

LATBbits.LATB8=0; // enable slave select

    LATBbits.LATB5=0;
    if(x==0){
        SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f); // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..
    }
    else{
        SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e); // Set PWR_UP bit, enable CRC(2 bytes) & Prim:RX. RX_DR enabled..
    }
    SPI_RW_Reg(WRITE_REG + EN_AA, 0x01); // enable auto acknowledgement
    SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01); // Enable Pipe0
    SPI_RW_Reg(WRITE_REG + SETUP_AW, 0x03); // Setup address width=5 bytes
    SPI_RW_Reg(WRITE_REG + SETUP_RETR, 0x1a); // 500us + 86us, 10 retrans...
    SPI_RW_Reg(WRITE_REG + RF_CH, 0);  // set frequency as 2.4GHz
    SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x27); // TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR
    SPI_RW_Reg(WRITE_REG + RX_PW_P0, 1); //enable pipe 0
    SPI_Write_Buf(WRITE_REG + TX_ADDR, address, 5); //set pipe address for transmitter
    SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, address, 5); // set  pipe address for receiver
   
    LATBbits.LATB5=1; // disable slave select 
}

void spi_init(){
    SPI1CON1=0;
    IFS0bits.SPI1IF = 0; // Clear the Interrupt flag 
    IEC0bits.SPI1IE = 0; // Disable the interrupt
    // SPI1CON1 Register Settings
    SPI1CON1bits.DISSCK = 0; // Internal serial clock is enabled
    SPI1CON1bits.DISSDO = 0; // SDOx pin is controlled by the module
    SPI1CON1bits.MODE16 = 0; // Communication is word-wide (8 bits)
    SPI1CON1bits.MSTEN = 1;  // Master mode enabled
    SPI1CON1bits.SMP = 0;   // Input data is sampled at the end of data output time
    SPI1CON1bits.CKE = 1;   // Serial output data changes on transition from
                            // Idle clock state to active clock state
    SPI1CON1bits.CKP = 0;   // Idle state for clock is a low level;
                            // active state is a high level
   
    SPI1CON1bits.SPRE=0b111; //secondary clock 1:1
    SPI1CON1bits.PPRE=0b01;   // primary clock 1:16
    SPI1CON2=0;
    SPI1STAT=0;
    SPI1STATbits.SISEL = 0b101; // IF set when last bit is shifted out
                                // That means the SPI xfer is complete.
    SPI1STATbits.SPIEN = 1; // Enable SPI module

    
    // Interrupt Controller Settings
    IFS0bits.SPI1IF = 0;        // Clear the Interrupt flag
    IEC0bits.SPI1IE = 0;        // Disable the interrupt
}

