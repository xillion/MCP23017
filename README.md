# MCP23017
MCP23017 lib for mbed

## Code example

```cpp
#include "mbed.h"
#include "MCP23017.h" // include 16-bit parallel I/O header file 


#define COLLECT_LED PB_1
#define CHARGE_LED PB_2
#define BTN_INT_PIN PA_0
#define COLLECT_BTN PC_13
#define CHARGE_BTN PB_7
#define MDM_RELEY PA_1



EventFlags event;

Serial pc(USBTX, USBRX);
MCP23017 port(PC_1, PC_0, I2C_BASE_ADDRESS);

InterruptIn key_pressed_int(BTN_INT_PIN, PullUp);
//InterruptIn btn1(COLLECT_BTN);
//InterruptIn btn2(CHARGE_BTN);

DigitalOut charge(CHARGE_LED);
DigitalOut collect(COLLECT_LED);
PwmOut modem(MDM_RELEY);

bool isr_done = false;

void pr(void)
{
    isr_done = true;
    event.set(0x01);
}

void int_thread(void)
{
    while (true)
    {
        event.wait_all(0x01);
        if(collect == 1)
        {
            collect = 0;
        }
        else
        {
            collect = 1;
        }
    }
}

int main(void)
{
    pc.baud(115200);
    key_pressed_int.rise(pr);
    unsigned short intfl;
    unsigned short intcap;
    unsigned short intport;
    unsigned short  &ref_intfl = intfl;
    unsigned short  &ref_intcap = intcap;

    /** Configure an MCP23017 device
     *
     * @param   dir_config         data direction value (1 = input, 0 = output)
     * @param   pullup_config      100k pullup value (1 = enabled, 0 = disabled)
     * @param   polarity_config    polarity value (1 = flip, 0 = normal)
     */           

    port.portConfig(0xFFFF, 0xFFFF, 0x0000);
    port.IOCONConfig(MASK_ITNPOL+MASK_MIRROR);
    port.interruptConfig(0xFFFF, 0xFFFF);
    port.enableInterrupts(0x0300);
    
    

    while(true)
    {
        if(isr_done)
        {
            port.ackInterrupt(ref_intfl, ref_intcap);
            intport = port.digitalWordRead();
            pc.printf("%s","interrup \n");
            pc.printf("INTF = %d\n",intfl);
            pc.printf("INTCAP = %d\n",intcap);
            pc.printf("PORT = %d\n",intport);
            isr_done = false;
        }
            //port.digitalWordRead();
    }
}
```
