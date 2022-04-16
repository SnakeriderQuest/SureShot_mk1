/*Setup Serial interface*/
#include <PacketSerial.h>
#define EEPROM_WRITTEN_AS_DEFAULT 0x10
#define EEPROM_WRITTEN_MODIFIED   0x20

#define EEPROM_STATE_ADDRESS        0x00
#define EEPROM_MIDI_CHANNEL_ADDRESS 0x10
#define EEPROM_VELOCITY_ADDRESS     0x20
#define EEPROM_NOTE_ADDRESS         0x40

const uint8_t TOTAL_BUTTONS = 16;
const uint8_t BUTTONS_PIN[TOTAL_BUTTONS] = {2,3,4,5,6,7,8,9,10,11,12,A0,A1,A2,A3,A4};

//for ComInterface
//PacketSerial myPacketSerial;
