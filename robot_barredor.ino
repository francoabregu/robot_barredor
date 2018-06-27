const int pinENA = 6;
const int pinIN1 = 8;
const int pinIN2 = 9;
const int pinIN3 = 10;
const int pinIN4 = 12;
const int pinENB = 7;

int distance;

const int trigPin = 3;
const int echoPin = 2;
 
const int waitTime = 1000;   //espera entre fases
const int speed = 200;      //velocidad de giro
 
const int pinMotorA[3] = { pinENA, pinIN1, pinIN2 };
const int pinMotorB[3] = { pinENB, pinIN3, pinIN4 };
 
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
}
 
void loop()
{
  avanzar(speed);
  distance = medir();
    if(distance < 6){
      fullStop(pinMotorA);
      fullStop(pinMotorB);
    }
   delay(waitTime);
  moveForward(pinMotorA, speed);
  moveForward(pinMotorB, speed);
delay(waitTime);
 
   //fullStop(pinMotorA);
   //fullStop(pinMotorB);
   //delay(waitTime);
}
 
void moveForward(const int pinMotor[3], int speed){
   digitalWrite(pinMotor[1], HIGH);
   digitalWrite(pinMotor[2], LOW);
   analogWrite(pinMotor[0], speed);
}
 
void moveBackward(const int pinMotor[3], int speed){
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], HIGH);
   analogWrite(pinMotor[0], speed);
}

void fullStop(const int pinMotor[3]){
   digitalWrite(pinMotor[1], LOW);
   digitalWrite(pinMotor[2], LOW);
   analogWrite(pinMotor[0], 0);
}

void avanzar(int speed){
  moveForward(pinMotorA, speed);
  moveForward(pinMotorB, speed);
  delay(waitTime);
}

void retroceder(int speed){
  moveBackward(pinMotorA, speed);
  moveBackward(pinMotorB, speed);
}

// girar a la derecha con una sola rueda suponiendo que motor B es el de la derecha y motor A el de la izquierda
void derecha(int speed){
      digitalWrite (pinMotorB[2], LOW);
      digitalWrite (pinMotorB[1], HIGH);
      digitalWrite (pinMotorA[2], HIGH);
      digitalWrite (pinMotorA[1], HIGH);
      analogWrite(pinMotorA[0],speed);
      analogWrite(pinMotorB[0], speed);
      delay(waitTime);
     }
// girar a la izquierda con una sola rueda suponiendo que motor B es el de la derecha y motor A el de la izquierda
void izquierda(int speed){
      digitalWrite (pinMotorB[2], HIGH);
      digitalWrite (pinMotorB[1], HIGH);
      digitalWrite (pinMotorA[2], LOW); // cause the motor (left rear) to operate
      digitalWrite (pinMotorA[1], HIGH);
      analogWrite(pinMotorA[0],speed);
      analogWrite(pinMotorB[0], speed);
      delay (waitTime);
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
    Serial.print("Distance: ");
    Serial.println(distancia);
    return distancia;
}


