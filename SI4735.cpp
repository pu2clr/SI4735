#include "Si4735.h"

//���������� ����� �������������� Si4735 �� I2C
//PB6 - SCLK PB7 - SDIO PB8 - #RST PB9 - #SEN

#include <built_in.h>

const Si4735adress = 0b00100010;
//const Si4735adress=0b11000110;

sbit nRST at GPIOB_ODR.B8;
sbit nSEN at GPIOB_ODR.B9;
sbit Led at GPIOC_ODR.B13;

// Software I2C connections
sbit Soft_I2C_Scl_Output at GPIOB_ODR.B6;
sbit Soft_I2C_Scl_Input at GPIOB_IDR.B6;
sbit Soft_I2C_Sda_Output at GPIOB_ODR.B7;
sbit Soft_I2C_Sda_Input at GPIOB_IDR.B7;

char recv_buf[255];
unsigned int l_recv_buf = 0;

unsigned int freq = 0;
char scanflag = 0;

void RecvOne() iv IVT_INT_USART1 ics ICS_AUTO
{
    char status = 0;
    char txt[7];
    char rdd;

    //USART1_CR1.RXNEIE = 0;             //??? ?????????? ?????????? ?? ?????????
    //USART1_CR2.RTOEN=0;  //??????? ????????
    // if(RXNE_bit == 1){          //? ????????? ????? ????
    while (UART1_Data_Ready())
    {

        rdd = Uart1_Read();
        UART1_Write('(');
        UART1_Write(rdd);
        UART1_Write(')');

        if ((rdd >= 48) & (rdd <= 57))
        {
            UART1_Write('>');
            UART1_Write(rdd);
            UART1_Write('<');
            UART1_Write_Text("l_recv_buf ");
            ByteToHex(l_recv_buf, txt);
            UART1_Write_Text(txt);
            recv_buf[l_recv_buf] = rdd;
            l_recv_buf++;
            if (l_recv_buf > 5)
            {
                l_recv_buf = 0;
            };
        }
        else

            if (rdd == 'u')
        { //seek up

            /*
//������� FM_SEEK_START
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress); //�����
Soft_I2C_Write(0x21);//FM_SEEK_START
Soft_I2C_Write(0b00001100);//seekup=1 wrap=1
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress+1); //����� ��� ������
Status=Soft_I2C_Read(0);
Soft_I2C_Stop();
UART1_Write_Text("FM_SEEK_START Status ");
ByteToHex(Status,txt);
UART1_Write_Text(txt);
UART1_Write_Text("\n\r");
*/

            //������� AM_SEEK_START
            Soft_I2C_Start();
            Soft_I2C_Write(Si4735adress); //�����
            Soft_I2C_Write(0x41);         //FM_SEEK_START
            Soft_I2C_Write(0b00001100);   //seekup=1 wrap=1
            Soft_I2C_Write(0x00);
            Soft_I2C_Write(0x00);
            Soft_I2C_Write(0x00);
            Soft_I2C_Write(0x00);
            Soft_I2C_Start();
            Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
            Status = Soft_I2C_Read(0);
            Soft_I2C_Stop();
            UART1_Write_Text("AM_SEEK_START Status ");
            ByteToHex(Status, txt);
            UART1_Write_Text(txt);
            UART1_Write_Text("\n\r");
        }
        else if (rdd == 'd')
        { //seek dn ;
            /*
//������� FM_SEEK_START
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress); //�����
Soft_I2C_Write(0x21);//FM_SEEK_START
Soft_I2C_Write(0b00000100);//seekup=1 wrap=1
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress+1); //����� ��� ������
Status=Soft_I2C_Read(0);
Soft_I2C_Stop();
UART1_Write_Text("FM_SEEK_START Status ");
ByteToHex(Status,txt);
UART1_Write_Text(txt);
UART1_Write_Text("\n\r");
*/

            //������� AM_SEEK_START
            Soft_I2C_Start();
            Soft_I2C_Write(Si4735adress); //�����
            Soft_I2C_Write(0x41);         //FM_SEEK_START
            Soft_I2C_Write(0b00000100);   //seekup=0 wrap=1
            Soft_I2C_Write(0x00);
            Soft_I2C_Write(0x00);
            Soft_I2C_Write(0x00);
            Soft_I2C_Write(0x00);
            Soft_I2C_Start();
            Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
            Status = Soft_I2C_Read(0);
            Soft_I2C_Stop();
            UART1_Write_Text("AM_SEEK_START Status ");
            ByteToHex(Status, txt);
            UART1_Write_Text(txt);
            UART1_Write_Text("\n\r");
        }
        else if (rdd == 's')
        { //scan manual ;
            scanflag = ~scanflag;
        }
        else if (rdd == 13)
        {
            recv_buf[l_recv_buf] = 0; //����������� ������
            UART1_Write_Text(" recv_buf ");
            UART1_Write_Text(recv_buf);
            freq = StrToInt(recv_buf);
            WordToStr(freq, txt);
            UART1_Write_Text("Freq ");
            UART1_Write_Text(txt);
            UART1_Write_Text(" kHz\n\r");
            l_recv_buf = 0;

            //������� AM_TUNE_FREQ

            Soft_I2C_Start();
            Soft_I2C_Write(Si4735adress); //�����
            Soft_I2C_Write(0x40);         //AM_TUNE_FREQ
            Soft_I2C_Write(0b00);         // Fast=0
            Soft_I2C_Write(Hi(freq));     //FreqH
            Soft_I2C_Write(Lo(freq));     //FreqL
            Soft_I2C_Write(0x00);         //AntCapH
            Soft_I2C_Write(0x00);         //AntCapL
            Soft_I2C_Start();
            Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
            Status = Soft_I2C_Read(0);
            Soft_I2C_Stop();
            UART1_Write_Text("AM_TUNE_FREQ Status ");
            ByteToHex(Status, txt);
            UART1_Write_Text(txt);
            UART1_Write_Text("\n\r");
        };
    };

    USART1_SR = 0;

    //USART1_CR1.RXNEIE = 1;
}

