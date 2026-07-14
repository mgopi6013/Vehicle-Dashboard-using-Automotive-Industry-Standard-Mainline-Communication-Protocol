#include<lpc21xx.h>
#include"lcd.c"
#define mode0 0x00
#define mode1 0x08
#define mode2 0x10
#define mode3 0x18
#define master_bit 5
#define CS 7 
void spi_init(void);
float read_adc(unsigned char);
char spi_data(unsigned char);
int main()
{
 float fl;
 spi_init();
 LCD_INIT();
 
 while(1)
 {
 LCD_COMMAND(0x01);
 LCD_COMMAND(0x80);
LCD_STR("MCP3024-&-LM35");
  fl=read_adc(0);
  LCD_COMMAND(0xc0);
 LCD_STR("temp : ");
  float_display(fl);
  delay_ms(500);
 }
}

void spi_init(void)
{
 PINSEL0=0x00001500;
 S0SPCCR=150;   ///(15000000/(100000*16))
 S0SPCR=(1<<master_bit|mode3);
 IODIR0=1<<CS; /// TO USE THIS GPIO AS CHIP SELECT PIN
 IOSET0=1<<CS;
}

float read_adc(unsigned char ch)
{
 unsigned char hbyte,lbyte;
 unsigned int adcval;
 IOCLR0=1<<CS;
 spi_data(0x06);
 hbyte=spi_data(ch<<6);
 lbyte=spi_data(0x00);
 IOSET0=1<<CS;
 adcval=(((hbyte&0x0f)<<8)|lbyte);
 return (adcval*3.3/4096*100);
}

char spi_data(unsigned char data)
{
char s;
s=S0SPSR;
S0SPDR=data;
while(((S0SPSR>>7)&1)==0);
return S0SPDR;
}
