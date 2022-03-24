// Current state of the pressed buttons.
byte currentRead[TOTAL_BUTTONS];
// Temporary input reads to check against current state.
byte tempRead;


void MidiHandler()
{
  for (byte i = 0; i < TOTAL_BUTTONS; i++) {
    // Get the digital state from the button pin.
    // In pull-up inputs the button logic is inverted (HIGH is not pressed, LOW is pressed).
    byte buttonState = digitalRead(BUTTONS_PIN[i]);
    // Temporarily store the digital state.
    tempRead = buttonState;
    // Continue only if the last state is different to the current state.
    if (currentRead[i] != tempRead) {
      // See https://www.arduino.cc/en/pmwiki.php?n=Tutorial/Debounce
      delay(2);
      // Get the pitch mapped to the pressed button.
      byte note = NOTE[i];
      // Save the new input state.
      currentRead[i] = tempRead;
      // Execute note on or noted off depending on the button state.
      if (buttonState == LOW) {
        noteOn(MIDI_CHANNEL, note, VELOCITY[i]);
      } else {
        noteOff(MIDI_CHANNEL, note,  VELOCITY[i]);
      }
    }
  }
}

void noteOn(uint8_t channel, uint8_t pitch, uint8_t velocity) {
  uint8_t midi_command=0x90 | channel;
  midiEventPacket_t noteOn = {0x09, 0x90 , pitch, velocity};
  MIDI.sendNoteOn(pitch, velocity, MIDI_CHANNEL);
}
 
void noteOff(uint8_t channel, uint8_t pitch, uint8_t velocity) {
  uint8_t midi_command=0x80 | channel;
  midiEventPacket_t noteOff = {0x08, 0x80, pitch, 0};
    MIDI.sendNoteOff(pitch, velocity, MIDI_CHANNEL);
}
