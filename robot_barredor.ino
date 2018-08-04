#include <Servo.h>
#include <IRremote.h>
#include <LiquidCrystal_I2C.h>

// Motores
const int pinENA = 6;
const int pinIN1 = 7;
const int pinIN2 = 8;
const int pinIN3 = 9;
const int pinIN4 = 10;
const int pinENB = 5;
const int pinMotorDerecha[3] = { pinENA, pinIN1, pinIN2 }; // el primero es hacia adelante, el segundo hacia atras y el tercero la velocidad de giro
const int pinMotorIzquierda[3] = { pinENB, pinIN3, pinIN4 };// el primero es hacia adelante, el segundo hacia atras y el tercero la velocidad de giro
const int speed = 130;      //velocidad de giro

// Sensor ultrasónico
int distance;
const int trigPin = 3;
const int echoPin = 2;
const int pinMotorCepillo = 12;

// Servo
Servo myservo;
const int waitTime = 250;
const int pinServo = 4;

// Direcciones
const int adelante = 8;
const int derecha = 6;
const int izquierda = 4;
const int atras = 2;

// Control Remoto
const int RECV_PIN = 13;
IRrecv irrecv(RECV_PIN);
decode_results results;
LiquidCrystal_I2C lcd(0x27, 16, 2);



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
   pinMode(pinMotorCepillo, OUTPUT);
   Serial.begin(9600);
   myservo.attach(pinServo); // Define the servo motor output pin 5 (PWM)
   irrecv.enableIRIn();
   lcd.begin();                      
   lcd.backlight();
   lcd.clear();
}


void loop(){
  myservo.write(90); // Pongo el servo mirando para el frente
  digitalWrite(pinMotorCepillo, HIGH);
  leerTecla(); // manejar desde conexión serial  
  leerEntrada();
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
  imprimirEnLCD("Movimiento:","Avanzar");
  digitalWrite(pinMotorDerecha[1], HIGH);
   digitalWrite(pinMotorDerecha[2], LOW);
   analogWrite(pinMotorDerecha[0], 120);
  //moveForward(pinMotorDerecha);
  moveForward(pinMotorIzquierda);
  
  delay(valor * 100);
}

void detenerse(int valor){
  imprimirEnLCD("Movimiento:","Detenerse");
  fullStop(pinMotorDerecha);
  fullStop(pinMotorIzquierda);
  delay(valor * 100);
}

void retroceder(int valor){
  imprimirEnLCD("Movimiento:","Retroceder");
  moveBackward(pinMotorDerecha);
  moveBackward(pinMotorIzquierda);
  delay(valor * 100);
}

// girar a la derecha con una sola rueda suponiendo que motor B es el de la derecha y motor A el de la izquierda
void derechaUnaRueda(int valor){
      digitalWrite(pinMotorIzquierda[2], LOW);
      digitalWrite(pinMotorIzquierda[1], HIGH);
      digitalWrite(pinMotorDerecha[2], HIGH);
      digitalWrite(pinMotorDerecha[1], HIGH);
      analogWrite(pinMotorDerecha[0],speed);
      analogWrite(pinMotorIzquierda[0], speed);
      delay(valor * 100);
     }
// girar a la izquierda con una sola rueda suponiendo que motor B es el de la derecha y motor A el de la izquierda
void izquierdaUnaRueda(int valor){
      digitalWrite(pinMotorIzquierda[2], HIGH);
      digitalWrite(pinMotorIzquierda[1], HIGH);
      digitalWrite(pinMotorDerecha[2], LOW); // cause the motor(left rear) to operate
      digitalWrite(pinMotorDerecha[1], HIGH);
      analogWrite(pinMotorDerecha[0],speed);
      analogWrite(pinMotorIzquierda[0], speed);
      delay(valor * 100);
}

// con las dos ruedas
void girarALaDerecha(int valor){
      imprimirEnLCD("Movimiento:","Girar ->");
      digitalWrite(pinMotorIzquierda[2], LOW); // cause the motor (right rear) to operate
      digitalWrite(pinMotorIzquierda[1], HIGH);
      digitalWrite(pinMotorDerecha[2], HIGH);
      digitalWrite(pinMotorDerecha[1], LOW); // cause the motor (left front) to operate
      analogWrite(pinMotorDerecha[0],speed);
      analogWrite(pinMotorIzquierda[0], speed);
      delay(valor * 100);

}

