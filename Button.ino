#include <TFUsbMidi.h>

// Function declaration for MIDI message callback
void OnMidiMessage(TFMidiMessage msg);

void setup() {
    Serial.begin(115200);
    Serial.println("Setup");

    // Set up VUSB MIDI with a callback for incoming messages
    Serial.println("VUSB setup onmsg callback");
    VUsbMidi.OnMsg(OnMidiMessage);
    
    // Begin VUSB MIDI
    Serial.println("VUSB begin");
    VUsbMidi.begin(false);
}

unsigned long nextSendTime = 0;

void loop() {
    // Watch for incoming MIDI packets
    VUsbMidi.refresh();
  
    // Send MIDI messages every 5 seconds
    if (millis() >= nextSendTime) {
        // Send Note On message: channel 1, note 60, velocity 100
        VUsbMidi.NoteOn(1, 60, 100);
        delay(200);
        // Send Note Off message: channel 1, note 60
        VUsbMidi.NoteOff(1, 60);
        // Send Control Change message: channel 2, controller 10, value 54
        VUsbMidi.ControlChange(2, 10, 54);
        
        // Send raw MIDI message (version 1)
        TFMidiMessage midimsg;
        midimsg.type = TFMidiType::NoteOn;
        midimsg.channel = 1;
        midimsg.data1 = 60; // note
        midimsg.data2 = 50; // velocity
        VUsbMidi.write(midimsg);
        
        // Send raw MIDI message (version 2) using byte buffer
        byte buffer[4];
        buffer[0] = 0x09; // USB MIDI Cable Number and Code Index Number
        buffer[1] = 0x90 | 1; // Note On message type and channel 1
        buffer[2] = 30 & 0x7F; // note (limited to 7 bits)
        buffer[3] = 50 & 0x7F; // velocity (limited to 7 bits)
        VUsbMidi.write(buffer, 4);

        // Set the next send time to 5 seconds from now
        nextSendTime = millis() + 5000;
    }
}

// Callback function to handle incoming MIDI messages
void OnMidiMessage(TFMidiMessage msg) {
    Serial.print("Type: ");
    Serial.print(msg.type);
    Serial.print("\tChannel: ");
    Serial.print(msg.channel);
    Serial.print("\tData1: ");
    Serial.print(msg.data1);
    Serial.print("\tData2: ");
    Serial.println(msg.data2);
}
