#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <math.h>

Adafruit_MPU6050 mpu;

const int red = 16;
const int green = 17;
const int blue = 15;

unsigned long lastSensorReadTime = 0;  
unsigned long delayDuration = 200;

int XYArray[2][2] = {{0,0},{0,0}};  // 0: 舊值, 1: 新值
int ledColorArray[6] = {0, 0, 0, 255, 255, 255};
int greenLedArray[6] = {32, 33, 25, 26, 27, 14};

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

const float Vibration = 0.5; // 震動系數

void setup() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);  
  for (int i = 0; i < 6; i++) {
    pinMode(greenLedArray[i], OUTPUT);
  }
  
  Serial.begin(115200);

  if (!mpu.begin()) {
    Serial.println("MPU6050 false！");
    while (1);
  }
  
  Serial.println("MPU6050 ture！");

  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("R！");
}

void loop() {
  unsigned long currentTime = millis();
  led(currentTime);
  
  if (currentTime - lastSensorReadTime >= delayDuration) {
    lastSensorReadTime = currentTime;

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // 舊值更新
    XYArray[0][0] = XYArray[1][0];
    XYArray[0][1] = XYArray[1][1];

    // 新值
    int x = a.acceleration.x * 100; 
    int y = a.acceleration.y * 100;
    XYArray[1][0] = x;
    XYArray[1][1] = y;

    // 計算是否震動
    float xa = XYArray[1][0] - XYArray[0][0];
    float ya = XYArray[1][1] - XYArray[0][1];
    float xyAverage = fabs(xa + ya);

    if (xyAverage > Vibration * 100) {
      ledColor();
    }
  }
}

void ledColor() {
  // 更新舊RGB
  ledColorArray[0] = ledColorArray[3];
  ledColorArray[1] = ledColorArray[4];
  ledColorArray[2] = ledColorArray[5];

  // 新 RGB 隨機
  ledColorArray[3] = random(0, 256);
  ledColorArray[4] = random(0, 256);
  ledColorArray[5] = random(0, 256);

  // RGB變化
  for (int i = 0; i < 3; i++) {
    int* currentValue = (i == 0) ? &redValue : (i == 1) ? &greenValue : &blueValue;
    int diff = ledColorArray[3 + i] - ledColorArray[i];

    if (diff > 0) {
      (*currentValue)++;
    } else if (diff < 0) {
      (*currentValue)--;
    }
  }

  analogWrite(red, redValue);
  analogWrite(green, greenValue);
  analogWrite(blue, blueValue);
}

void led(unsigned long currentTime) {
  float sinA = sin(TWO_PI / 7200 * currentTime);
  float sinB = sin(TWO_PI * currentTime / 7200 + PI / 3); 
  float sinC = sin(TWO_PI * currentTime / 7200 + 2 * PI / 3);

  analogWrite(32, ((sinA + 1) * 100));
  analogWrite(33, ((sinA + 1) * 100));

  analogWrite(25, ((sinB + 1) * 100));
  analogWrite(26, ((sinB + 1) * 100));

  analogWrite(27, ((sinC + 1) * 100));
  analogWrite(14, ((sinC + 1) * 100));
}
