#include <TFUsbMidi.h>

// Define the pin for the button
const int buttonPin = 2;

// Define the MIDI Note number for the button
const int note = 60; // Middle C

// Track button state to detect changes
bool buttonState = false;

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

    // Initialize button pin
    pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
    // Watch for incoming MIDI packets
    VUsbMidi.refresh();

    // Read button state
    bool newButtonState = digitalRead(buttonPin) == LOW;

    // Send MIDI Note messages based on button state
    if (newButtonState != buttonState) {
        buttonState = newButtonState;
        if (buttonState) {
            VUsbMidi.NoteOn(1, note, 127);
        } else {
            VUsbMidi.NoteOff(1, note);
        }
    }

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
