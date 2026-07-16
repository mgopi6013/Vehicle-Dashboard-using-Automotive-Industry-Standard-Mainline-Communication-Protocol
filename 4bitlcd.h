#define LCD_D (0xf<<14)
#define RS (1<<12)
#define E (1<<13)
void delay_sec(unsigned int sec){

T0PR=15000000-1;

T0TCR=0X01;

while(T0TC<sec);

T0TCR=0X03;

T0TCR=0X00;

}


void delay_ms(unsigned int ms){

T0PR=15000-1;

T0TCR=0X01;

while(T0TC<ms);

T0TCR=0X03;

T0TCR=0X00;

}
void LCD_CMD(unsigned char cmd);
void LCD_INIT(void)
{
	IODIR0 |= LCD_D|RS|E;
	LCD_CMD(0X01);
	LCD_CMD(0X02);
	LCD_CMD(0X0C);
	LCD_CMD(0X28);
}
void LCD_CMD(unsigned char cmd)
{
	 IOCLR0=LCD_D;
	 IOSET0=(cmd&0XF0)<<10;
	 IOCLR0=RS; 
	 IOSET0=E;
	 delay_ms(2);
	 IOCLR0=E;

	 IOCLR0=LCD_D;
	 IOSET0=(cmd&0X0f)<<14;
	 IOCLR0=RS; 
	 IOSET0=E;
	 delay_ms(2);
	 IOCLR0=E;

	
}

void LCD_DATA(unsigned char d)
{
	 IOCLR0=LCD_D;
	 IOSET0=(d&0XF0)<<10;
	 IOSET0=RS; 
	 IOSET0=E;
	 delay_ms(2);
	 IOCLR0=E;

	 IOCLR0=LCD_D;
	 IOSET0=(d&0X0f)<<14;
	 IOSET0=RS; 
	 IOSET0=E;
	 delay_ms(2);
	 IOCLR0=E;
}
/*******TO DISPLAY INTEGER ON LCD*******/

void LCD_INT(int n)
{
	unsigned char arr[5];
	signed char i=0;
	if(n==0)
	{
	        LCD_DATA('0');
  	}
	else
	{
		if(n<0)
		{
			LCD_DATA('-');
		        n=-n;
		}
		while(n>0)
		{
			arr[i++]=n%10;
			n=n/10;
    }
		for(--i;i>=0;i--)
		LCD_DATA(arr[i]+48);
	}
}

/*********TO DISPLAY STRING ON DISPLAY******/

void LCD_STR(unsigned char *s)
{
	
	while(*s)
	{
		LCD_DATA(*s++);
	
  }
}

/********TO DISPLAY FLOAT ON LCD*******/

void LCD_FLT(float f)
{
	int temp;
	temp=f;
	LCD_INT(temp);
	LCD_DATA('.');
	temp=(f-temp)*1000;
	LCD_INT(temp);
}

/********TO SCROLL THE STRING*********/

void LCD_STRSCROL(unsigned char *s,unsigned char i)
{
	while(*s)
	{
		LCD_DATA(*s++);
		if(i==16)			LCD_CMD(0Xc0);
		i++;
  }
}
 
