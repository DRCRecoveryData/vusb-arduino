#include <TFUsbMidi.h>

void setup() {
    Serial.begin(115200);
    Serial.println("Setup");
   
    Serial.println("VUSB setup onmsg callback");
    VUsbMidi.OnMsg(OnMidiMessage);
    
    Serial.println("VUSB begin");
    VUsbMidi.begin(false);

    analogReadResolution(14); // Set ADC resolution to 14 bits
}

unsigned long ms;
int potmeter = A0; // Potentiometer input pin

int potmeter_last_value = 0;

void loop() {
    //watch for midi packets
    VUsbMidi.refresh();

    int tmp;
    tmp = analogRead(potmeter);
    if (tmp != potmeter_last_value) {
        OnPotmeterValueChanged(tmp);
        potmeter_last_value = tmp;
        Serial.println("Potmeter value changed to " + String(tmp));
    }
}

void OnPotmeterValueChanged(int value) { 
    //map ADC 0-16383 value to 0-127 
    int velocity_msb = map(value, 0, 16383, 0, 127);
    int velocity_lsb = 0;

    // Calculate the LSB value for 14-bit resolution
    if (velocity_msb > 0) {
        velocity_lsb = map(value % velocity_msb, 0, velocity_msb, 0, 127);
    }

    //channel, controller number (MSB), value (MSB)
    VUsbMidi.CtrlChange(1, 0, velocity_msb);

    //channel, controller number (LSB), value (LSB)
    VUsbMidi.CtrlChange(1, 32, velocity_lsb);
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
