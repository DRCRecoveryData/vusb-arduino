#include <TFUsbMidi.h>

void setup() {
    Serial.begin(115200);
    Serial.println("Setup");
   
    Serial.println("VUSB setup onmsg callback");
    VUsbMidi.OnMsg(OnMidiMessage);
    
    Serial.println("VUSB begin");
    VUsbMidi.begin(false);
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
  //map ADC 0-1024 value to 0-127 
  int velocity = map(value, 0, 1024, 0, 127);

  //channel, note, velocity
  VUsbMidi.NoteOn(1, 60, velocity);
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
