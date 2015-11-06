int led = 13;

void setup() {                
  Serial.begin(9600);
  pinMode(led, OUTPUT);     
}

void loop() {
  char c;
  if (Serial.available()) {
    c = Serial.read();
    if (c == '1') {
      digitalWrite(led, HIGH);
      Serial.write("LED on\r\n");
    } else if (c == '0') {
      digitalWrite(led, LOW);
      Serial.write("LED off\r\n");
    } else
      Serial.write("Switch LED on/off using keys '1' and '0'\r\n");
  };
}
