/******************************************************************************/
/* User Level #define Macros                                                  */
/******************************************************************************/

#define _125MS_SYSTEM_TICK  0xFF-0x40
#define _250MS_SYSTEM_TICK  0xFF-0x80
#define _500MS_SYSTEM_TICK  0xFF-0x00

#define SYSTEM_TICK _125MS_SYSTEM_TICK

volatile uint16_t systemTick;   //counter increased by timer, according to
                                //data received from RTC
                                //input from rtc 512 Hz
volatile uint16_t old_systemTick;

volatile uint8_t buttonStatus;
volatile uint8_t rotationStatus;

//#define READOUT_ON 1
//#define READOUT_OFF 0

/******************************************************************************/
/* User Function Prototypes                                                   */
/******************************************************************************/

void InitApp(void);         /* I/O and Peripheral Initialization */
