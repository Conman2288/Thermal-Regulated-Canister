float setpointValueTemp = 22.8;
int setpointValueAnalog = 12.343 * (setpointValueTemp) + 213.32;
int standardDeviation = 2;
int UCL = (3 * standardDeviation) + setpointValueAnalog;
int LCL = setpointValueAnalog - (3 * standardDeviation);
float temp_UCL = 0.0807 * UCL - 17.117;
float temp_LCL = 0.0807 * LCL - 17.117;
int currentTempAnalog = analogRead(5);
float currentTempDegree = 0.0807 * currentTempAnalog - 17.117;
int fan = 4;
int redRGB = 9;
int blueRGB = 11;
int relay = 12;
boolean fan_status = false;
boolean heater_status = false;
int fromHIGH = setpointValueAnalog + 10;
int fromLOW = setpointValueAnalog - 10;
int toLOW = 0;
int toHIGH = 255;
int PWM;

void fanOn() {
  digitalWrite(fan, HIGH);  //turns fan on
}
void fanOff() {
  digitalWrite(fan, LOW);   //turns fan off
}
void heaterOn() {
  digitalWrite(relay, HIGH);  //turns heater on
}
void heaterOff() {
  digitalWrite(relay, LOW); //turns heater off
}

void setup() {
  Serial.begin(9600); //starts serial monitor
  pinMode(4, OUTPUT);   //output for MOSFET transistor
  pinMode(relay, OUTPUT); //output for relay
}

void loop() {
  int currentTempAnalog = analogRead(5);   //reads in current analog temperature
  float currentTempDegree = 0.0807 * currentTempAnalog - 17.117;
  PWM = map(currentTempAnalog, fromLOW, fromHIGH, toLOW, toHIGH); //maps the analog values to proper RGB range
  analogWrite(redRGB, PWM);
  analogWrite(blueRGB, 256 - PWM);  
  Serial.println("LCLA\tSP\tUCLA\tTempA\tTempC\theater\tfan");
  Serial.print(String(LCL) + "\t" + String(setpointValueAnalog) + "\t" + String(UCL) + "\t" + String(currentTempAnalog) + "\t" + String(currentTempDegree));
  if (fan_status == true && heater_status == false){
    Serial.println("\tOff\tOn");
  }
  if (heater_status == true && fan_status == false){
    Serial.println("\tOn\tOff");
  }
  if (heater_status == false && fan_status == false){
    Serial.println("\tOff\tOff");
  }
  Serial.println("");
  if (currentTempAnalog > UCL) { //if current analog value is greater than UCL, then the fan is turned on, and the heater is turned off
    fanOn();
    heaterOff();
    fan_status = true;
    heater_status = false;
  }
  if (currentTempAnalog <= setpointValueAnalog) { //if current analog value is less than or equal to setpoint, then fan is turned off
    fanOff();
    fan_status = false;
  }
  if (currentTempAnalog < LCL) { //if current analog values is less than LCL, then heater is turned on
    heaterOn();
    heater_status = true;
  }
  if (currentTempAnalog < fromLOW) { //prevents a buffer overflow of data
    currentTempAnalog = fromLOW;
  }
  if (currentTempAnalog > fromHIGH) { //prevents a buffer overflow of data
    currentTempAnalog = fromHIGH;
  }
  
  delay(100);
}
