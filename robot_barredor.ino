#include <Servo.h>

// Motores
const int pinENA = 6;
const int pinIN1 = 8;
const int pinIN2 = 9;
const int pinIN3 = 10;
const int pinIN4 = 12;
const int pinENB = 7;
const int pinMotorIzquierda[3] = { pinENA, pinIN1, pinIN2 }; // el primero es hacia adelante, el segundo hacia atras y el tercero la velocidad de giro
const int pinMotorDerecha[3] = { pinENB, pinIN3, pinIN4 };// el primero es hacia adelante, el segundo hacia atras y el tercero la velocidad de giro
const int waitTime = 1000;   //espera entre fases
const int speed = 200;      //velocidad de giro

// Sensor ultrasónico
int distance;
const int trigPin = 3;
const int echoPin = 2;

Servo myservo; // set myservo


void setup()
{
   pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
   pinMode(echoPin, INPUT);
   pinMode(pinIN1, OUTPUT);
   pinMode(pinIN2, OUTPUT);
   pinMode(pinENA, OUTPUT);
   pinMode(pinIN3, OUTPUT);
   pinMode(pinIN4, OUTPUT);
   pinMode(pinENB, OUTPUT);
   Serial.begin(9600);
   myservo.attach (5); // Define the servo motor output pin 5 (PWM)
}
 
void loop()
{
  avanzar(10);
  distance = medir();
    if(distance < 6){
      detenerse(10);
    }
    avanzar(10);
}
 
void moveForward(const int pinMotor[3]){
   digitalWrite(pinMotor[1], HIGH);
   digitalWrite(pinMotor[2], LOW);
   analogWrite(pinMotor[0], speed);
}
 
void moveBackward(const int pinMotor[3]){
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], HIGH);
   analogWrite(pinMotor[0], speed);
}

void fullStop(const int pinMotor[3]){
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], LOW);
   analogWrite(pinMotor[0], 0);
}

void avanzar(int valor){
  moveForward(pinMotorIzquierda);
  moveForward(pinMotorDerecha);
  delay(valor * 100);
}

void detenerse(int valor){
  fullStop(pinMotorIzquierda);
  fullStop(pinMotorDerecha);
  delay(valor * 10);
}

void retroceder(int valor){
  moveBackward(pinMotorIzquierda);
  moveBackward(pinMotorDerecha);
  delay(valor * 100);
}

// girar a la derecha con una sola rueda suponiendo que motor B es el de la derecha y motor A el de la izquierda
void derecha(int valor){
      digitalWrite(pinMotorDerecha[2], LOW);
      digitalWrite(pinMotorDerecha[1], HIGH);
      digitalWrite(pinMotorIzquierda[2], HIGH);
      digitalWrite(pinMotorIzquierda[1], HIGH);
      analogWrite(pinMotorIzquierda[0],speed);
      analogWrite(pinMotorDerecha[0], speed);
      delay(valor * 100);
     }
// girar a la izquierda con una sola rueda suponiendo que motor B es el de la derecha y motor A el de la izquierda
void izquierda(int valor){
      digitalWrite(pinMotorDerecha[2], HIGH);
      digitalWrite(pinMotorDerecha[1], HIGH);
      digitalWrite(pinMotorIzquierda[2], LOW); // cause the motor(left rear) to operate
      digitalWrite(pinMotorIzquierda[1], HIGH);
      analogWrite(pinMotorIzquierda[0],speed);
      analogWrite(pinMotorDerecha[0], speed);
      delay(valor * 100);
}

// con las dos ruedas
void girarALaDerecha(int valor){
      digitalWrite(pinMotorDerecha[2], LOW); // cause the motor (right rear) to operate
      digitalWrite(pinMotorDerecha[1], HIGH);
      digitalWrite(pinMotorIzquierda[2], HIGH);
      digitalWrite(pinMotorDerecha[1], LOW); // cause the motor (left front) to operate
      analogWrite(pinMotorIzquierda[0],speed);
      analogWrite(pinMotorDerecha[0], speed);
      delay(valor * 100);

}

//con las dos ruedas
void girarALaIzquierda(int valor){
      digitalWrite(pinMotorDerecha[2], HIGH);
      digitalWrite(pinMotorDerecha[1], LOW); // cause the motor (right front) to operate
      digitalWrite(pinMotorIzquierda[2], LOW); // cause the motor (left rear) to operate
      digitalWrite(pinMotorIzquierda[1], HIGH);
      analogWrite(pinMotorIzquierda[0],speed);
      analogWrite(pinMotorDerecha[0], speed);
      delay(valor * 100);
     }

int medir(){
    long duracion;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duracion = pulseIn(echoPin, HIGH);
    int distancia= duracion*0.034/2;
    //Serial.print("Distance: ");
    Serial.println(distancia);
    return distancia;
}

int medirFrente(){
       myservo.write(90);
       Serial.print ("Distancia Frente: "); // Output distance (unit: cm)
       int distancia = medir();       
       return distancia;
}

int medirIzquierda(){
       myservo.write(5);
       Serial.print ("Distancia Izquierda: "); // Output distance (unit: cm)
       int distancia = medir();       
       return distancia;
}

int medirDerecha(){
       myservo.write(5);
       Serial.print ("Distancia Derecha: "); // Output distance (unit: cm)
       int distancia = medir();       
       return distancia;
}
