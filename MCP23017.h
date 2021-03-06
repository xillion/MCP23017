/*  MCP23017 library for Arduino
    Copyright (C) 2009 David Pye    <davidmpye@gmail.com
    Modified for use on the MBED ARM platform

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef     MBED_MCP23017_H
#define     MBED_MCP23017_H

#include    "mbed.h"

#define MASK_ITNPOL  2   //INTPOL: This bit sets the polarity of the INT output pin
                         //1 = Active-high
                         //0 = Active-low
#define MASK_ODR     4   //bit 2 ODR: Configures the INT pin as an open-drain output
                         //1 = Open-drain output (overrides the INTPOL bit.)
                         //0 = Active driver output (INTPOL bit sets the polarity.)
#define MASK_DISSLW  16  //bit 4 DISSLW: Slew Rate control bit for SDA output
                         //1 = Slew rate disabled
                         //0 = Slew rate enabled
#define MASK_SEQOP   32  //bit 5 SEQOP: Sequential Operation mode bit
                         //1 = Sequential operation disabled, address pointer does not increment.
                         //0 = Sequential operation enabled, address pointer increments.
#define MASK_MIRROR  64  //bit 6 MIRROR: INT Pins Mirror bit
                         //1 = The INT pins are internally connected
                         //0 = The INT pins are not connected. INTA is associated with PORTA and INTB is associated with PORTB
#define MASK_BANK    128 //bit 7 BANK: Controls how the registers are addressed
                         //1 = The registers associated with each port are separated into different banks.
                         //0 = The registers are in the same bank (addresses are sequential).



//
// Register defines from data sheet - we set IOCON.BANK to 0
// as it is easier to manage the registers sequentially.
//
#define     IODIR       0x00 //Controls the direction of the data I/O.
#define     IPOL        0x02 //This register allows the user to configure the polarity on the corresponding GPIO port bits.
#define     GPINTEN     0x04 //The GPINTEN register controls the interrupt-on-change feature for each pin.
#define     DEFVAL      0x06 //
#define     INTCON      0x08 //
#define     IOCON       0x0A //
#define     GPPU        0x0C //
#define     INTF        0x0E //
#define     INTCAP      0x10 //
#define     GPIO        0x12 //
#define     OLAT        0x14 //

#define     I2C_BASE_ADDRESS    0x40

#define     DIR_OUTPUT      0
#define     DIR_INPUT       1

/** MCP23017 class
 *
 * Allow access to an I2C connected MCP23017 16-bit I/O extender chip
 * Example:
 * @code
 *      MCP23017     *par_port; 
 * @endcode
 *
 */
class MCP23017 {
public:

    enum Frequency {
        Frequency_100KHz = 100000,
        Frequency_400KHz = 400000,
        /* Note: 1.7MHz probably won't work for mbed */
        Frequency_1700KHz = 1700000
    };
    enum Pin {
        Pin_PA0 = 0x0001,
        Pin_PA1 = 0x0002,
        Pin_PA2 = 0x0004,
        Pin_PA3 = 0x0008,
        Pin_PA4 = 0x0010,
        Pin_PA5 = 0x0020,
        Pin_PA6 = 0x0040,
        Pin_PA7 = 0x0080,

        Pin_PB0 = 0x0100,
        Pin_PB1 = 0x0200,
        Pin_PB2 = 0x0400,
        Pin_PB3 = 0x0800,
        Pin_PB4 = 0x1000,
        Pin_PB5 = 0x2000,
        Pin_PB6 = 0x4000,
        Pin_PB7 = 0x8000,
        Pin_All = 0xFFFF

    };
    /** Constructor for the MCP23017 connected to specified I2C pins at a specific address
     *
     * 16-bit I/O expander with I2C interface
     *
     * @param   sda         I2C data pin
     * @param   scl         I2C clock pin
     * @param   i2cAddress  I2C address
     * @param   i2cSpeed    I2C speed
     */
    MCP23017(PinName sda, PinName scl, int i2cAddress, Frequency freq = Frequency_400KHz);

