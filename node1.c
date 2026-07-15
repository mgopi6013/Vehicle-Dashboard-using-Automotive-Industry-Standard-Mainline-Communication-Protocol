#include <lpc21xx.h>
#include "lcd.c"
#include "can.c"
#define SW 3
CAN2_MSG rx;
unsigned char temperature = 0;
unsigned char fuel = 0;
int r = 0;
int dir = 1;
int x = (1000 * 60) - 1;

void delay_ms(unsigned int ms)
{
    T0PR = 60000 - 1;
    T0TCR = 0x01;
    while(T0TC < ms);
    T0TCR = 0x03;
    T0TCR = 0x00;
}

void pwm_init(void)
{
    PINSEL1 |= (1 << 10);       // PWM5
    VPBDIV = 1;
    PWMMR0 = (20000 * 60) - 1;
    PWMMR5 = x;
    PWMMCR = 0x02;
    PWMPCR = (1 << 13);
    PWMLER = (1 << 0) | (1 << 5);
    PWMTCR = 0x09;
}

int main()
{
    LCD_INIT();
    pwm_init();
    can2_init();
LCD_COMMAND(0x01);
LCD_COMMAND(0x80);
LCD_STR("Vehicle Dashboard");
LCD_COMMAND(0xC0);
LCD_STR("Using Automotive");
LCD_COMMAND(0x94);
LCD_STR("Industry Standard");
LCD_COMMAND(0xD4);
LCD_STR("CAN Protocol");
delay_ms(3000);
	
    while(1)
    {
        /* Receive CAN message */
        can2_rx(&rx);
        if(rx.id == 0x100)
        {
            temperature = rx.byteA & 0xFF;
        }
        if(rx.id == 0x101)
        {
            fuel = rx.byteA & 0xFF;
        }
        /* Wiper Switch */
        if(((IOPIN0 >> SW) & 1) == 0)
        {
            delay_ms(200);
            while(((IOPIN0 >> SW) & 1) == 0);
            if(r == 0)
            {
                r = 1;
            }
            else
            {
                x = (1000 * 60) - 1;
                PWMMR5 = x;
                PWMLER = (1 << 5);
                delay_ms(500);
                dir = 1;
                r = 0;
            }
        }
        /* Servo Movement */
        if(r)
        {
            PWMMR5 = x;
            PWMLER = (1 << 5);
            delay_ms(20);
            if(dir)
            {
                x += (56 * 60);
                if(x >= (2100 * 60) - 1)
                    dir = 0;
            }
            else
            {
                x -= (56 * 60);
                if(x <= (1000 * 60) - 1)
                    dir = 1;
            }
        }
/* LCD Display */
LCD_COMMAND(0x01);
/* Line 1 */
LCD_COMMAND(0x80);
LCD_STR("CAR DASHBOARD");
/* Line 2 */
LCD_COMMAND(0xC0);
LCD_STR("Temp : ");
LCD_INTEGER(temperature);
LCD_DATA(0xDF);      // Degree symbol (optional)
LCD_DATA('C');
/* Line 3 */
LCD_COMMAND(0x94);
LCD_STR("Fuel : ");
LCD_INTEGER(fuel);
LCD_DATA('%');
/* Line 4 */
LCD_COMMAND(0xD4);
LCD_STR("Wiper: ");
if(r)
    LCD_STR("ON ");
else
    LCD_STR("OFF");
delay_ms(100);
    }
}