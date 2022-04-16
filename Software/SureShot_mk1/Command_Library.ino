
/*0x00* request_protocol_version*/
void request_protocol_version(Message *message, Response *response)
{
  response->common.command = message->common.command;
  response->common.byte_length = 1;
  response->request_protocol_version.protocol_version = PROTOCOL_VERSION;
  ResponseChecksumCalculation(response);
  return;
}

/*0x01* play_request*/
void play_request(Message *message, Response *response)
{
  response->common.command = message->common.command;
  response->common.byte_length = 1;
  /*Play MIDI velocity for 1sec*/
  noteOn(MIDI_CHANNEL, NOTE[message->play_request.pad_nr], VELOCITY[message->play_request.pad_nr]);
  delay(1000);
  noteOff(MIDI_CHANNEL, NOTE[message->play_request.pad_nr], VELOCITY[message->play_request.pad_nr]);
  response->play_request.response = RESPONSE_ACK;
  ResponseChecksumCalculation(response);
  return;
}

/*0x02 read_midi_channel*/
void read_midi_channel(Message *message, Response *response)
{
  response->common.command = message->common.command;
  response->common.byte_length = 1;
  response->read_midi_channel.midi_channel = MIDI_CHANNEL;
  ResponseChecksumCalculation(response);
  return;
}

/*0x03 write_midi_channel*/
void write_midi_channel(Message *message, Response *response)
{
  response->common.command = message->common.command;
  response->common.byte_length = 1;
  uint8_t midi_channel = message->write_midi_channel.midi_channel;

   /*MIDI channel is set only channel number is valid*/
  if  ( (midi_channel <= MIDI_CHANNEL_MAX)
      &&(midi_channel >= MIDI_CHANNEL_MIN) 
      )
  {
    MIDI_CHANNEL = message->write_midi_channel.midi_channel;
    response->write_midi_channel.response=RESPONSE_ACK;
  }
  else
  {
    response->write_midi_channel.response=RESPONSE_NACK; 
  }
  
  ResponseChecksumCalculation(response);
  return;
}

/*0x04 read_note*/
void read_note(Message *message, Response *response)
{
  response->common.command = message->common.command;
  response->common.byte_length = TOTAL_BUTTONS;

  for (byte i=0; i < response->common.byte_length ; i++)
  {
    response->read_note.note[i] = NOTE[i];
  }
  ResponseChecksumCalculation(response);
  return;
}

/*0x05 write_single_note*/
void write_single_note(Message *message, Response *response)
{
  response->common.command = message->common.command;
  response->common.byte_length = 1;
  uint8_t note = message->write_single_note.note;
     /*note is set only note number is valid*/
  if  ( ( (note <= NOTE_MAX)
        &&(note >= NOTE_MIN) 
        )
      &&(message->write_single_note.pad_nr < TOTAL_BUTTONS)
      )
  {
    NOTE[message->write_single_note.pad_nr]=note;
    
    response->write_midi_channel.response=RESPONSE_ACK;
  }
  else
  {
    response->write_midi_channel.response=RESPONSE_NACK; 
  }
  
  ResponseChecksumCalculation(response);
  return;
}

/*0x06 write_all_note*/
void write_all_note(Message *message, Response *response)
{
  response->common.command = message->common.command;
  response->common.byte_length = 1;
  /*note is set only note number is valid*/
  bool note_valid = true;
  uint8_t note[TOTAL_BUTTONS];
  for(byte i=0;i < TOTAL_BUTTONS;i++)
  {
    note[i] = message->write_all_note.note[i];
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
    response->write_midi_channel.response=RESPONSE_ACK;
  }
  else
  {
    response->write_midi_channel.response=RESPONSE_NACK; 
  }
  
  ResponseChecksumCalculation(response);
  return;
}

/*0x07 read_velocity*/
void read_velocity(Message *message, Response *response)
{
  response->common.command = message->common.command;
  response->common.byte_length = TOTAL_BUTTONS;

  for (byte i=0; i < response->common.byte_length ; i++)
  {
    response->read_velocity.velocity[i] = VELOCITY[i];
  }
  ResponseChecksumCalculation(response);
  return;
}

/*0x08 write_single_velocity*/
void write_single_velocity(Message *message, Response *response)
{
  response->common.command = message->common.command;
  response->common.byte_length = 1;
  uint8_t velocity = message->write_single_velocity.velocity;

     /*velocity is set only velocity number is valid*/
  if  ( ( (velocity <= VELOCITY_MAX)
        &&(velocity >= VELOCITY_MIN) 
        )
      && (message->write_single_velocity.pad_nr < TOTAL_BUTTONS)
      )
  {
    VELOCITY[message->write_single_velocity.pad_nr]=velocity;
    response->write_midi_channel.response=RESPONSE_ACK;
  }
  else
  {
    response->write_midi_channel.response=RESPONSE_NACK; 
  }
  
  ResponseChecksumCalculation(response);
  return;
}

/*0x09 write_all_velocity*/
void write_all_velocity(Message *message, Response *response)
{
  response->common.command = message->common.command;
  response->common.byte_length = 1;
  /*velocity is set only velocity number is valid*/
  bool velocity_valid = true;
  uint8_t velocity[TOTAL_BUTTONS];
  for(byte i=0;i < TOTAL_BUTTONS;i++)
  {
    velocity[i] = message->write_all_velocity.velocity[i];
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
    response->write_midi_channel.response=RESPONSE_ACK;
  }
  else
  {
    response->write_midi_channel.response=RESPONSE_NACK; 
  }
  ResponseChecksumCalculation(response);
  return;
}

/*0x0A write_eeprom*/
void write_eeprom(Message *message, Response *response)
{
  response->common.command = message->common.command;
  response->common.byte_length = 1;
  response->write_eeprom.parameter = message->write_eeprom.parameter;
  /*parameter == PARAMETER_EEPROM_MEMORY, write memory to rom*/
  if(message->write_eeprom.parameter == PARAMETER_EEPROM_MEMORY)
  {
    
    Eeprom_Write();
    response->write_eeprom.response=RESPONSE_ACK;
  }
   /*parameter == PARAMETER_EEPROM_DEFAULT, write default values to rom*/
  else if(message->write_eeprom.parameter == PARAMETER_EEPROM_DEFAULT)
  {
    Eeprom_Write_Default();
    response->write_eeprom.response=RESPONSE_ACK;
  }
  else
  {
    response->write_eeprom.response=RESPONSE_NACK;
  }
  ResponseChecksumCalculation(response);
  return;
}

void ResponseChecksumCalculation(Response *response)
{
  uint8_t sum =0;
      sum = sum + response->common.command;
      for (int i=0; i < (response->common.byte_length); i++)
      {
        sum = sum + response->common.bin[i];
      }
      sum = (sum & 0x7F);
      response->common.bin[response->common.byte_length]=sum;
  return;
}
