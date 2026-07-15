#include <lpc21xx.h>
typedef struct CAN2_MSG
{
    unsigned int id;      // CAN Identifier
    unsigned char dlc;    // Data Length Code (0-8)
    unsigned char rtr;    // Remote Transmission Request

    unsigned int byteA;   // Data Byte0 - Byte3
    unsigned int byteB;   // Data Byte4 - Byte7

}CAN2_MSG;

void can2_init(void)
{
    /* P0.23 -> RD2
       P0.24 -> TD2 */
    PINSEL1 |= 0x00014000;
    VPBDIV = 1;          // PCLK = CCLK = 60 MHz
    C2MOD = 0x01;        // Reset mode
    AFMR = 0x02;         // Accept all messages
    /* 125 kbps @ 60 MHz PCLK */
    C2BTR = 0x001C001D;
    C2MOD = 0x00;        // Normal mode
}

void can2_tx(CAN2_MSG m1)
{
    while((C2SR & (1<<2)) == 0);
    C2TID1 = m1.id;
    C2TFI1 = (m1.dlc << 16);
    if(m1.rtr == 0)
    {
        C2TFI1 &= ~(1<<30);
        C2TDA1 = m1.byteA;
        C2TDB1 = m1.byteB;
    }
    else
    {
        C2TFI1 |= (1<<30);
    }
    C2CMR = (1<<0) | (1<<5);
    while((C2GSR & (1<<2)) == 0);
}
unsigned char can2_rx(CAN2_MSG *m1)
{
    /* No message received */
    if((C2GSR & 0x01) == 0)
        return 0;

    m1->id  = C2RID;
    m1->dlc = (C2RFS >> 16) & 0x0F;
    m1->rtr = (C2RFS >> 30) & 0x01;

    if(m1->rtr == 0)
    {
        m1->byteA = C2RDA;
        m1->byteB = C2RDB;
    }

    /* Release receive buffer */
    C2CMR = (1 << 2);

    return 1;
}}