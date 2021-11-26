#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "kho_1wire.h"


uint8_t oneWire_init_devices(void)
{
    /* VARIALBLES */
    uint8_t temp;
    uint8_t return_value;
    /* BODY */
    temp = oneWire_find_first();
    return_value = temp;
    while (temp)
    {
        temp = oneWire_find_next();
    }
    return return_value;
}

/*This function sets pin as an output and drives it low.*/
void oneWire_drive_low(void)
{
    /* VARIALBLES */

    /* BODY */
    TRISAbits.TRISA4    = PIN_OUTPUT;
    PORTAbits.RA4                 = LOW;
}

/*This function sets pin as an output and drives it high.*/
void oneWire_drive_high(void)
{
   /* VARIALBLES */

   /* BODY */
   TRISAbits.TRISA4     = PIN_OUTPUT;
   PORTAbits.RA4                  = HIGH;
}

/*This function sets pin as an input and read its value.*/
uint8_t oneWire_read_pin(void)
{
    /* VARIALBLES */
    uint8_t return_data = 0;

    /* BODY */
    TRISAbits.TRISA4 = PIN_INPUT;
    if (0x01 == PORTAbits.RA4)
        return_data = HIGH;
    else
        return_data = LOW;

    return return_data;
}

/*This function is used to transmit a byte of data on a bus.*/
void oneWire_write_byte(uint8_t data_to_send)
{
        /* VARIALBLES */
	uint8_t loop;

        /* BODY */
	for (loop = 0; loop < 8; loop++)
	{
		oneWire_write_bit_on_bus(data_to_send & 0x01);
		data_to_send >>= 1;               
	}
}

/*This function is used for reading a complete byte from a bus.*/
uint8_t oneWire_read_byte(void)
{
    /* VARIALBLES */
    uint8_t return_value = LOW;
    unsigned char loop;

    /* BODY */
    for (loop = 0; loop < 8; loop++)
    {
        return_value >>= 1; 	
        if (oneWire_read_bit_from_bus())
        return_value |= 0x80;			
    }

    return return_value;
}

/*Reset function with detection of slaves on bus.*/
uint8_t oneWire_reset_bus(void)
{
    /* VARIALBLES */
    uint8_t presence_detect;

    /* BODY */
    oneWire_drive_low();
    _delay(240);
    _delay(240);
    _delay(240);
    _delay(240);
    _delay(240);
    _delay(240);
    _delay(240);
    _delay(240);
    oneWire_drive_high ();
    _delay(70);
    _delay(70);
    _delay(70);
    _delay(70);
    presence_detect = oneWire_read_pin();
    _delay(205);
    _delay(205);
    _delay(205);
    _delay(205);
    _delay(205);
    _delay(205);
    _delay(205);
    _delay(205);
    oneWire_drive_high ();
    return presence_detect;
}

/*The fuction to write bit on the bus. */
void oneWire_write_bit_on_bus(uint8_t bit_to_set)
{
    /* VARIALBLES */

    /* BODY */
    if (bit_to_set)
    {
        //writing a bit '1'
        oneWire_drive_low(); 			
        _delay(24);				
        oneWire_drive_high ();  			
        _delay(128);				
        _delay(128);
    }
    else
    {
        oneWire_drive_low(); 	
        _delay(240);		
        oneWire_drive_high ();
        _delay(40);	
    }
}

/*The function to read bit from the bus.*/
uint8_t oneWire_read_bit_from_bus(void)
{
    /* VARIALBLES */
    uint8_t return_value = LOW;

    /* BODY */
    oneWire_drive_low();
    _delay(24);		
    oneWire_drive_high ();
    _delay(36);
    return_value = oneWire_read_pin();	
    _delay(220);		
    return return_value;
}

uint8_t oneWire_reset_and_detect(void)
{
    /* VARIALBLES */
    uint8_t return_value;
    /* BODY */
    if (!oneWire_reset_bus())
            return_value = HIGH;
    else
            return_value = LOW;
    return return_value;
}

