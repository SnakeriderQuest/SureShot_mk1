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
    byte byte_length;    
    byte bin[18];
  }write_interface;

  struct
  {
    byte byte_length;    
    byte bin[18];
  }checksum_interface;


  struct 
  {
    byte byte_length;
    byte command;
    byte bin[17];
  }common;

  struct 
  {
    byte byte_length;
    byte command;
    byte parameter;
    byte checksum;
  }request_protocol_version;

  struct
  {
    byte byte_length;    
    byte command;
    byte pad_nr;
    byte checksum;
  }play_request;

  struct
  { 
    byte byte_length;
    byte command;
    byte parameter;
    byte checksum;
  }read_midi_channel;

  struct 
  {
    byte byte_length;
    byte command;
    byte midi_channel;
    byte checksum;
  }write_midi_channel;

  struct
  {
    byte byte_length;    
    byte command;
    byte parameter;
    byte checksum;
  }read_note;

  struct
  {
    byte byte_length;
    byte command;
    byte pad_nr;
    byte note;
    byte checksum;
  }write_single_note;
  
  struct write_all_note
  {
    byte byte_length;    
    byte command;
    byte note[16];
    byte checksum;
  }write_all_note;

  struct 
  {
    byte byte_length;
    byte command;
    byte parameter;
    byte checksum;
  }read_velocity;

  struct 
  {
    byte command;
    byte byte_length;
    byte pad_nr;
    byte velocity;
    byte checksum;
  }write_single_velocity;

  struct
  {
    byte command;
    byte byte_length;
    byte velocity[16];
    byte checksum;
  }write_all_velocity;

  struct
  {
    byte command;
    byte byte_length;
    byte parameter;
    byte checksum;
  }write_eeprom;
};

/*responseponse definitions*/
union Response
{
  struct
  {
    byte byte_length;    
    byte bin[18];
  }write_interface;
  struct
  {
    byte byte_length;    
    byte bin[18];
  }checksum_interface;
  
  struct 
  {
    byte byte_length;
    byte command;
    byte bin[17];
  }common;

  struct 
  {
    byte byte_length;
    byte command;
    byte protocol_version;
    byte checksum;
  }request_protocol_version;

  struct 
  {
    byte byte_length;
    byte command;
    byte response;
    byte checksum;
  }play_request;

  struct 
  {
    byte byte_length;
    byte command;
    byte midi_channel;
    byte checksum;
  }read_midi_channel;

  struct 
  {
    byte byte_length;
    byte command;
    byte response;
    byte checksum;
  }write_midi_channel;

  struct 
  {
    byte byte_length;
    byte command;
    byte note[TOTAL_BUTTONS];
    byte checksum;
  }read_note; 

  struct 
  {
    byte byte_length;   
    byte command;
    byte response;
    byte checksum;
  }write_single_note;

  struct 
  {
    byte byte_length;
    byte command;
    byte response;
    byte checksum;
  }write_all_note;

  struct 
  {
    byte byte_length;
    byte command;
    byte velocity[TOTAL_BUTTONS];
    byte checksum;
  }read_velocity; 

  struct 
  { 
    byte byte_length;
    byte command;
    byte response;
    byte checksum;
  }write_single_velocity;

  
  struct 
  {
    byte byte_length;
    byte command;
    byte response;
    byte checksum;
  }write_all_velocity;
  
  struct 
  {
    byte byte_length;
    byte command;
    byte parameter;
    byte response;
    byte checksum;
  }write_eeprom;
  
};
