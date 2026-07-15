#include<LPC21XX.H>
#define LCD_D 0xFF<<8
#define RS 1<<16
#define E 1<<17

void LCD_INIT(void);
void LCD_COMMAND(unsigned char);
void LCD_DATA(unsigned char);
void LCD_STR(unsigned char *);

void LCD_INIT(void)
{
	IODIR0 |= LCD_D|RS|E;
	LCD_COMMAND(0x01);
	LCD_COMMAND(0x02);
	LCD_COMMAND(0x0C);
	LCD_COMMAND(0x28);
	LCD_COMMAND(0x80);
}

void LCD_COMMAND(unsigned char cmd)
{
	IOCLR0 = LCD_D;
	IOSET0 = (cmd&0xF0)<<4;
	IOCLR0 = RS;
	IOSET0 = E;
	delay_ms(2);
	IOCLR0 =E;
	
	IOCLR0 = LCD_D;
	IOSET0 = (cmd&0x0F)<<8;
	IOCLR0 = RS;
	IOSET0 = E;
	delay_ms(2);
	IOCLR0 =E;
}

void LCD_DATA(unsigned char d)
{
	IOCLR0 = LCD_D;
	IOSET0 = (d&0XF0)<<4;
	IOSET0 = RS;
	IOSET0 = E;
	delay_ms(2);
	IOCLR0 =E;
	
	IOCLR0 = LCD_D;
	IOSET0 = (d&0X0F)<<8;
	IOSET0 = RS;
	IOSET0 = E;
	delay_ms(2);
	IOCLR0 =E;
}

void LCD_STR(unsigned char*s)
{
	char count=0;
	while(*s)
	{
		LCD_DATA(*s++);
		count++;
		if(count==16)
			LCD_COMMAND(0xC0);
	}
}



