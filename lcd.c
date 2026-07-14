#define LCD_D 0xFF<<23

#define RS 1<<21

#define E 1<<22

void LCD_INIT(void);

void LCD_COMMAND(unsigned char);

void LCD_DATA(unsigned char);

void LCD_INT(int);

void LCD_STR(unsigned char*);

void LCD_SCROLL_RL(unsigned char *);
void float_display(float);

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


void LCD_INIT(void){

IODIR1 |=LCD_D|RS|E;

LCD_COMMAND(0x01);

LCD_COMMAND(0X02);

LCD_COMMAND(0X0C);

LCD_COMMAND(0X38);

}


void LCD_COMMAND(unsigned char cmd){

IOCLR1=LCD_D;

IOSET1=cmd<<23;

IOCLR1=RS;

IOSET1=E;

delay_ms(2);

IOCLR1=E;

}


void LCD_DATA(unsigned char d){

IOCLR1=LCD_D;

IOSET1=d<<23;

IOSET1=RS;

IOSET1=E;

delay_ms(2);

IOCLR1=E;

}


void LCD_INT(int n){

unsigned char a[10];

int i=0;

if(n==0)

LCD_DATA('0');

else{

if(n<0){

LCD_DATA('-');

n=-n;

}

while(n>0){

a[i++]=n%10;

n/=10;

}

for(--i;i>=0;i--)

LCD_DATA(a[i]+48);

}

}


void LCD_STR(unsigned char *s){

while(*s){

LCD_DATA(*s++);

}

}

void LCD_SCROLL_RL(unsigned char *msg)

{

    unsigned char buf[120] = "                "; // 16 leading spaces

    int i, j = 16,k;


    while(*msg)

        buf[j++] = *msg++;


    for(i = 0; i < 16; i++)      // 16 trailing spaces

        buf[j++] = ' ';


    buf[j] = '\0';


    for(i = 0; i <= j - 16; i++)

    {

        LCD_COMMAND(0x80);       // First line, first position


        

        for(k = 0; k < 16; k++)

            LCD_DATA(buf[i + k]);


        delay_ms(200);

    }

}

void float_display(float f_val)
{
  unsigned int number;
  number = f_val;
  LCD_INT(number);
  LCD_DATA('.');
  number=(f_val-number)*100;
  LCD_INT(number);	
}

