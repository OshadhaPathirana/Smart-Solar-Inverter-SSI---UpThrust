//sends the reading over serial

void setup() { // this function runs once when the sketch starts up
  Serial.begin(115200);
}

void loop() {

  int x=random(10,50);
  int y=random(10,50);
  Serial.print(1);
  Serial.print("x");
  Serial.print(x);
  Serial.print("x");
  Serial.println(y);
  delay (100); 
  Serial.print(2);
  Serial.print("x");
  Serial.print(x);
  Serial.print("x");
  Serial.println(y);
  delay (100);

}
