

volatile uint8_t tempBuffer[64];               //受信生データ保存配列
volatile boolean serial_update_flag = false;   //データの受信フラッグ
volatile byte received_data = 0;              //受信データ

/*
void onPacketReceived(const uint8_t* buffer, size_t size) {
  serial_update_flag = true;
  
  for (int i = 0; i < size; i++) {
    tempBuffer[i] = buffer[i];
    Serial.println("a");   
  }
}
*/

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
        Serial.print("array2:\n");
        for ( int index = 0;index < total_length ;index++ )        
         {
            message.write_interface.bin[index] = array[index + SYSEX_RESPONSE_FIXED_START_BYTE_LENGTH];
            
            Serial.print(message.write_interface.bin[index],HEX);
            Serial.print("\n");
         }
      }
  
  if(message.common.byte_length > 0)
  {
    Serial.print("\nresult packet(incl. checksum):");
    for(int index =0; index < message.common.byte_length;index++)
    {
      sprintf(sendPacket, "%02x", message.write_interface.bin[index]);
      Serial.print(sendPacket);
    }
    Serial.print("\n");
  }
 

    if (message.common.byte_length > 0)
    {
      /*calculate checksum*/
      uint8_t checksum_of_packet = message.checksum_interface.bin[message.checksum_interface.byte_length];
      uint8_t calculated_checksum = MessageChecksumCalculation(&message);
      Serial.print("calculated checksum:");
      Serial.print(calculated_checksum);
      Serial.print("\n");
      Serial.print("checksum_of_packet:");
      Serial.print(checksum_of_packet);
      Serial.print("\n");
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
    Serial.print("message->checksum_interface.bin[index]");
    Serial.print(message->checksum_interface.bin[index]);
    sum = sum + (int32_t)message->checksum_interface.bin[index];
  }
  return (sum & 0x7F);
}

/*
void SetResponseChecksum(Response *response)
{
  int32_t sum =0;
  sum = sum + (int32_t)response->common.command;
  for (int i=0; i < (response->common.byte_length); i++)
  {
    sum = sum + (int32_t)response->common.bin[i];
  }
  sum = (sum & 0x7F);
  response->common.bin[response->common.byte_length]=sum;
}
*/

void SendResponseSysExPacket(Response *response)
{
  char sendPacket[30];
  Serial.print("\nresponse->common.byte_length:");
  Serial.print(response->common.byte_length);

  Serial.print("\n");
  uint8_t command_data_length = (SYSEX_RESPONSE_FIXED_BYTE_LENGTH + response->common.byte_length); /*checksum, so +1*/  
  uint8_t bin[command_data_length]; 
  bin[0] = SYSEX_EXCLUSIVE_STATUS;
  bin[1] = SYSEX_ID_UPPER_BYTE;
  bin[2] = SYSEX_ID_MIDDLE_BYTE;
  bin[3] = SYSEX_ID_LOWER_BYTE;
  bin[4] = SYSEX_DESTINATION_HOST;
  bin[5] = SYSEX_PROTOCOL_VERSION;


  Serial.print("command_data_length");
  Serial.print(command_data_length);
  Serial.print("\n");
  for(int i=0; i < command_data_length-1 ;i++)       
  {
    bin[SYSEX_RESPONSE_FIXED_START_BYTE_LENGTH + i] = response->write_interface.bin[i];
  }
  bin[command_data_length-1] = SYSEX_END_OF_EXCLUSIVE;   /*last byte of sysex is command_data_length (last byte of command data + 1)*/

    Serial.print("send bin:");
  for(int i=0; i < command_data_length ;i++)   
  {
    sprintf(sendPacket, "%02X", bin[i]);
    Serial.print(sendPacket);
  }

  /*separate data bytes to midiEventPacket_t*/
  uint8_t last_packet_byte_number = command_data_length%3;
  uint8_t packet_number;
  uint8_t remainder_byte = 0; 
  
  if(last_packet_byte_number > 0)
  {
    remainder_byte = 1;
  }
  packet_number = (command_data_length/3) + remainder_byte;
  
  Serial.print("packet_number");
  Serial.print(packet_number);
  Serial.print("\n");

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
    Serial.print("Index:");
    Serial.print(i);
    Serial.print("\n");
    Serial.print("header:");
    Serial.print(sysExResponse.header,HEX);
    Serial.print("\n");

    Serial.print("byte1:");
    Serial.print(sysExResponse.byte1,HEX);
    Serial.print("\n");

    Serial.print("byte2:");
    Serial.print(sysExResponse.byte2,HEX);
    Serial.print("\n");

    Serial.print("byte3:");
    Serial.print(sysExResponse.byte3,HEX);
    Serial.print("\n");
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

    Serial.print("header:");
    Serial.print(sysExResponse_last_packet.header,HEX);
    Serial.print("\n");

    Serial.print("byte1:");
    Serial.print(sysExResponse_last_packet.byte1,HEX);
    Serial.print("\n");

    Serial.print("byte2:");
    Serial.print(sysExResponse_last_packet.byte2,HEX);
    Serial.print("\n");

    Serial.print("byte3:");
    Serial.print(sysExResponse_last_packet.byte3,HEX);
    Serial.print("\n");
  MidiUSB.sendMIDI(sysExResponse_last_packet); 
  MidiUSB.flush();
}
