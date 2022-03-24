/*0x00* request_protocol_version*/
void request_protocol_version(SysExMessage *sysExMessage, SysExResponse *sysExResponse)
{
  sysExResponse->common.command = sysExMessage->common.command;
  sysExResponse->common.byte_length = 1;
  sysExResponse->request_protocol_version.protocol_version = PROTOCOL_VERSION;
  SysExResponseChecksumCalculation(sysExResponse);
  return;
}

/*0x01* play_request*/
void play_request(SysExMessage *sysExMessage, SysExResponse *sysExResponse)
{
  sysExResponse->common.command = sysExMessage->common.command;
  sysExResponse->common.byte_length = 1;
  /*Play MIDI velocity for 1sec*/
  noteOn(MIDI_CHANNEL, NOTE[sysExMessage->play_request.pad_nr], VELOCITY[sysExMessage->play_request.pad_nr]);
  delay(1000);
  noteOff(MIDI_CHANNEL, NOTE[sysExMessage->play_request.pad_nr], VELOCITY[sysExMessage->play_request.pad_nr]);
  sysExResponse->play_request.response = RESPONSE_ACK;
  SysExResponseChecksumCalculation(sysExResponse);
  return;
}

/*0x02 read_midi_channel*/
void read_midi_channel(SysExMessage *sysExMessage, SysExResponse *sysExResponse)
{
  sysExResponse->common.command = sysExMessage->common.command;
  sysExResponse->common.byte_length = 1;
  sysExResponse->read_midi_channel.midi_channel = MIDI_CHANNEL;
  SysExResponseChecksumCalculation(sysExResponse);
  return;
}

/*0x03 write_midi_channel*/
void write_midi_channel(SysExMessage *sysExMessage, SysExResponse *sysExResponse)
{
  sysExResponse->common.command = sysExMessage->common.command;
  sysExResponse->common.byte_length = 1;
  uint8_t midi_channel = sysExMessage->write_midi_channel.midi_channel;

   /*MIDI channel is set only channel number is valid*/
  if  ( (midi_channel <= MIDI_CHANNEL_MAX)
      &&(midi_channel >= MIDI_CHANNEL_MIN) 
      )
  {
    MIDI_CHANNEL = sysExMessage->write_midi_channel.midi_channel;
    sysExResponse->write_midi_channel.response=RESPONSE_ACK;
  }
  else
  {
    sysExResponse->write_midi_channel.response=RESPONSE_NACK; 
  }
  
  SysExResponseChecksumCalculation(sysExResponse);
  return;
}

/*0x04 read_note*/
void read_note(SysExMessage *sysExMessage, SysExResponse *sysExResponse)
{
  sysExResponse->common.command = sysExMessage->common.command;
  sysExResponse->common.byte_length = TOTAL_BUTTONS;

  for (byte i=0; i < sysExResponse->common.byte_length ; i++)
  {
    sysExResponse->read_note.note[i] = NOTE[i];
  }
  SysExResponseChecksumCalculation(sysExResponse);
  return;
}

/*0x05 write_single_note*/
void write_single_note(SysExMessage *sysExMessage, SysExResponse *sysExResponse)
{
  sysExResponse->common.command = sysExMessage->common.command;
  sysExResponse->common.byte_length = 1;
  uint8_t note = sysExMessage->write_single_note.note;
     /*note is set only note number is valid*/
  if  ( ( (note <= NOTE_MAX)
        &&(note >= NOTE_MIN) 
        )
      &&(sysExMessage->write_single_note.pad_nr < TOTAL_BUTTONS)
      )
  {
    NOTE[sysExMessage->write_single_note.pad_nr]=note;
    
    sysExResponse->write_midi_channel.response=RESPONSE_ACK;
  }
  else
  {
    sysExResponse->write_midi_channel.response=RESPONSE_NACK; 
  }
  
  SysExResponseChecksumCalculation(sysExResponse);
  return;
}

/*0x06 write_all_note*/
void write_all_note(SysExMessage *sysExMessage, SysExResponse *sysExResponse)
{
  sysExResponse->common.command = sysExMessage->common.command;
  sysExResponse->common.byte_length = 1;
  /*note is set only note number is valid*/
  bool note_valid = true;
  uint8_t note[TOTAL_BUTTONS];
  for(byte i=0;i < TOTAL_BUTTONS;i++)
  {
    note[i] = sysExMessage->write_all_note.note[i];
    if( (note[i] > NOTE_MAX)
      ||(note[i] < NOTE_MIN)
      )
    {
      note_valid =false;
    }
  }
     /*MIDI channel is set only note number is valid*/
  if  (note_valid==true)
  {
    /*set all pad's notes*/
    for(byte i=0;i < TOTAL_BUTTONS;i++)
    {
      NOTE[i]=note[i];
    }
    sysExResponse->write_midi_channel.response=RESPONSE_ACK;
  }
  else
  {
    sysExResponse->write_midi_channel.response=RESPONSE_NACK; 
  }
  
  SysExResponseChecksumCalculation(sysExResponse);
  return;
}