void main()
{
    char status = 0;
    char txt[7];
    char buf[5]; //����� ��� i2c
    char rev[7];
    unsigned int i;
    GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_8 | _GPIO_PINMASK_9);
    GPIO_Digital_Output(&GPIOC_BASE, _GPIO_PINMASK_13);
    UART1_Init(57600);
    //I2C1_Init();
    Soft_I2C_Init();
    UART1_Write_Text("Init\n\r");

    //SEN � ����
    nSEN = 0;
    //RST � ����
    nRST = 1;

    /*
//���� ����������
for (i=1;i<255;i++){

Soft_I2C_Start();
Soft_I2C_Write(i); //�����
Status=Soft_I2C_Read(0);
Soft_I2C_Stop();
ByteToHex(i,txt);
UART1_Write_Text(txt);
UART1_Write_Text(":");
ByteToHex(Status,txt);
UART1_Write_Text(txt);
UART1_Write_Text(" ");


};
*/

    /*

//������� Power Up
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress); //�����
Soft_I2C_Write(0x01);//Power Up
Soft_I2C_Write(0b01010000);//FM, GPO2OEN OSC32768 ���
Soft_I2C_Write(0b00000101);//Analog Audio Out
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress+1); //����� ��� ������
Status=Soft_I2C_Read(0);
Soft_I2C_Stop();
UART1_Write_Text("Power Up Ststus ");
ByteToHex(Status,txt);
UART1_Write_Text(txt);
UART1_Write_Text("\n\r");

delay_ms(100);

//������� Get Rev
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress); //�����
Soft_I2C_Write(0x10);//Get Rev
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress+1); //����� ��� ������
rev[0]=Soft_I2C_Read(1);
rev[1]=Soft_I2C_Read(1);
rev[2]=Soft_I2C_Read(1);
rev[3]=Soft_I2C_Read(1);
rev[4]=Soft_I2C_Read(1);
rev[5]=Soft_I2C_Read(1);
rev[6]=Soft_I2C_Read(1);
rev[7]=Soft_I2C_Read(0);
Soft_I2C_Stop();
UART1_Write_Text("Get Rev ");
for(i=0;i<8;i++){
ByteToHex(rev[i],txt);
UART1_Write_Text(txt);
UART1_Write_Text(" ");
};
UART1_Write_Text("\n\r");

//������� FM_TUNE_FREQ

Soft_I2C_Start();
Soft_I2C_Write(Si4735adress); //�����
Soft_I2C_Write(0x20);//FM_TUNE_FREQ
Soft_I2C_Write(0b00);//Freeze=0 Fast=0
Soft_I2C_Write(0x22);//FreqH
Soft_I2C_Write(0x2E);//FreqL
Soft_I2C_Write(0x00);//AntCap
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress+1); //����� ��� ������
Status=Soft_I2C_Read(0);
Soft_I2C_Stop();
UART1_Write_Text("FM_TUNE_FREQ Status ");
ByteToHex(Status,txt);
UART1_Write_Text(txt);
UART1_Write_Text("\n\r");

//�������FM_TUNE_STATUS
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress); //�����
Soft_I2C_Write(0x22);//FM_TUNE_STATUS
Soft_I2C_Write(0x00);//Cancel=0 Intack=0
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress+1); //����� ��� ������
rev[0]=Soft_I2C_Read(1);
rev[1]=Soft_I2C_Read(1);
rev[2]=Soft_I2C_Read(1);
rev[3]=Soft_I2C_Read(1);
rev[4]=Soft_I2C_Read(1);
rev[5]=Soft_I2C_Read(1);
rev[6]=Soft_I2C_Read(1);
rev[7]=Soft_I2C_Read(0);
Soft_I2C_Stop();
UART1_Write_Text("FM_TUNE_STATUS ");
for(i=0;i<8;i++){
ByteToHex(rev[i],txt);
UART1_Write_Text(txt);
UART1_Write_Text(" ");
};
UART1_Write_Text("\n\r");

//������� FM_SEEK_START
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress); //�����
Soft_I2C_Write(0x21);//FM_SEEK_START
Soft_I2C_Write(0b00001100);//seekup=1 wrap=1
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress+1); //����� ��� ������
Status=Soft_I2C_Read(0);
Soft_I2C_Stop();
UART1_Write_Text("FM_SEEK_START Status ");
ByteToHex(Status,txt);
UART1_Write_Text(txt);
UART1_Write_Text("\n\r");

 */

    //������� Power Up   - AM
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress); //�����
    Soft_I2C_Write(0x01);         //Power Up
    Soft_I2C_Write(0b01010001);   //AM, GPO2OEN OSC32768 ���
    Soft_I2C_Write(0b00000101);   //Analog Audio Out
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
    Status = Soft_I2C_Read(0);
    Soft_I2C_Stop();
    UART1_Write_Text("Power Up Ststus ");
    ByteToHex(Status, txt);
    UART1_Write_Text(txt);
    UART1_Write_Text("\n\r");

    delay_ms(100);

    //������� Get Rev
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress); //�����
    Soft_I2C_Write(0x10);         //Get Rev
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
    rev[0] = Soft_I2C_Read(1);
    rev[1] = Soft_I2C_Read(1);
    rev[2] = Soft_I2C_Read(1);
    rev[3] = Soft_I2C_Read(1);
    rev[4] = Soft_I2C_Read(1);
    rev[5] = Soft_I2C_Read(1);
    rev[6] = Soft_I2C_Read(1);
    rev[7] = Soft_I2C_Read(0);
    Soft_I2C_Stop();
    UART1_Write_Text("Get Rev ");
    for (i = 0; i < 8; i++)
    {
        ByteToHex(rev[i], txt);
        UART1_Write_Text(txt);
        UART1_Write_Text(" ");
    };
    UART1_Write_Text("\n\r");

    //������������� ��������
    //AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN 0x3103  = 0x7800
    //AM_SOFT_MUTE_SNR_THRESHOLD 0x3303 = 0
    //AM_SEEK_BAND_BOTTOM 0x3400 = 100
    //AM_SEEK_BAND_TOP 0x3401 = 30000
    //AM_SEEK_FREQ_SPACING 0x3402 = 5 ���
    //AM_SEEK_TUNE_SNR_THRESHOLD 0x3403 = 1
    //AM_SEEK_TUNE_RSSI_THRESHOLD 0x3404 = 1
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress); //�����
    Soft_I2C_Write(0x12);         //Set Property
    Soft_I2C_Write(0b00);         // 0
    Soft_I2C_Write(0x31);         //PropH
    Soft_I2C_Write(0x03);         //PropL
    Soft_I2C_Write(0x78);         //PropDH
    Soft_I2C_Write(0x00);         //PropDL
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
    Status = Soft_I2C_Read(0);
    Soft_I2C_Stop();
    UART1_Write_Text("Set Property AM_AUTOMATIC_VOLUME_CONTROL_MAX_GAIN Status ");
    ByteToHex(Status, txt);
    UART1_Write_Text(txt);
    UART1_Write_Text("\n\r");

    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress); //�����
    Soft_I2C_Write(0x12);         //Set Property
    Soft_I2C_Write(0b00);         // 0
    Soft_I2C_Write(0x33);         //PropH
    Soft_I2C_Write(0x03);         //PropL
    Soft_I2C_Write(0x00);         //PropDH
    Soft_I2C_Write(0x00);         //PropDL
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
    Status = Soft_I2C_Read(0);
    Soft_I2C_Stop();
    UART1_Write_Text("Set Property AM_SOFT_MUTE_SNR_THRESHOLD Status ");
    ByteToHex(Status, txt);
    UART1_Write_Text(txt);
    UART1_Write_Text("\n\r");

    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress); //�����
    Soft_I2C_Write(0x12);         //Set Property
    Soft_I2C_Write(0b00);         // 0
    Soft_I2C_Write(0x34);         //PropH
    Soft_I2C_Write(0x00);         //PropL
    Soft_I2C_Write(0x00);         //PropDH
    Soft_I2C_Write(0x64);         //PropDL
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
    Status = Soft_I2C_Read(0);
    Soft_I2C_Stop();
    UART1_Write_Text("Set Property AM_SEEK_BAND_BOTTOM Status ");
    ByteToHex(Status, txt);
    UART1_Write_Text(txt);
    UART1_Write_Text("\n\r");

    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress); //�����
    Soft_I2C_Write(0x12);         //Set Property
    Soft_I2C_Write(0b00);         // 0
    Soft_I2C_Write(0x34);         //PropH
    Soft_I2C_Write(0x01);         //PropL
    Soft_I2C_Write(0x75);         //PropDH
    Soft_I2C_Write(0x30);         //PropDL
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
    Status = Soft_I2C_Read(0);
    Soft_I2C_Stop();
    UART1_Write_Text("Set Property AM_SEEK_BAND_TOP Status ");
    ByteToHex(Status, txt);
    UART1_Write_Text(txt);
    UART1_Write_Text("\n\r");

    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress); //�����
    Soft_I2C_Write(0x12);         //Set Property
    Soft_I2C_Write(0b00);         // 0
    Soft_I2C_Write(0x34);         //PropH
    Soft_I2C_Write(0x02);         //PropL
    Soft_I2C_Write(0x00);         //PropDH
    Soft_I2C_Write(0x05);         //PropDL
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
    Status = Soft_I2C_Read(0);
    Soft_I2C_Stop();
    UART1_Write_Text("Set Property AM_SEEK_FREQ_SPACING Status ");
    ByteToHex(Status, txt);
    UART1_Write_Text(txt);
    UART1_Write_Text("\n\r");

    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress); //�����
    Soft_I2C_Write(0x12);         //Set Property
    Soft_I2C_Write(0b00);         // 0
    Soft_I2C_Write(0x34);         //PropH
    Soft_I2C_Write(0x03);         //PropL
    Soft_I2C_Write(0x00);         //PropDH
    Soft_I2C_Write(0x01);         //PropDL
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
    Status = Soft_I2C_Read(0);
    Soft_I2C_Stop();
    UART1_Write_Text("Set Property AM_SEEK_TUNE_SNR_THRESHOLD Status ");
    ByteToHex(Status, txt);
    UART1_Write_Text(txt);
    UART1_Write_Text("\n\r");

    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress); //�����
    Soft_I2C_Write(0x12);         //Set Property
    Soft_I2C_Write(0b00);         // 0
    Soft_I2C_Write(0x34);         //PropH
    Soft_I2C_Write(0x04);         //PropL
    Soft_I2C_Write(0x00);         //PropDH
    Soft_I2C_Write(0x01);         //PropDL
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
    Status = Soft_I2C_Read(0);
    Soft_I2C_Stop();
    UART1_Write_Text("Set Property AM_SEEK_TUNE_RSSI_THRESHOLD Status ");
    ByteToHex(Status, txt);
    UART1_Write_Text(txt);
    UART1_Write_Text("\n\r");

    //������� AM_TUNE_FREQ

    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress); //�����
    Soft_I2C_Write(0x40);         //AM_TUNE_FREQ
    Soft_I2C_Write(0b00);         // Fast=0
    Soft_I2C_Write(0x69);         //FreqH
    Soft_I2C_Write(0xff);         //FreqL
    Soft_I2C_Write(0x00);         //AntCapH
    Soft_I2C_Write(0x00);         //AntCapL
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
    Status = Soft_I2C_Read(0);
    Soft_I2C_Stop();
    UART1_Write_Text("AM_TUNE_FREQ Status ");
    ByteToHex(Status, txt);
    UART1_Write_Text(txt);
    UART1_Write_Text("\n\r");

    //�������AM_TUNE_STATUS
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress); //�����
    Soft_I2C_Write(0x42);         //AM_TUNE_STATUS
    Soft_I2C_Write(0x00);         //Cancel=0 Intack=0
    Soft_I2C_Start();
    Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
    rev[0] = Soft_I2C_Read(1);
    rev[1] = Soft_I2C_Read(1);
    rev[2] = Soft_I2C_Read(1);
    rev[3] = Soft_I2C_Read(1);
    rev[4] = Soft_I2C_Read(1);
    rev[5] = Soft_I2C_Read(1);
    rev[6] = Soft_I2C_Read(1);
    rev[7] = Soft_I2C_Read(0);
    Soft_I2C_Stop();
    UART1_Write_Text("AM_TUNE_STATUS ");
    for (i = 0; i < 8; i++)
    {
        ByteToHex(rev[i], txt);
        UART1_Write_Text(txt);
        UART1_Write_Text(" ");
    };
    UART1_Write_Text("\n\r");
    /*
//������� FM_SEEK_START
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress); //�����
Soft_I2C_Write(0x21);//FM_SEEK_START
Soft_I2C_Write(0b00001100);//seekup=1 wrap=1
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress+1); //����� ��� ������
Status=Soft_I2C_Read(0);
Soft_I2C_Stop();
UART1_Write_Text("FM_SEEK_START Status ");
ByteToHex(Status,txt);
UART1_Write_Text(txt);
UART1_Write_Text("\n\r");
  */

    USART1_CR1.RXNEIE = 1;          //??? ?????????? ?????????? ?? ?????????
    NVIC_IntEnable(IVT_INT_USART1); //????????? ?????????? ?? ?????????
    while (1)
    {
        /*
//�������FM_TUNE_STATUS
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress); //�����
Soft_I2C_Write(0x22);//FM_TUNE_STATUS
Soft_I2C_Write(0x00);//Cancel=0 Intack=0
Soft_I2C_Start();
Soft_I2C_Write(Si4735adress+1); //����� ��� ������
rev[0]=Soft_I2C_Read(1);
rev[1]=Soft_I2C_Read(1);
rev[2]=Soft_I2C_Read(1);
rev[3]=Soft_I2C_Read(1);
rev[4]=Soft_I2C_Read(1);
rev[5]=Soft_I2C_Read(1);
rev[6]=Soft_I2C_Read(1);
rev[7]=Soft_I2C_Read(0);
Soft_I2C_Stop();
UART1_Write_Text("FM_TUNE_STATUS ");
for(i=0;i<8;i++){
ByteToHex(rev[i],txt);
UART1_Write_Text(txt);
UART1_Write_Text(" ");
};
UART1_Write_Text("\n\r");

*/

        if (scanflag)
        {
            freq++;

            WordToStr(freq, txt);
            UART1_Write_Text("Freq ");
            UART1_Write_Text(txt);
            UART1_Write_Text(" kHz");
            //������� AM_TUNE_FREQ

            Soft_I2C_Start();
            Soft_I2C_Write(Si4735adress); //�����
            Soft_I2C_Write(0x40);         //AM_TUNE_FREQ
            Soft_I2C_Write(0b0000000);    // Fast=0
            Soft_I2C_Write(Hi(freq));     //FreqH
            Soft_I2C_Write(Lo(freq));     //FreqL
            Soft_I2C_Write(0x00);         //AntCapH
            Soft_I2C_Write(0x00);         //AntCapL
            Soft_I2C_Start();
            Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
            Status = Soft_I2C_Read(0);
            Soft_I2C_Stop();
            //UART1_Write_Text("AM_TUNE_FREQ Status ");
            //ByteToHex(Status,txt);
            //UART1_Write_Text(txt);
            //UART1_Write_Text("\n\r");
        };

        //������� AM_TUNE_STATUS
        Soft_I2C_Start();
        Soft_I2C_Write(Si4735adress); //�����
        Soft_I2C_Write(0x42);         //AM_TUNE_STATUS
        Soft_I2C_Write(0x00);         //Cancel=0 Intack=0
        Soft_I2C_Start();
        Soft_I2C_Write(Si4735adress + 1); //����� ��� ������
        rev[0] = Soft_I2C_Read(1);
        rev[1] = Soft_I2C_Read(1);
        rev[2] = Soft_I2C_Read(1);
        rev[3] = Soft_I2C_Read(1);
        rev[4] = Soft_I2C_Read(1);
        rev[5] = Soft_I2C_Read(1);
        rev[6] = Soft_I2C_Read(1);
        rev[7] = Soft_I2C_Read(0);
        Soft_I2C_Stop();
        if (scanflag)
        {
            UART1_Write_Text("RSSI ");
            IntToHex(rev[4], txt);
            UART1_Write_Text(txt);
            UART1_Write_Text("\n\r");
        }
        else
        {
            UART1_Write_Text("AM_TUNE_STATUS ");
            for (i = 0; i < 8; i++)
            {
                ByteToHex(rev[i], txt);
                UART1_Write_Text(txt);
                UART1_Write_Text(" ");
            };
            UART1_Write_Text("\n\r");
        };

        delay_ms(500);

        Led = ~Led;
    }
}
