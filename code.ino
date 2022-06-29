/*
 * Arduino Turbidity Meter, Water Quality monitoring
 * Download Libraries
 * https://www.electroniclinic.com/arduino-libraries-download-and-projects-they-are-used-in-project-codes/
 * 
 */
#include <Wire.h> 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h> // Library for LCD

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

int Turbidity_Sensor_Pin = A0;
int Temperature_Sensor_Pin = 13;
int SyncButton = A1;
int buttonState = 0;

float Turbidity_Sensor_Voltage;
int samples = 10000;
float ntu;

float tempCelsius;
float tempFahrenheit;

int setButton = 0;

float JERNIH = 2.80;
float KERUH = 2.78;

bool status = false;

OneWire oneWire(Temperature_Sensor_Pin);
DallasTemperature tempSensor(&oneWire);
 
void setup()
{
  Serial.begin(9600); // for debugging purposes

  lcd.init(); //initialize the lcd
  lcd.backlight(); //open the backlight 

  tempSensor.begin();
  
  pinMode(Turbidity_Sensor_Pin, INPUT);
  pinMode(SyncButton, INPUT);
  lcd.setCursor(0, 0);         // move cursor to   (0, 0)
  lcd.print("Dwi Wardana");        // print message at (0, 0)
  lcd.setCursor(0, 1);         // move cursor to   (2, 1)
  lcd.print("Ivan Kurniawan");
}
 
void loop() {
  delay(2000);
    if (!status){
      lcd.clear();
    }
    tempSensor.requestTemperatures();
    tempCelsius = tempSensor.getTempCByIndex(0);
    
    Serial.print("Temperature Air: ");
    Serial.print(tempCelsius);
    Serial.println("°C");
    
    Turbidity_Sensor_Voltage = 0;
    for(int i=0; i<samples; i++)
    {
      Turbidity_Sensor_Voltage += ((float)analogRead(Turbidity_Sensor_Pin)/1023)*5;
    }
    
    Turbidity_Sensor_Voltage = Turbidity_Sensor_Voltage/samples;
    Turbidity_Sensor_Voltage = round_to_dp(Turbidity_Sensor_Voltage,2);

    buttonState = analogRead(SyncButton);
    if (!(buttonState) == HIGH) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Mengubah");
        lcd.setCursor(0, 1);
        lcd.print("Parameter Air");
        JERNIH = Turbidity_Sensor_Voltage-0.04;
        KERUH = JERNIH - 0.03;
        delay(2000);
        lcd.clear();
    }

    if (Turbidity_Sensor_Voltage >= JERNIH) {
      lcd.setCursor(0,0);
      lcd.print("Air Jernih");
    } else if (Turbidity_Sensor_Voltage >= KERUH && Turbidity_Sensor_Voltage < JERNIH) {
      lcd.setCursor(0,0);
      lcd.print("Air Keruh");
    } else {
      lcd.setCursor(0,0);
      lcd.print("Air Sangat Keruh");
    }

    lcd.setCursor(0,1);
    lcd.print("Temp ");
    lcd.print(tempCelsius);
    lcd.print("C");

    delay(10);
}
 
float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}
