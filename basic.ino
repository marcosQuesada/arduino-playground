/*
Adafruit Arduino - Lesson 16. Stepper
 */

#include <Stepper.h>

int in1Pin = 13;
int in2Pin = 12;
int in3Pin = 11;
int in4Pin = 10;

int Echo = 6;
int Trigger = 7;

long pulseTime;
long distance;

int turnSize = 0;
int stepSize = 4;
int back = 1;

// 1/4 reduction One step is 2048
Stepper motor(512, in1Pin, in2Pin, in3Pin, in4Pin);  

void setup()
{
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);

  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
  // this line is for Leonardo's, it delays the serial interface
  // until the terminal window is opened
  while (!Serial);

  Serial.begin(57600);
  motor.setSpeed(20);
}

int previousSize;

void loop()
{ 
 if (Serial.available()) {
    turnSize = Serial.parseInt();
    Serial.print("TurnSize:");
    Serial.println(turnSize);    
    previousSize = turnSize;
  }

  if (turnSize != 0) {
    distance = measureDistance();
    Serial.println(distance);  
    
    if (turnSize > 0) {
      turnSize= turnSize - stepSize;
      motor.step(-stepSize);
    }  
    if (turnSize < 0) {
      turnSize = turnSize + stepSize;
      motor.step(stepSize);
    }
    if (turnSize == 0 ) {
      if (back) {
        back = 0;
      } else {
        back = 1;
        Serial.println("Done");
        return;
      }
      turnSize = -previousSize;
      Serial.println("going back");  
    } 

  }  
  delay(10);
}


long measureDistance() {
  digitalWrite(Trigger,LOW); /* Stabilize sensor*/
  delayMicroseconds(5);

  digitalWrite(Trigger, HIGH); /* envío del pulso ultrasónico*/
  delayMicroseconds(10);

  pulseTime=pulseIn(Echo, HIGH, 30000); /* Función para medir la longitud del pulso entrante. Mide el tiempo que transcurrido entre el envío
   del pulso ultrasónico y cuando el sensor recibe el rebote, es decir: desde que el pin 12 empieza a recibir el rebote, HIGH, hasta que
   deja de hacerlo, LOW, la longitud del pulso entrante*/

  if (pulseTime == 0) {
    pinMode(Echo,OUTPUT);
    digitalWrite(Echo,LOW);
    delay(1);
    pinMode(Echo,INPUT);
    pulseTime = 5882; // means out of range
  }

  return microsecondsToCentimeters(pulseTime);
}  

long microsecondsToCentimeters(long microseconds) {
  // Speed of light is 29.4117647 microseconds by cm
  //return (microseconds / 29.4117647 /2);
  return (microseconds / 29.4117647 /2.-1.697959)/1.257335;
} 

