

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

void ComHandler()
{
    Message message;

    for (int i=0; i <22; i++)
    {
      message.bin[i]=0x00;
    }

    /*waiting 0xFF*/
    uint8_t serial_buffer=(uint8_t)Serial.read();
    
    if  (serial_buffer == HEADER_MESSAGE_FIRST)
    {
      /*check whether second byte is 0x77*/
      message.common.header0=serial_buffer;
      message.common.header1=(uint8_t)Serial.read();
      if(message.common.header1 == HEADER_MESSAGE_SECCOND)    /* 0xff 0x77 -> It's nitro protocol*/
      {
        message.common.mode =  (uint8_t)Serial.read();
        message.common.command =  (uint8_t)Serial.read();
        message.common.byte_length = (uint8_t)Serial.read();
        
        for (int i=0; i < (message.common.byte_length+1); i++)
        {
          message.common.bin[i] = (uint8_t)Serial.read();
        }

        /*calculate checksum*/
        uint8_t checksum_of_packet = message.common.bin[message.common.byte_length];
        uint8_t calculated_checksum = MessageChecksumCalculation(&message);
        if( (message.common.mode==MODE_SURESHOT)
          && (checksum_of_packet==calculated_checksum)
          )
        {
          Response response;
          /*initialize response*/
          for (int i=0; i <22; i++)
          {
            response.bin[i]=0x00;
          }

          switch(message.common.command)
          {
            case cmd_request_protocol_version:
              request_protocol_version(&message, &response);
              break;
  
            case cmd_play_request:
              play_request(&message, &response);
              break;
  
            case cmd_read_midi_channel:
              read_midi_channel(&message, &response);
              break;
  
            case cmd_write_midi_channel:
              write_midi_channel(&message, &response);
              break;
  
            case cmd_read_note:
              read_note(&message, &response);
              break;
  
            case cmd_write_single_note:
              write_single_note(&message, &response);
              break;
  
            case cmd_write_all_note:
              write_all_note(&message, &response);
              break;
  
            case cmd_read_velocity:
              read_velocity(&message, &response);
              break;
  
            case cmd_write_single_velocity:
              write_single_velocity(&message, &response);
              break;
  
            case cmd_write_all_velocity:
              write_all_velocity(&message, &response);
              break;
  
            case cmd_write_eeprom:
              write_eeprom(&message, &response);;
              break;
            
          }
          /*Send response via serial port*/
          SetResponseFixedPacket(&response);
          SetResponseChecksum(&response);
          uint8_t sendlength = response.common.byte_length + FIXED_BYTE_LENGTH;
          for(int i=0;i<sendlength;i++)
          {
            Serial.write(response.bin[i]);
          }
        }
      }
    }
}

uint8_t MessageChecksumCalculation(Message *message)
{
  int32_t sum =0;
      sum = sum + (int32_t)message->common.mode;
      sum = sum + (int32_t)message->common.command;
      sum = sum + (int32_t)message->common.byte_length;
      for (int i=0; i < (message->common.byte_length); i++)
      {
        sum = sum + (int32_t)message->common.bin[i];
      }
  return (sum & 0xFF);
}
void SetResponseChecksum(Response *response)
{
  int32_t sum =0;
  sum = sum + (int32_t)response->common.mode;
  sum = sum + (int32_t)response->common.command;
  sum = sum + (int32_t)response->common.byte_length;
  for (int i=0; i < (response->common.byte_length); i++)
  {
    sum = sum + (int32_t)response->common.bin[i];
  }
  response->common.bin[response->common.byte_length]=sum;
}


void SetResponseFixedPacket(Response *response)
{
  response->common.header0 = 0xFF;
  response->common.header1 = 0xB7;
  response->common.mode = 0x00;
}
