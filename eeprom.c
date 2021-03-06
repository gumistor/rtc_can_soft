//Initialize EEPROM
//ADDRESS:description
//0x00:class of the device
//0x01:id of the device
//0x02:RTC configuration byte - default value 0x40
#include <xc.h>
__EEPROM_DATA(0x00, 0x00, 0x40, 0x03, 0x00, 0x00, 0x00, 0x00);

//CLASS:
//0x00 - RTC
//0x10 - SENSORS
//0x20 - ACTUATORS
//0x30 - DISPLAYS