#include<LPC17xx.h>
#define RS_CTRL 0x08000000  // P0.27: Register Select
#define EN_CTRL 0x10000000  // P0.28: Enable
#define DT_CTRL 0x07800000  // P0.23 - P0.26 Data lines

unsigned long int temp1 = 0, temp2 = 0, i;
unsigned char msg[] = {"Welcome"};

void lcd_init(void); // initalize lcd
void wr_cn(void); // RS 0
void clr_disp(void); // clear display
void delay_lcd(unsigned int); // delay
void lcd_com(void); // sending commands
void wr_dr(void); // RS 1
void lcd_data(void); // write data
void clear_ports(void);
void lcd_puts(unsigned char *); // writes entire message at current cursor position

int main(void) {
    SystemInit();
    SystemCoreClockUpdate();

    lcd_init();
    delay_lcd(3000);
    temp1 = 0x80; // 1st message, 1st line
    lcd_com(); // sends commands
    delay_lcd(800);
    lcd_puts(msg);
    // temp1 = 0xC0
    // lcd_com();
    // delay_lcd(800);
    // lcd_puts(msg);
    // delay_lcd(5000);
    while(1);
}

void lcd_init() {
    // Ports initialized as GPIO
    LPC_PINCON->PINSEL1 &= 0xFC003FFF;

    LPC_GPIO0->FIODIR |= DT_CTRL;
    LPC_GPIO0->FIODIR |= RS_CTRL;
    LPC_GPIO0->FIODIR |= EN_CTRL;

    clear_ports();
    delay_lcd(200000); // won't clear immediately, requires some time
    temp1 = 0x20;
    temp2 = temp1 & 0xF0;
    temp2 = temp2 <<< 19;
    wr_cn();
    delay_lcd(30000);

    temp1 = 0x28;
    lcd_com();
    delay_lcd(30000);

    temp1 = 0x0C;
    lcd_com();
    delay_lcd(800);

    temp1 = 0x06;
    lcd_com();
    delay_lcd(800);
}

void clear_ports(void) {
    LPC_GPIO0->FIOCLR = DT_CTRL;
    LPC_GPIO0->FIOCLR = RS_CTRL;
    LPC_GPIO0->FIOCLR = EN_CTRL;
}

void lcd_com(void) {
    temp2 = temp1 & 0x80; // to get 2 from 0x28
    temp2 = temp2 << 19;
    wr_cn();
    delay_lcd(1000);
    return;
}

void wr_cn(void) {
    clear_ports();
    LPC_GPIO0->FIOPIN = temp2;
    LPC_GPIO0->FIOCLR = RS_CTRL; // RS = 0
    LPC_GPIO0->FIOSET = EN_CTRL; // EN = 1
    delay_lcd(28);
    LPC_GPIO0->FIOCLR = EN_CTRL; // EN = 0
    return;
}

void lcd_data(void) {
    temp2 = temp1 & 0xF0;
    temp2 = temp2 << 19;
    wr_dn();
}

void lcd_puts(unsigned char *buff) {
    unsigned int i = 0;
    while(buff[i] != '\0') {
        temp1 = buff[i];
        lcd_data();
        i++;
        if(i == 16) {
            temp1 = 0xC0;
            lcd_com();
        }
    }
    return;
}
