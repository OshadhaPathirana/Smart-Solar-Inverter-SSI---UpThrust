float solar_volt = 0;
float bat_volt = 0;
float sample1 = 0;   // solar voltage input pin A0
float sample2 = 0;   // battery voltage input pin A1
int iot_input = 3;
int pwm = 6;
int load = 9;
int charged_percent = 0;
int RED = 7;
int GREEN = 8;

float avg_charge = 12.5;
float max_charge = 14.0;
float low_charge = 11.0;
float dead_charge = 10.5;


void setup()
{
  TCCR0B = TCCR0B & 0b11111000 | 0x05; // setting prescalar for 61.03Hz pwm
  Serial.begin(9600);
  pinMode(pwm, OUTPUT);
  pinMode(load, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(iot_input, INPUT);
  digitalWrite(pwm, LOW);
  digitalWrite(load, LOW);
  digitalWrite(RED, LOW);
  digitalWrite(RED, LOW);

}
void loop()
{
  //****************************************  VOLTAGE MEASURING ****************************************
  for (int i = 0; i < 150; i++)
  {
    sample1 += analogRead(A0); // solar panel input voltage
    sample2 += analogRead(A1); // battery voltage
    delay(2);
  }
  sample1 = sample1 / 150; // averaged values
  sample2 = sample2 / 150;

  //**************************************** CALCULATIONS OF VOLTAGE MEASUREMENTS ****************************************

  solar_volt = (sample1 * 4.673 * 3.127) / 1000;  // Needs to be calibrated according to the resistor values of voltage devider
  bat_volt = (sample2 * 4.673 * 3.127) / 1000;    // Needs to be calibrated according to the resistor values of voltage devider
  Serial.print("solar input voltage :");
  Serial.println(solar_volt);
  Serial.print("battery voltage :");
  Serial.println(bat_volt);

  // ****************************************PWM CHARGING ****************************************
  // As 12v battery is gradually charged the charging rate (pwm duty cycle) is decreased
  // 14v = fully charged(100%)
  // 11v =fully discharged(0%)
  // when battery voltage is less than 11v, trun on RED LED

  if ((solar_volt > bat_volt) && ( bat_volt <= avg_charge ))
  {
    analogWrite(pwm, 242.25); // @ 95% duty cycle // fast charging mode
    Serial.print("pwm duty cycle is :");
    Serial.println("95%");
  }
  else if ((solar_volt > bat_volt) && (bat_volt > avg_charge) && (bat_volt <= max_charge ))
  {
    analogWrite(pwm, 25.5); // 10% duty cycle // slow charging mode
    Serial.print("pwm duty cycle is :");
    Serial.println("10%");
  }

  // shut down the charging process when battery is fully charged or solar input is low/off
  else if ((bat_volt > max_charge) or (solar_volt < bat_volt))
  {
    analogWrite(pwm, 0);
    Serial.print("pwm duty cycle is :");
    Serial.println("0%");
    digitalWrite(GREEN, LOW); // charging LED(green) will be truned off
  }

  /////////////////////////////  LOAD  ///////////////////////////////////////////////////////
  if ((digitalRead(iot_input)==HIGH ) && (bat_volt > avg_charge))
  {
    digitalWrite(load, HIGH); //turn on the inverter
  }

  //*************************************LED INDICATIONS OF CHARGING**************************************

  if (  solar_volt > bat_volt && bat_volt < max_charge)
  {
    ///Green LED blinks continiously while charging
    digitalWrite(GREEN, HIGH);
    delay(20);
    digitalWrite(GREEN, LOW);
    delay(20);
  }

  if (bat_volt < low_charge && bat_volt > dead_charge)
  {
    digitalWrite(RED, HIGH); // indicating battery is discharged
  }

  if (bat_volt > low_charge)
  {
    digitalWrite(RED, LOW); // indicating battery is not discharged
  }

  if (bat_volt >= max_charge)
  {
    digitalWrite(GREEN, HIGH); //Fully charged
  }

}
