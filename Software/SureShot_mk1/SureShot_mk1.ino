#include "hardware_conf.h"
#include "Com.h"
#include "USB-MIDI.h"
#include "Midi_variable.h"
#include "EEPROM.h"

USBMIDI_CREATE_DEFAULT_INSTANCE();
bool flgSysExRemain = false;
// The setup function runs once when you press reset or power the board
void setup() {
  // Initialize all the pins as a pull-up input.
  for (byte i = 0; i < TOTAL_BUTTONS; i++) {
    pinMode(BUTTONS_PIN[i], INPUT_PULLUP);
  }
  //Setup Serial port
    Serial.begin(9600);
    
    /*
    myPacketSerial.setStream(&Serial);   
    myPacketSerial.setPacketHandler(&onPacketReceived);
    */
  Eeprom_Handler();
  MIDI.setHandleSystemExclusive(SysExHandler);
  MIDI.begin();
}

// The loop function runs over and over again forever
void loop() {
  MIDI.read();
  MidiHandler();
  //ComHandler();  

}
