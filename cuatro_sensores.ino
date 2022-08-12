// notas y frecuencias
#define DO 4186.01
#define RE 4698.64
#define MI 5274.04
#define FA 5587.66
#define SOL 6271.92
#define LA 7040.00
#define SI 7902.14


int cuerda_1[] = {
  DO, RE
};
int cuerda_2[] = {
  MI, FA
};
int cuerda_3[] = {
  SOL, LA
};
int cuerda_4[] = {
  SI
};

int buzzPin_1 = 22;
int buzzPin_2 = 26;
#include "Adafruit_VL53L0X.h"

// address we will assign if dual sensor is present
#define LOX1_ADDRESS 0x30
#define LOX2_ADDRESS 0x31
#define LOX3_ADDRESS 0X32
#define LOX4_ADDRESS 0X33
// set the pins to shutdown
#define SHT_LOX1 7
#define SHT_LOX2 6
#define SHT_LOX3 5
#define SHT_LOX4 4


// objects for the vl53l0x
Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox4 = Adafruit_VL53L0X();

// this holds the measurement
VL53L0X_RangingMeasurementData_t measure1;
VL53L0X_RangingMeasurementData_t measure2;
VL53L0X_RangingMeasurementData_t measure3;
VL53L0X_RangingMeasurementData_t measure4;
/*
    Reset all sensors by setting all of their XSHUT pins low for delay(10), then set all XSHUT high to bring out of reset
    Keep sensor #1 awake by keeping XSHUT pin high
    Put all other sensors into shutdown by pulling XSHUT pins low
    Initialize sensor #1 with lox.begin(new_i2c_address) Pick any number but 0x29 and it must be under 0x7F. Going with 0x30 to 0x3F is probably OK.
    Keep sensor #1 awake, and now bring sensor #2 out of reset by setting its XSHUT pin high.
    Initialize sensor #2 with lox.begin(new_i2c_address) Pick any number but 0x29 and whatever you set the first sensor to
*/
void setID() {
  // all reset
  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  delay(10);

  // all unreset
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, HIGH);
  digitalWrite(SHT_LOX3, HIGH);
  digitalWrite(SHT_LOX4, HIGH);
  delay(10);

  // activating LOX1 and resetting LOX2
  digitalWrite(SHT_LOX1, HIGH);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);

  // initing LOX1
  if (!lox1.begin(LOX1_ADDRESS)) {
    Serial.println(F("Failed to boot first VL53L0X"));
    while (1);
  }
  delay(10);

  // activating LOX2
  digitalWrite(SHT_LOX2, HIGH);
  delay(10);

  //initing LOX2
  if (!lox2.begin(LOX2_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1);
  }

  // activating LOX3
  digitalWrite(SHT_LOX3, HIGH);
  delay(10);

  //initing LOX3
  if (!lox3.begin(LOX3_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1);
  }

  // activating LOX4
  digitalWrite(SHT_LOX4, HIGH);
  delay(10);

  //initing LOX4
  if (!lox4.begin(LOX4_ADDRESS)) {
    Serial.println(F("Failed to boot second VL53L0X"));
    while (1);
  }
}

void read_dual_sensors() {

  lox1.rangingTest(&measure1, false); // pass in 'true' to get debug data printout!
  lox2.rangingTest(&measure2, false); // pass in 'true' to get debug data printout!
  lox3.rangingTest(&measure3, false); // pass in 'true' to get debug data printout!
  lox4.rangingTest(&measure4, false); // pass in 'true' to get debug data printout!

  // print sensor one reading

}
const int LED_1 = 13;
const int LED_2 = 12;
const int LED_3 = 11;
const int LED_4 = 10;


void setup() {
  Serial.begin(115200);
  pinMode(buzzPin_1, OUTPUT);
  pinMode(buzzPin_2, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  pinMode(SHT_LOX1, OUTPUT);
  pinMode(SHT_LOX2, OUTPUT);
  pinMode(SHT_LOX3, OUTPUT);
  pinMode(SHT_LOX4, OUTPUT);


  Serial.println(F("Shutdown pins inited..."));

  digitalWrite(SHT_LOX1, LOW);
  digitalWrite(SHT_LOX2, LOW);
  digitalWrite(SHT_LOX3, LOW);
  digitalWrite(SHT_LOX4, LOW);
  Serial.println(F("Both in reset mode...(pins are low)"));


  Serial.println(F("Starting..."));
  setID();

}

void loop() {
  read_dual_sensors();
  if (measure1.RangeMilliMeter > 100 || measure2.RangeMilliMeter > 100) {
    if (measure1.RangeMilliMeter < 100 ) {
      if (measure1.RangeMilliMeter < 70 && measure1.RangeMilliMeter > 40) {
        tone(buzzPin_1, cuerda_1[1]);
        Serial.println("DO");
      }
      if (measure1.RangeMilliMeter < 40 && measure1.RangeMilliMeter > 20) {
        tone(buzzPin_1, cuerda_1[0]);
        Serial.println("RE");
      }
    }
    if (measure2.RangeMilliMeter < 100 ) {
      if (measure2.RangeMilliMeter < 70 && measure2.RangeMilliMeter > 40) {
        tone(buzzPin_1, cuerda_2[1]);
        Serial.println("MI");
      }
      if (measure2.RangeMilliMeter < 40 && measure2.RangeMilliMeter > 20) {
        tone(buzzPin_1, cuerda_2[0]);
        Serial.println("FA");
      }
    }
  } else {
    noTone(buzzPin_1);
  }
  if (measure3.RangeMilliMeter > 100 || measure4.RangeMilliMeter > 100) {
    if (measure3.RangeMilliMeter < 100 ) {
      if (measure3.RangeMilliMeter < 70 && measure3.RangeMilliMeter > 40) {
        tone(buzzPin_1, cuerda_3[1]);
        Serial.println("SOL");
      }
      if (measure3.RangeMilliMeter < 40 && measure3.RangeMilliMeter > 20) {
        tone(buzzPin_1, cuerda_3[0]);
        Serial.println("LA");
      }
    }

    if (measure4.RangeMilliMeter < 100 ) {
      if (measure4.RangeMilliMeter < 40 && measure4.RangeMilliMeter > 20) {
        tone(buzzPin_1, cuerda_4[0]);
        Serial.println("SI");
      }
    }
  }else{
    noTone(buzzPin_1);
  }
  delay(1);
}
