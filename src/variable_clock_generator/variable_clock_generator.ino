/* -----------------------------------------------------------------
    Clock generator sketch for use with my Homemade CPU

    https://github.com/Andy4495/Homemade-CPU

    25-Oct-2025 Andy4495  Original

*/


unsigned int freq = 50;

void setup() {
  pinMode(3, OUTPUT);
  Serial.begin(9600);
  Serial.println("Initial frequency: 50");
  tone(3, freq);
}

void loop() {
  if (Serial.available()) {
    freq = Serial.parseInt();
    if (freq != 0) {
      Serial.print("New frequency: ");
      Serial.println(freq);
      tone(3, freq);
      Serial.println("Enter frequency: ");
    }
  }
}
