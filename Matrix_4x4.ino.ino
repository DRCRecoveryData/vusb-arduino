#include <TFUsbMidi.h>

// Define the number of rows and columns in the matrix
#define NUM_ROWS 4
#define NUM_COLS 4

// Define the Arduino pins connected to the rows and columns
int rowPins[NUM_ROWS] = {2, 3, 4, 5};    // Pins connected to the rows with resistors
int colPins[NUM_COLS] = {6, 7, 8, 9};    // Pins connected to the columns directly

// Define the MIDI channel to use
#define MIDI_CHANNEL 1

// Define the MIDI notes corresponding to each button
int noteMapping[NUM_ROWS][NUM_COLS] = {
  {36, 37, 38, 39}, // Row 1
  {40, 41, 42, 43}, // Row 2
  {44, 45, 46, 47}, // Row 3
  {48, 49, 50, 51}  // Row 4
};

void setup() {
    Serial.begin(115200);
    Serial.println("Setup");

    Serial.println("VUSB setup onmsg callback");
    VUsbMidi.OnMsg(OnMidiMessage);
    
    Serial.println("VUSB begin");
    VUsbMidi.begin(false);

    // Set column pins as inputs with internal pull-up resistors
    for (int col = 0; col < NUM_COLS; col++) {
        pinMode(colPins[col], INPUT_PULLUP);
    }

    // Set row pins as outputs
    for (int row = 0; row < NUM_ROWS; row++) {
        pinMode(rowPins[row], OUTPUT);
        digitalWrite(rowPins[row], HIGH);  // Set initial state to HIGH
    }
}

void loop() {
    // Scan the button matrix
    for (int row = 0; row < NUM_ROWS; row++) {
        // Activate the current row
        digitalWrite(rowPins[row], LOW);

        // Check each column for button presses
        for (int col = 0; col < NUM_COLS; col++) {
            if (digitalRead(colPins[col]) == LOW) {
                // Button pressed, send MIDI message
                int note = noteMapping[row][col];
                sendNoteOn(note);
                delay(50); // Debounce delay
            }
        }

        // Deactivate the current row
        digitalWrite(rowPins[row], HIGH);
    }
}

// Function to send a Note On message
void sendNoteOn(int note) {
    VUsbMidi.NoteOn(MIDI_CHANNEL, note, 100); // MIDI channel, note, velocity
}

// Function to send a Note Off message
void sendNoteOff(int note) {
    VUsbMidi.NoteOff(MIDI_CHANNEL, note); // MIDI channel, note
}

void OnMidiMessage(TFMidiMessage msg) {
    Serial.print(msg.type);
    Serial.print("\t");
    Serial.print(msg.channel);
    Serial.print("\t");
    Serial.print(msg.data1);
    Serial.print("\t");
    Serial.println(msg.data2);
}
