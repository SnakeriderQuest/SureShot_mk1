
#define MIDI_CHANNEL_DEFAULT        0x00
#define VELOCITY_ARRAY_DEFAULT      {127,127,127,127,127,127,127,127,127,127,127,127,127,127,127,127}
#define NOTE_ARRAY_DEFAULT          {36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51}

uint8_t MIDI_CHANNEL;
uint8_t VELOCITY[TOTAL_BUTTONS];
byte NOTE[TOTAL_BUTTONS];

const uint8_t MIDI_CHANNEL_MAX = 0x15;
const uint8_t MIDI_CHANNEL_MIN = 0x00;
const uint8_t NOTE_MAX =0x7F; 
const uint8_t NOTE_MIN =0x00;

const uint8_t VELOCITY_MAX =0x7F; 
const uint8_t VELOCITY_MIN =0x00;

const byte SYSEX_EXCLUSIVE_STATUS         = 0xF0;
const byte SYSEX_END_OF_EXCLUSIVE         = 0xF7;
const byte SYSEX_ID_UPPER_BYTE            = 0x00;
const byte SYSEX_ID_MIDDLE_BYTE           = 0x5F;       /*ID gotten from AMEI*/
const byte SYSEX_ID_LOWER_BYTE            = 0x7F;       /*ID gotten from AMEI*/
const byte SYSEX_DESTINATION_SURESHOT     = 0x00;       /*SURESHOT*/
const byte SYSEX_PROTOCOL_VERSION         = 0x01;       /*Ver.1*/
const byte SYSEX_DESTINATION_HOST         = 0x40;

const byte SYSEX_HEADER_SYSEX_START_OR_CONTINUE   = 0x04;
const byte SYSEX_HEADER_SYSEX_END_WITH_ONE_BYTE   = 0x05;
const byte SYSEX_HEADER_SYSEX_END_WITH_TWO_BYTE   = 0x06;
const byte SYSEX_HEADER_SYSEX_END_WITH_THREE_BYTE = 0x07;    
     
const uint8_t SYSEX_RESPONSE_FIXED_BYTE_LENGTH    = 9;      /*Status, ID x3,Destination, Version, command, checksum, EOL*/
const uint8_t SYSEX_RESPONSE_FIXED_START_BYTE_LENGTH    = 6;      /*Status, ID x3,Destination, Version*/
