

volatile uint8_t tempBuffer[64];               //受信生データ保存配列
volatile boolean serial_update_flag = false;   //データの受信フラッグ
volatile byte received_data = 0;              //受信データ


void SysExHandler(byte* array, unsigned size)
{
  Serial.print("size:");
  Serial.print(size);
  Serial.print("\n");
  Serial.print("array:");
  for(int i =0; i < size; i++)
  {
    Serial.print(array[i],HEX);
    Serial.print("\n");
  }

  
  char sendPacket[30];
  Message message;
  uint8_t total_length =size-SYSEX_RESPONSE_FIXED_START_BYTE_LENGTH;
  message.write_interface.byte_length=size-SYSEX_RESPONSE_FIXED_START_BYTE_LENGTH - SYSEX_RESPONSE_FIXED_END_BYTE_LENGTH; /*comnmand + parameter*/


   if (   (array[0]==SYSEX_EXCLUSIVE_STATUS)
      &&  (array[1]==SYSEX_ID_UPPER_BYTE)
      &&  (array[2]==SYSEX_ID_MIDDLE_BYTE)      /*SysEx message to Bulbousbow*/
      &&  (array[3]==SYSEX_ID_LOWER_BYTE)
      &&  (array[4]==SYSEX_DESTINATION_SURESHOT)
      &&  (array[5]==SYSEX_PROTOCOL_VERSION)
      )
      {
        for ( int index = 0;index < total_length ;index++ )        
         {
            message.write_interface.bin[index] = array[index + SYSEX_RESPONSE_FIXED_START_BYTE_LENGTH];
         }
      }

    if (message.common.byte_length > 0)
    {
      /*calculate checksum*/
      uint8_t checksum_of_packet = message.checksum_interface.bin[message.checksum_interface.byte_length];
      uint8_t calculated_checksum = MessageChecksumCalculation(&message);

      if(checksum_of_packet==calculated_checksum)
      {
        Response response;
        CommandExcecution(&message, &response);
        
        /*Send response via serial port*/
        SendResponseSysExPacket(&response);

      }
    }
}

uint8_t MessageChecksumCalculation(Message *message)
{

  int32_t sum =0;
  for (int index =0; index < (message->checksum_interface.byte_length);index++)   /*as last byte is checksum, excluded from sum calculation*/
  { 
    sum = sum + (int32_t)message->checksum_interface.bin[index];
  }
  return (sum & 0x7F);
}


void SendResponseSysExPacket(Response *response)
{
  char sendPacket[30];

  uint8_t command_data_length = (SYSEX_RESPONSE_FIXED_BYTE_LENGTH + response->common.byte_length); /*checksum, so +1*/  
  uint8_t bin[command_data_length]; 
  bin[0] = SYSEX_EXCLUSIVE_STATUS;
  bin[1] = SYSEX_ID_UPPER_BYTE;
  bin[2] = SYSEX_ID_MIDDLE_BYTE;
  bin[3] = SYSEX_ID_LOWER_BYTE;
  bin[4] = SYSEX_DESTINATION_HOST;
  bin[5] = SYSEX_PROTOCOL_VERSION;

  for(int i=0; i < command_data_length-1 ;i++)       
  {
    bin[SYSEX_RESPONSE_FIXED_START_BYTE_LENGTH + i] = response->write_interface.bin[i];
  }
  bin[command_data_length-1] = SYSEX_END_OF_EXCLUSIVE;   /*last byte of sysex is command_data_length (last byte of command data + 1)*/

  /*separate data bytes to midiEventPacket_t*/
  uint8_t last_packet_byte_number = command_data_length%3;
  uint8_t packet_number;
  uint8_t remainder_byte = 0; 
  
  if(last_packet_byte_number > 0)
  {
    remainder_byte = 1;
  }
  packet_number = (command_data_length/3) + remainder_byte;

/*Send continuous SysEx*/
  for(int i=0; i < packet_number - 1; i++)        /*exclude last packet, packet_number-1*/
  {
    uint8_t bin_current_index = i * 3;
    midiEventPacket_t sysExResponse = { SYSEX_HEADER_SYSEX_START_OR_CONTINUE,     /*header*/
                                        bin[bin_current_index],                   /*byte1*/
                                        bin[bin_current_index +1],                   /*byte2*/
                                        bin[bin_current_index +2]                   /*byte3*/
                                      };
    MidiUSB.sendMIDI(sysExResponse);     
    MidiUSB.flush();
  }

  /*Send last packet of SysEx*/
  midiEventPacket_t sysExResponse_last_packet;
  uint8_t bin_last_packet_index = (packet_number -1) * 3;

  switch(last_packet_byte_number)
  {

    case 1:
      sysExResponse_last_packet = { SYSEX_HEADER_SYSEX_END_WITH_ONE_BYTE,   /*header*/
                                    bin[bin_last_packet_index],             /*byte1*/
                                    0x00,                                   /*byte2*/
                                    0x00                                    /*byte3*/
                                  };
      break;
    case 2:
      sysExResponse_last_packet = { SYSEX_HEADER_SYSEX_END_WITH_TWO_BYTE,   /*header*/
                                    bin[bin_last_packet_index],             /*byte1*/
                                    bin[bin_last_packet_index + 1],         /*byte2*/
                                    0x00                                    /*byte3*/
                                  };
      break;
    default:
      sysExResponse_last_packet = { SYSEX_HEADER_SYSEX_END_WITH_THREE_BYTE,   /*header*/
                                    bin[bin_last_packet_index],             /*byte1*/
                                    bin[bin_last_packet_index + 1],         /*byte2*/
                                    bin[bin_last_packet_index + 2],         /*byte3*/
                                  };
      break;
  }

  MidiUSB.sendMIDI(sysExResponse_last_packet); 
  MidiUSB.flush();
}