//con las dos ruedas
void girarALaIzquierda(int valor){
      imprimirEnLCD("Movimiento:","<- Girar ");
      digitalWrite(pinMotorIzquierda[2], HIGH);
      digitalWrite(pinMotorIzquierda[1], LOW); // cause the motor (right front) to operate
      digitalWrite(pinMotorDerecha[2], LOW); // cause the motor (left rear) to operate
      digitalWrite(pinMotorDerecha[1], HIGH);
      analogWrite(pinMotorDerecha[0],speed);
      analogWrite(pinMotorIzquierda[0], speed);
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
    Serial.print("Distancia: ");
    Serial.println(distancia);
    imprimirEnLCD("Distancia:",(String) distancia);
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
       myservo.write(140);
       delay(500);
       Serial.print ("Distancia Izquierda: "); // Output distance (unit: cm)
       int distancia = medir();       
       return distancia;
}

int medirDerecha(){
       myservo.write(40);
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
       if(distanciaFrente < 35){
         detenerse(1);
         distanciaIzquierda = medirIzquierda();
         delay(delay_time);
         distanciaDerecha = medirDerecha();
         delay(delay_time);        
         if(distanciaIzquierda > distanciaDerecha){
          direccion = izquierda;
         }
         else{
          direccion = derecha;
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
  /*if (irrecv.decode(&results)){
      Serial.println(results.value, HEX); //imprime el valor en hexa
      irrecv.resume();
}*/
   if (irrecv.decode(&results)){
    
    switch(results.value)
    {
      case 0x00FF629D:  avanzar(2);
                        Serial.println("Tecla: Arriba"); 
                        break;
      case 0x00FF22DD: girarALaIzquierda(1);
                       Serial.println("Tecla: Izquierda");    
                       break;
      case 0x00FF02FD: detenerse(2);
                       Serial.println("Tecla: OK");    
                       break;
      case 0x00FFC23D: girarALaDerecha(1);
                       Serial.println("Tecla: Derecha");   
                       break;
      case 0x00FFA857: retroceder(2);
                       Serial.println("Tecla: Abajo"); 
                       break;
      case 0x00FF6897: Serial.println("Tecla: 1");    
                       break;
      case 0x00FF9867: Serial.println("Tecla: 2");    
                       break;
      case 0x0FFB04F: Serial.println("Tecla: 3");    
                       break;
      case 0x00FF30CF: medirIzquierda();
                       Serial.println("Tecla: 4");
                       
                       break;
      case 0x00FF18E7: Serial.println("Tecla: 5");
                       
                       break;
      case 0x00FF7A85: medirDerecha();
                       Serial.println("Tecla: 6");                       
                       break;
      case 0x00FF10EF: Serial.println("Tecla: 7");                       
                       break; 
      case 0x00FF38C7: medir();
                       Serial.println("Tecla: 8");    
                       break;
      case 0x00FF5AA5: Serial.println("Tecla: 9");    
                       break;
      case 0x00FF42BD: Serial.println("Tecla: *");    
                       break;
      case 0x00FF4AB5: 
                       Serial.println("Tecla: 0");    
                       break;
      case 0x00FF52AD: Serial.println("Tecla: #");    
                       break;
    }
    irrecv.resume();
  }  
  //delay(300);
}

void leerEntrada(){
   if (Serial.available()>0){ // si se ingreso algo por teclado en el monitor serie
     String opcion = Serial.readStringUntil('\n');
     Serial.print(opcion);
     if(opcion == "medir"){
      Serial.print("Medir distancia \n");
      medir();
     }
     if(opcion == "medirDerecha"){
      Serial.print("Medir distancia a la derecha \n");
      medirDerecha();
      myservo.write(90);
     }
     if(opcion == "medirIzquierda"){
      Serial.print("Medir distancia a la izquierda \n");
      medirIzquierda();
      myservo.write(90);
     }     
     if(opcion == "avanzar"){
      Serial.print("Avanzar\n");
      avanzar(2);
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
     if(opcion == "parar"){
      Serial.print("Parar\n");
      detenerse(2);
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

void imprimirEnLCD(String linea1, String linea2){
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print(linea1);
   lcd.setCursor(0, 1);
   lcd.print(linea2);   
}

