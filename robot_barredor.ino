#include <Servo.h>
#include <IRremote.h>

// Motores
const int pinENA = 6;
const int pinIN1 = 8;
const int pinIN2 = 9;
const int pinIN3 = 10;
const int pinIN4 = 12;
const int pinENB = 7;
const int pinMotorIzquierda[3] = { pinENA, pinIN1, pinIN2 }; // el primero es hacia adelante, el segundo hacia atras y el tercero la velocidad de giro
const int pinMotorDerecha[3] = { pinENB, pinIN3, pinIN4 };// el primero es hacia adelante, el segundo hacia atras y el tercero la velocidad de giro
const int speed = 200;      //velocidad de giro

// Sensor ultrasónico
int distance;
const int trigPin = 3;
const int echoPin = 2;

// Servo
Servo myservo;
const int waitTime = 250;
const int pinServo = 5;

// Direcciones
const int adelante = 8;
const int derecha = 6;
const int izquierda = 4;
const int atras = 2;

// Control Remoto
const int RECV_PIN = 9;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);
   pinMode(pinIN1, OUTPUT);
   pinMode(pinIN2, OUTPUT);
   pinMode(pinENA, OUTPUT);
   pinMode(pinIN3, OUTPUT);
   pinMode(pinIN4, OUTPUT);
   pinMode(pinENB, OUTPUT);
   Serial.begin(9600);
   myservo.attach(pinServo); // Define the servo motor output pin 5 (PWM)
   irrecv.enableIRIn();
}


void loop(){
  myservo.write(90); // Pongo el servo mirando para el frente
  //leerTecla(); // manejar desde el tecladito
  //leerEntrada(); // manejar desde conexión serial
  moverse(); //se mueve de forma automática  
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
void derechaUnaRueda(int valor){
      digitalWrite(pinMotorDerecha[2], LOW);
      digitalWrite(pinMotorDerecha[1], HIGH);
      digitalWrite(pinMotorIzquierda[2], HIGH);
      digitalWrite(pinMotorIzquierda[1], HIGH);
      analogWrite(pinMotorIzquierda[0],speed);
      analogWrite(pinMotorDerecha[0], speed);
      delay(valor * 100);
     }
// girar a la izquierda con una sola rueda suponiendo que motor B es el de la derecha y motor A el de la izquierda
void izquierdaUnaRueda(int valor){
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
       delay(waitTime);
       Serial.print ("Distancia Frente: "); // Output distance (unit: cm)
       int distancia = medir();       
       return distancia;
}

int medirIzquierda(){
       myservo.write(5);
       delay(waitTime);
       Serial.print ("Distancia Izquierda: "); // Output distance (unit: cm)
       int distancia = medir();       
       return distancia;
}

int medirDerecha(){
       myservo.write(117);
       delay(waitTime);
       Serial.print ("Distancia Derecha: "); // Output distance (unit: cm)
       int distancia = medir();       
       return distancia;
}

int obtenerDireccion(){
       int direccion, distanciaIzquierda, distanciaDerecha, distanciaFrente = 0;
       int delay_time = 250; // tiempo para que arranque el servo
       distanciaFrente = medirFrente();
       if(distanciaFrente <10){
        detenerse(1);
        retroceder(2);
       }   
       if(distanciaFrente <25){
         detenerse(1);
         distanciaIzquierda = medirIzquierda();
         delay(delay_time);
         distanciaDerecha = medirDerecha();
         delay(delay_time);        
         if(distanciaIzquierda > distanciaDerecha){
          direccion = derecha;
         }
         else{
          direccion = izquierda;
         }        
         if(distanciaIzquierda <10 && distanciaDerecha <10){
          direccion = atras;
         }
       }
       else{
         direccion = adelante;
       }
       return direccion;
}

void leerTecla(){
  if (irrecv.decode(&results)){
      Serial.println(results.value, HEX); //imprime el valor en hexa
      irrecv.resume();
  }
}

void leerEntrada(){
   if (Serial.available()>0){ // si se ingreso algo por teclado en el monitor serie
     String opcion = Serial.readStringUntil('\n');
     if(opcion == "medir"){
      Serial.print("Medir distancia \n");
      medir();
     }     
     if(opcion == "servo"){
      Serial.print("Girar servo \n");
      myservo.write(117);
      delay(waitTime);
      myservo.write(117);
      delay(waitTime);
      myservo.write(90);
     }
     if(opcion == "avanzar"){
      Serial.print("Avanzar\n");
      avanzar(3);
     }
     if(opcion == "izquierda"){
      Serial.print("Girar a la derecha\n");
      girarALaIzquierda(6);
     }
     if(opcion == "derecha"){
      Serial.print("Girar a la derecha\n");
      girarALaDerecha(6);
     }
     if(opcion == "retroceder"){
      Serial.print("Retroceder\n");
      retroceder(3);
     }    
  }
}

void moverse(){
  int proximaDireccion = obtenerDireccion();
  switch(proximaDireccion){
  case atras: retroceder(8);
              girarALaIzquierda(2);
              Serial.print("Retroceder");
              break;
  case derecha: retroceder(1);
                girarALaDerecha(6);
                Serial.print("Derecha");
                break;
  case izquierda: retroceder(1);
                  girarALaIzquierda(6);
                  Serial.print ("Izquierda");
                  break;
  case adelante: avanzar(1);
                 Serial.print("Avanzar");
                 break;
  default:  Serial.print("La dirección ");
            Serial.print(proximaDireccion);
            Serial.print(" no es válida");
            break;

  }
}

