#include<Servo.h>
Servo myservo;
#define MAX_DISTANCE 400

int servo_pin = 3;
int uv_trig = 9;
int uv_echo = 10;
int green_pin = 6;
int red_pin = 7;


// int step_delay = 30; 

void setup() {
  Serial.begin(9600);
  myservo.attach(servo_pin);
  pinMode(uv_trig, OUTPUT);
  pinMode(uv_echo, INPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(red_pin, OUTPUT);
}

void loop() {
  for(int i = 15; i <= 165; i ++){ 
    moveAndCheck(i);
  }
  for(int i = 165; i >= 15; i --){
    moveAndCheck(i);
  }
}

void moveAndCheck(int angle) { 
  myservo.write(angle); 
  delay(30); 
  float distance = get_distance();
  Serial.println(distance); 
  if(distance<=150.0){
    digitalWrite(red_pin,HIGH); 
    digitalWrite(green_pin,LOW);
  }
  else{
    digitalWrite(red_pin,LOW);
    digitalWrite(green_pin,HIGH); 
  }
  delay(40);
  }

float get_distance() {
  digitalWrite(uv_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(uv_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(uv_trig, LOW);

  long duration = pulseIn(uv_echo, HIGH, 25000); 
  if (duration == 0) return MAX_DISTANCE;

  float distance_val = duration * 0.034 / 2.0;
  if (distance_val > MAX_DISTANCE) return MAX_DISTANCE;
  return distance_val;
}