    /** Reset MCP23017 device to its power-on state
     */    
    void reset(void);

    /** Write a 0/1 value to an output bit
     *
     * @param   value         0 or 1
     * @param   bit_number    bit number range 0 --> 15
     */   
    void write_bit(int value, int bit_number);
      
    /** Write a masked 16-bit value to the device
     *
     * @param   data    16-bit data value
     * @param   mask    16-bit mask value
     */       
    void write_mask(unsigned short data, unsigned short mask);

    /** Read a 0/1 value from an input bit
     *
     * @param   bit_number    bit number range 0 --> 15
     * @return                0/1 value read
     */       
    int  read_bit(int bit_number);
    
    /** Read a 16-bit value from the device and apply mask
     *
     * @param   mask    16-bit mask value
     * @return          16-bit data with mask applied
     */     
    int  read_mask(unsigned short mask);

    /** Configure an MCP23017 device
     *
     * @param   dir_config         data direction value (1 = input, 0 = output)
     * @param   pullup_config      100k pullup value (1 = enabled, 0 = disabled)
     * @param   polarity_config    polarity value (1 = flip, 0 = normal)
     */           
    void portConfig(unsigned short dir_config, unsigned short pullup_config, unsigned short polarity_config);
    /** Configure an interrupts of MCP23017 device
     *
     * @param   compare_value       compare value (1/0)
     * @param   interrupt_ctrl      interrupt ctrl (1 = on campare, 0 = on change)
     */ 
    void interruptConfig(unsigned short compare_value, unsigned short interrupt_ctrl);
    
    void IOCONConfig(unsigned char IOCON_value);

    void writeRegister(int regAddress, unsigned char  val);
    void writeRegister(int regAddress, unsigned short val);
    int  readRegister(int regAddress);

/*----------------------------------------------------------------------------- 
 * pinmode
 * Set units to sequential, bank0 mode
 */  
    void pinMode(int pin, int mode); 
    void digitalWrite(int pin, int val);
    int  digitalRead(int pin);

// These provide a more advanced mapping of the chip functionality
// See the data sheet for more information on what they do

//Returns a word with the current pin states (ie contents of the GPIO register)
    unsigned short digitalWordRead();
// Allows you to write a word to the GPIO register
    void digitalWordWrite(unsigned short w);
// Sets up the polarity mask that the MCP23017 supports
// if set to 1, it will flip the actual pin value.
    void inputPolarityMask(unsigned short mask);
//Sets which pins are inputs or outputs (1 = input, 0 = output) NB Opposite to arduino's
//definition for these
    void inputOutputMask(unsigned short mask);
// Allows enabling of the internal 100k pullup resisters (1 = enabled, 0 = disabled)
    void internalPullupMask(unsigned short mask);
    int read(void);
    void write(int data);
    /** Disables interrupts for the specified pins.
     *
     * @param values A bitmask indicating which interrupts should be disabled.
     */
    void disableInterrupts ( unsigned short mask );
    /** Enables interrupts for the specified pins.
     *
     * @param values A bitmask indicating which interrupts should be enabled.
     */
    void enableInterrupts ( unsigned short mask );
    /** Acknowledge a generated interrupt.
     *
     * This function must be called when an interrupt is generated to discover
     * which pin caused the interrupt and to enable future interrupts.
     *
     * @param pin An output paramter that specifies which pin generated the interrupt.
     * @param values The current state of the input pins.
     */
    void ackInterrupt( unsigned short &pin, unsigned short &values );

private:
    I2C     _i2c;
    int     MCP23017_i2cAddress;                        // physical I2C address
    unsigned short   shadow_GPIO, shadow_IODIR, shadow_GPPU, shadow_IPOL, shadow_GPINTEN;     // Cached copies of the register values
    
};

#endif
