#include <TFUsbMidi.h>

// Define the pins for the three potentiometers
const int potentiometer1Pin = A0;
const int potentiometer2Pin = A1;
const int potentiometer3Pin = A2;

// Define the MIDI Control Change controller numbers for the potentiometers
const int controller1 = 10;
const int controller2 = 11;
const int controller3 = 12;

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
    int potValue1 = analogRead(potentiometer1Pin) / 8; // Scale to 0-127
    int potValue2 = analogRead(potentiometer2Pin) / 8; // Scale to 0-127
    int potValue3 = analogRead(potentiometer3Pin) / 8; // Scale to 0-127

    // Send MIDI Control Change messages based on potentiometer values
    VUsbMidi.ControlChange(1, controller1, potValue1);
    VUsbMidi.ControlChange(1, controller2, potValue2);
    VUsbMidi.ControlChange(1, controller3, potValue3);

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
