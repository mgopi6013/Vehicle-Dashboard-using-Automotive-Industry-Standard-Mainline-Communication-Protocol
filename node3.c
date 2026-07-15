#include <lpc21xx.h>
#iinclude "can.c"
#define mode3      0x18
#define master_bit 5
#define CS         7
void spi_init(void);
char spi_data(unsigned char);
float read_adc(unsigned char);
CAN2_MSG tx;
int main()
{
    float voltage;
    unsigned char fuel;
    spi_init();
    can2_init();
    while(1)
    {
        voltage = read_adc(0);
        /* Convert voltage (0-3.3V) to fuel percentage */
        fuel = (unsigned char)((voltage / 330.0) * 100);
        tx.id = 0x101;      // Fuel CAN ID
        tx.dlc = 1;
        tx.rtr = 0;
        tx.byteA = fuel;
        tx.byteB = 0;
        can2_tx(tx);
        delay_ms(1000);
    }
}

void spi_init(void)
{
    PINSEL0 = 0x00001500;
    S0SPCCR = 150;
    S0SPCR = (1<<master_bit) | mode3;
    IODIR0 |= (1<<CS);
    IOSET0 = (1<<CS);
}

float read_adc(unsigned char ch)
{
    unsigned char hbyte, lbyte;
    unsigned int adc;
    IOCLR0 = (1<<CS);
    spi_data(0x06);
    hbyte = spi_data(ch<<6);
    lbyte = spi_data(0x00);
    IOSET0 = (1<<CS);
    adc = ((hbyte & 0x0F) << 8) | lbyte;
    return (adc * 3.3 / 4096.0 * 100.0);
}

char spi_data(unsigned char data)
{
    S0SPDR = data;
    while((S0SPSR & (1<<7)) == 0);
    return S0SPDR;
}