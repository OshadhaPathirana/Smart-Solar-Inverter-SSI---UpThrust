int duty_cycle = 127; // 50% duty cycle

void setup() {

  TCCR0B = TCCR0B & B11111000 | B00000101; // set PWM frequency to 61.04 Hz
  pinMode(3, OUTPUT);


}

void loop() {
    analogWrite(3, 150);
  

}
