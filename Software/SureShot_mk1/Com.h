#define cmd_request_protocol_version  0x00
#define cmd_play_request              0x01
#define cmd_read_midi_channel         0x02
#define cmd_write_midi_channel        0x03
#define cmd_read_note                 0x04
#define cmd_write_single_note         0x05
#define cmd_write_all_note            0x06
#define cmd_read_velocity             0x07
#define cmd_write_single_velocity     0x08
#define cmd_write_all_velocity        0x09
#define cmd_write_eeprom              0x0a

#define RESPONSE_ACK                  0x01
#define RESPONSE_NACK                 0xFF

#define PROTOCOL_VERSION              0x00
#define MODE_SURESHOT                 0x00

#define HEADER_MESSAGE_FIRST          0xFF
#define HEADER_MESSAGE_SECCOND        0x77

#define HEADER_RESPONSE_FIRST          0xFF
#define HEADER_RESPONSE_SECCOND        0xB7

#define PARAMETER_FIXED                0x00
#define PARAMETER_EEPROM_MEMORY        0x00
#define PARAMETER_EEPROM_DEFAULT       0x01

#define FIXED_BYTE_LENGTH              6      /*header0 + header1 + mode + command + Length + checksum*/

/*message definitions*/
union Message
{ 
  struct
  {
    uint8_t byte_length;    
    uint8_t bin[18];
  }write_interface;

  struct
  {
    uint8_t byte_length;    
    uint8_t bin[18];
  }checksum_interface;


  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t bin[17];
  }common;

  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t parameter;
    uint8_t checksum;
  }request_protocol_version;

  struct
  {
    uint8_t byte_length;    
    uint8_t command;
    uint8_t pad_nr;
    uint8_t checksum;
  }play_request;

  struct
  { 
    uint8_t byte_length;
    uint8_t command;
    uint8_t parameter;
    uint8_t checksum;
  }read_midi_channel;

  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t midi_channel;
    uint8_t checksum;
  }write_midi_channel;

  struct
  {
    uint8_t byte_length;    
    uint8_t command;
    uint8_t parameter;
    uint8_t checksum;
  }read_note;

  struct
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t pad_nr;
    uint8_t note;
    uint8_t checksum;
  }write_single_note;
  
  struct write_all_note
  {
    uint8_t byte_length;    
    uint8_t command;
    uint8_t note[16];
    uint8_t checksum;
  }write_all_note;

  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t parameter;
    uint8_t checksum;
  }read_velocity;

  struct 
  {
    uint8_t command;
    uint8_t byte_length;
    uint8_t pad_nr;
    uint8_t velocity;
    uint8_t checksum;
  }write_single_velocity;

  struct
  {
    uint8_t command;
    uint8_t byte_length;
    uint8_t velocity[16];
    uint8_t checksum;
  }write_all_velocity;

  struct
  {
    uint8_t command;
    uint8_t byte_length;
    uint8_t parameter;
    uint8_t checksum;
  }write_eeprom;
};

/*responseponse definitions*/
union Response
{
  struct
  {
    uint8_t byte_length;    
    uint8_t bin[18];
  }read_interface;
  struct
  {
    uint8_t byte_length;    
    uint8_t bin[18];
  }read_interface;
  
  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t bin[17];
  }common;

  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t protocol_version;
    uint8_t checksum;
  }request_protocol_version;

  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t response;
    uint8_t checksum;
  }play_request;

  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t midi_channel;
    uint8_t checksum;
  }read_midi_channel;

  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t response;
    uint8_t checksum;
  }write_midi_channel;

  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t note[TOTAL_BUTTONS];
    uint8_t checksum;
  }read_note; 

  struct 
  {
    uint8_t byte_length;   
    uint8_t command;
    uint8_t response;
    uint8_t checksum;
  }write_single_note;

  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t response;
    uint8_t checksum;
  }write_all_note;

  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t velocity[TOTAL_BUTTONS];
    uint8_t checksum;
  }read_velocity; 

  struct 
  { 
    uint8_t byte_length;
    uint8_t command;
    uint8_t response;
    uint8_t checksum;
  }write_single_velocity;

  
  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t response;
    uint8_t checksum;
  }write_all_velocity;
  
  struct 
  {
    uint8_t byte_length;
    uint8_t command;
    uint8_t parameter;
    uint8_t response;
    uint8_t checksum;
  }write_eeprom;
  
};
