#include <TFUsbMidi.h>

// Define the pins for the three buttons
const int button1Pin = 2;
const int button2Pin = 3;
const int button3Pin = 4;

// Define the MIDI Note numbers for the buttons
const int note1 = 60; // Middle C
const int note2 = 62; // D
const int note3 = 64; // E

// Track button states to detect changes
bool button1State = false;
bool button2State = false;
bool button3State = false;

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

    // Initialize button pins
    pinMode(button1Pin, INPUT_PULLUP);
    pinMode(button2Pin, INPUT_PULLUP);
    pinMode(button3Pin, INPUT_PULLUP);
}

void loop() {
    // Watch for incoming MIDI packets
    VUsbMidi.refresh();

    // Read button states
    bool newButton1State = digitalRead(button1Pin) == LOW;
    bool newButton2State = digitalRead(button2Pin) == LOW;
    bool newButton3State = digitalRead(button3Pin) == LOW;

    // Send MIDI Note messages based on button states
    if (newButton1State != button1State) {
        button1State = newButton1State;
        if (button1State) {
            VUsbMidi.NoteOn(1, note1, 127);
        } else {
            VUsbMidi.NoteOff(1, note1);
        }
    }

    if (newButton2State != button2State) {
        button2State = newButton2State;
        if (button2State) {
            VUsbMidi.NoteOn(1, note2, 127);
        } else {
            VUsbMidi.NoteOff(1, note2);
        }
    }

    if (newButton3State != button3State) {
        button3State = newButton3State;
        if (button3State) {
            VUsbMidi.NoteOn(1, note3, 127);
        } else {
            VUsbMidi.NoteOff(1, note3);
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
