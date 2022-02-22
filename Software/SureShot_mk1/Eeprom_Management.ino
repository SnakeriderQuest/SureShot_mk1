void Eeprom_Handler()
{
  uint8_t eeprom_status = EEPROM.read(EEPROM_STATE_ADDRESS);
  if  (   (eeprom_status != EEPROM_WRITTEN_AS_DEFAULT)
      &&  (eeprom_status != EEPROM_WRITTEN_MODIFIED)
      )
  {
    Eeprom_Write_Default();
  }

  Eeprom_Read();
}

void Eeprom_Write_Default()
{
  /*default values*/
  uint8_t eeprom_status = EEPROM_WRITTEN_AS_DEFAULT;
  uint8_t midi_channel = MIDI_CHANNEL_DEFAULT;
  uint8_t velocity[] = VELOCITY_ARRAY_DEFAULT;
  uint8_t note[] = NOTE_ARRAY_DEFAULT;
  
  EEPROM.put(EEPROM_STATE_ADDRESS, eeprom_status);
  EEPROM.put(EEPROM_MIDI_CHANNEL_ADDRESS, midi_channel);
  EEPROM.put(EEPROM_VELOCITY_ADDRESS, velocity);
  EEPROM.put(EEPROM_NOTE_ADDRESS, note);
  
  for(int i=0 ;i<16;i++)
  {
    Serial.write(EEPROM.read(EEPROM_VELOCITY_ADDRESS+i));
  }
  Eeprom_Read();
  
}

void Eeprom_Read()
{
  EEPROM.get(EEPROM_MIDI_CHANNEL_ADDRESS, MIDI_CHANNEL);
  EEPROM.get(EEPROM_VELOCITY_ADDRESS, VELOCITY);
  EEPROM.get(EEPROM_NOTE_ADDRESS, NOTE);
}

void Eeprom_Write()
{
  EEPROM.put(EEPROM_MIDI_CHANNEL_ADDRESS, MIDI_CHANNEL);
  EEPROM.put(EEPROM_VELOCITY_ADDRESS, VELOCITY);
  EEPROM.put(EEPROM_NOTE_ADDRESS, NOTE);
}
