const int Red = 9;
const int Green = 10;
const int Blue = 11;

void setup() {
  // put your setup code here, to run once:
  pinMode(Red, OUTPUT);
  pinMode(Green, OUTPUT);
  pinMode(Blue, OUTPUT);
   
}

void loop() {
  // put your main code here, to run repeatedly:
setColor(255, 255, 255);
}


void setColor(int redValue, int greenValue,  int blueValue) {
  analogWrite(Red, redValue);
  analogWrite(Green,  greenValue);
  analogWrite(Blue, blueValue);
}