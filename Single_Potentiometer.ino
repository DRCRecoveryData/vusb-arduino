#include <TFUsbMidi.h>

// Define the pin for the potentiometer
const int potentiometerPin = A0;

// Define the MIDI Control Change controller number for the potentiometer
const int controller = 10;

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

    // Initialize potentiometer pin
    pinMode(potentiometerPin, INPUT);
}

void loop() {
    // Watch for incoming MIDI packets
    VUsbMidi.refresh();

    // Read potentiometer value
    int potValue = analogRead(potentiometerPin) / 8; // Scale to 0-127

    // Send MIDI Control Change message based on potentiometer value
    VUsbMidi.ControlChange(1, controller, potValue);

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