uint8_t oneWire_search()
{
    /* VARIALBLES */
    uint8_t id_bit_number;
    uint8_t last_zero, rom_byte_number, search_result;
    uint8_t id_bit, cmp_id_bit;
    uint8_t rom_byte_mask, search_direction;
    
    /* BODY */
    id_bit_number = 1;
    last_zero = 0;
    rom_byte_number = 0;
    rom_byte_mask = 1;
    search_result = FALSE;
    crc8 = 0;
    // if the last call was not the last one
    if (!LastDeviceFlag)
    {
    // 1-Wire reset
        if (!oneWire_reset_and_detect())
        {
            // reset the search
            LastDiscrepancy = 0;
            LastDeviceFlag = FALSE;
            LastFamilyDiscrepancy = 0;
            search_result = FALSE;
        }
        else
        {
            // issue the search command
            oneWire_write_byte(0xF0);
            // loop to do the search
            do
            {
                // read a bit and its complement
                id_bit = oneWire_read_bit_from_bus();
                cmp_id_bit = oneWire_read_bit_from_bus();
                // check for no devices on 1-wire
                if ((id_bit == 1) && (cmp_id_bit == 1))
                {
                    break;
                }
                else
                {
                    // all devices coupled have 0 or 1
                    if (id_bit != cmp_id_bit)
                    {
                        search_direction = id_bit; // bit write value for search
                    }
                    else
                    {
                        // if this discrepancy if before the Last Discrepancy
                        // on a previous next then pick the same as last time
                        if (id_bit_number < LastDiscrepancy)
                        {
                            search_direction = ((OneWireDevice_list[current_device][rom_byte_number] & rom_byte_mask) > 0);
                        }
                        else
                        {
                            // if equal to last pick 1, if not then pick 0
                            search_direction = (id_bit_number == LastDiscrepancy);
                        }
                        // if 0 was picked then record its position in LastZero
                        if (search_direction == 0)
                        {
                            last_zero = id_bit_number;
                            // check for Last discrepancy in family
                            if (last_zero < 9)
                            {
                                LastFamilyDiscrepancy = last_zero;
                            }
                        }
                    }
                    // set or clear the bit in the ROM byte rom_byte_number
                    // with mask rom_byte_mask
                    if (search_direction == 1)
                    {
                        OneWireDevice_list[current_device][rom_byte_number] |= rom_byte_mask;
                    }
                    else
                    {
                        OneWireDevice_list[current_device][rom_byte_number] &= ~rom_byte_mask;
                    }
                    // serial number search direction write bit
                    oneWire_write_bit_on_bus(search_direction);
                    // increment the byte counter id_bit_number
                    // and shift the mask rom_byte_mask
                    id_bit_number++;
                    rom_byte_mask <<= 1;
                    // if the mask is 0 then go to new SerialNum byte rom_byte_number and reset mask
                    if (rom_byte_mask == 0)
                    {
                        oneWire_calculate_crc8(OneWireDevice_list[current_device][rom_byte_number]); // accumulate the CRC
                        rom_byte_number++;
                        rom_byte_mask = 1;
                    }
                }
            }
            while(rom_byte_number < 8); // loop until through all ROM bytes 0-7
            // if the search was successful then
            if (!((id_bit_number < 65) || (crc8 != 0)))
            {
                // search successful so set LastDiscrepancy,LastDeviceFlag,search_result
                LastDiscrepancy = last_zero;
                // check for last device
                if (LastDiscrepancy == 0)
                LastDeviceFlag = TRUE;
                search_result = TRUE;
            }
        }
    }
    // if no device found then reset counters so next 'search' will be like a first
    if (!search_result || !OneWireDevice_list[current_device][0])
    {
        LastDiscrepancy = 0;
        LastDeviceFlag = FALSE;
        LastFamilyDiscrepancy = 0;
        search_result = FALSE;
    }

    return search_result;
}

/* The function return CRC8 value acording to CRC table.*/
uint8_t oneWire_calculate_crc8(uint8_t value)
{
    uint8_t crc;
    uint8_t i;

    i = (value ^ crc8) & 0xff;
    crc8 = 0;
    if(i & 1)
      crc8 ^= 0x5e;
    if(i & 2)
      crc8 ^= 0xbc;
    if(i & 4)
      crc8 ^= 0x61;
    if(i & 8)
      crc8 ^= 0xc2;
    if(i & 0x10)
      crc8 ^= 0x9d;
    if(i & 0x20) 
      crc8 ^= 0x23;
    if(i & 0x40)
      crc8 ^= 0x46;
    if(i & 0x80)
      crc8 ^= 0x8c;

    return crc8;
}

/* The function is looking for first device on 1Wire bus.
 * TRUE = found
 * FALSE = no device  */
uint8_t oneWire_find_first(void)
{
    /* VARIALBLES */
    uint8_t return_value;
    /* BODY */
    LastDiscrepancy = 0;
    LastDeviceFlag = FALSE;
    LastFamilyDiscrepancy = 0;
    current_device = 0;
    return_value = oneWire_search();
    if(return_value)
        current_device++;
    return return_value;
}

/* The function is looking for next device on 1Wire bus.
 * TRUE = found
 * FALSE = no device  */
uint8_t oneWire_find_next(void)
{
    /* VARIALBLES */
    uint8_t return_value = FALSE;
    /* BODY */
    if(current_device < MAX_1WIRE_IDS)
    {
        return_value = oneWire_search();
        if(return_value)
        {
            current_device++;
        }
    }
    return return_value;
}
