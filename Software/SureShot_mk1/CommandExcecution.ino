void CommandExcecution(Message *message, Response *response)
{
  switch(message->common.command)
  {
    case cmd_request_protocol_version:
      Serial.print("cmd_request_protocol_version");
      request_protocol_version(message, response);
      break;

    case cmd_play_request:
      Serial.print("cmd_play_request");
      play_request(message, response);
      break;

    case cmd_read_midi_channel:
      Serial.print("cmd_read_midi_channel");
      read_midi_channel(message, response);
      break;

    case cmd_write_midi_channel:
      write_midi_channel(message, response);
      break;

    case cmd_read_note:
      Serial.print("cmd_read_note");
      read_note(message, response);
      break;

    case cmd_write_single_note:
      Serial.print("cmd_write_single_note");
      write_single_note(message, response);
      break;

    case cmd_write_all_note:
      Serial.print("cmd_write_all_note");
      write_all_note(message, response);
      break;

    case cmd_read_velocity:
      Serial.print("cmd_read_velocity");
      read_velocity(message, response);
      break;

    case cmd_write_single_velocity:
      Serial.print("cmd_write_single_velocity");
      write_single_velocity(message, response);
      break;

    case cmd_write_all_velocity:
      Serial.print("cmd_write_all_velocity");
      write_all_velocity(message, response);
      break;

    case cmd_write_eeprom:
      Serial.print("cmd_write_eeprom");
      write_eeprom(message, response);
      break;
    
  }
}