/*0x07 read_velocity*/
void read_velocity(SysExMessage *sysExMessage, SysExResponse *sysExResponse)
{
  sysExResponse->common.command = sysExMessage->common.command;
  sysExResponse->common.byte_length = TOTAL_BUTTONS;

  for (byte i=0; i < sysExResponse->common.byte_length ; i++)
  {
    sysExResponse->read_velocity.velocity[i] = VELOCITY[i];
  }
  SysExResponseChecksumCalculation(sysExResponse);
  return;
}

/*0x08 write_single_velocity*/
void write_single_velocity(SysExMessage *sysExMessage, SysExResponse *sysExResponse)
{
  sysExResponse->common.command = sysExMessage->common.command;
  sysExResponse->common.byte_length = 1;
  uint8_t velocity = sysExMessage->write_single_velocity.velocity;

     /*velocity is set only velocity number is valid*/
  if  ( ( (velocity <= VELOCITY_MAX)
        &&(velocity >= VELOCITY_MIN) 
        )
      && (sysExMessage->write_single_velocity.pad_nr < TOTAL_BUTTONS)
      )
  {
    VELOCITY[sysExMessage->write_single_velocity.pad_nr]=velocity;
    sysExResponse->write_midi_channel.response=RESPONSE_ACK;
  }
  else
  {
    sysExResponse->write_midi_channel.response=RESPONSE_NACK; 
  }
  
  SysExResponseChecksumCalculation(sysExResponse);
  return;
}

/*0x09 write_all_velocity*/
void write_all_velocity(SysExMessage *sysExMessage, SysExResponse *sysExResponse)
{
  sysExResponse->common.command = sysExMessage->common.command;
  sysExResponse->common.byte_length = 1;
  /*velocity is set only velocity number is valid*/
  bool velocity_valid = true;
  uint8_t velocity[TOTAL_BUTTONS];
  for(byte i=0;i < TOTAL_BUTTONS;i++)
  {
    velocity[i] = sysExMessage->write_all_velocity.velocity[i];
    if( (velocity[i] > VELOCITY_MAX)
      ||(velocity[i] < VELOCITY_MIN)
      )
    {
      velocity_valid =false;
    }
  }
     /*MIDI channel is set only velocity number is valid*/
  if  (velocity_valid==true)
  {
    /*set all pad's velocitys*/
    for(byte i=0;i < TOTAL_BUTTONS;i++)
    {
      VELOCITY[i]=velocity[i];
    }
    sysExResponse->write_midi_channel.response=RESPONSE_ACK;
  }
  else
  {
    sysExResponse->write_midi_channel.response=RESPONSE_NACK; 
  }
  SysExResponseChecksumCalculation(sysExResponse);
  return;
}

/*0x0A write_eeprom*/
void write_eeprom(SysExMessage *sysExMessage, SysExResponse *sysExResponse)
{
  sysExResponse->common.command = sysExMessage->common.command;
  sysExResponse->common.byte_length = 1;
  sysExResponse->write_eeprom.parameter = sysExMessage->write_eeprom.parameter;
  /*parameter == PARAMETER_EEPROM_MEMORY, write memory to rom*/
  if(sysExMessage->write_eeprom.parameter == PARAMETER_EEPROM_MEMORY)
  {
    
    Eeprom_Write();
    sysExResponse->write_eeprom.response=RESPONSE_ACK;
  }
   /*parameter == PARAMETER_EEPROM_DEFAULT, write default values to rom*/
  else if(sysExMessage->write_eeprom.parameter == PARAMETER_EEPROM_DEFAULT)
  {
    Eeprom_Write_Default();
    sysExResponse->write_eeprom.response=RESPONSE_ACK;
  }
  else
  {
    sysExResponse->write_eeprom.response=RESPONSE_NACK;
  }
  
}

void SysExResponseChecksumCalculation(SysExResponse *sysExResponse)
{
  uint8_t sum =0;
      sum = sum + sysExResponse->common.command;
      for (int i=0; i < (sysExResponse->common.byte_length); i++)
      {
        sum = sum + sysExResponse->common.bin[i];
      }
      sum = (sum & 0x7F);
      sysExResponse->common.bin[sysExResponse->common.byte_length]=sum;
  return;
}
