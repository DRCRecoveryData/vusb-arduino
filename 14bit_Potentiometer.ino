#include <TFUsbMidi.h>

// Define the pins for the three potentiometers
const int potentiometer1Pin = A0;
const int potentiometer2Pin = A1;
const int potentiometer3Pin = A2;

// Define the MIDI NRPN (Non-Registered Parameter Number) numbers for the faders
const int nrpn1_MSB = 98; // Most significant byte of NRPN for fader 1
const int nrpn1_LSB = 99; // Least significant byte of NRPN for fader 1
const int nrpn2_MSB = 100; // Most significant byte of NRPN for fader 2
const int nrpn2_LSB = 101; // Least significant byte of NRPN for fader 2
const int nrpn3_MSB = 102; // Most significant byte of NRPN for fader 3
const int nrpn3_LSB = 103; // Least significant byte of NRPN for fader 3

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

    // Initialize potentiometer pins
    pinMode(potentiometer1Pin, INPUT);
    pinMode(potentiometer2Pin, INPUT);
    pinMode(potentiometer3Pin, INPUT);
}

void loop() {
    // Watch for incoming MIDI packets
    VUsbMidi.refresh();

    // Read potentiometer values
    int potValue1 = analogRead(potentiometer1Pin);
    int potValue2 = analogRead(potentiometer2Pin);
    int potValue3 = analogRead(potentiometer3Pin);

    // Send 14-bit MIDI NRPN messages based on potentiometer values
    VUsbMidi.ControlChange(1, nrpn1_MSB, potValue1 >> 7); // Send most significant byte
    VUsbMidi.ControlChange(1, nrpn1_LSB, potValue1 & 0x7F); // Send least significant byte
    VUsbMidi.ControlChange(1, nrpn2_MSB, potValue2 >> 7);
    VUsbMidi.ControlChange(1, nrpn2_LSB, potValue2 & 0x7F);
    VUsbMidi.ControlChange(1, nrpn3_MSB, potValue3 >> 7);
    VUsbMidi.ControlChange(1, nrpn3_LSB, potValue3 & 0x7F);

    // Small delay to prevent overwhelming the MIDI output
    delay(10);
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
