/**
 * @Authors
 * Adrian Suberville Perulero
 * Omar Olea Mayo
 * Samanta Riso Zuñiga
 * Rodrigo Maldonado Soriano
 * Netzai Diaz Murgo
 * Irving Cruz Suastegui
 * Osvaldo Cruz Suastegui
 * Samuel Carrillo
**/

// Libraries
#include <SoftwareSerial.h>
#include <Wire.h>

#define MAX_TMP 500
// Variables globales
#define ECHO 11
#define TRIGG 12
 
// PUERTO SERIE EMULADO: EL ARDUINO UNO TIENE SOLO 1 UART
SoftwareSerial mySerial(8, 9); //SIM800L Tx & Rx is connected to Arduino #8 & #9

// SDA : A4, SCL : A5
 
// Bombas selenoides
int BS1 = 2;
int BS2 = 3;
int BS3 = 4;
int BS4 = 5;

// Bomba
int BOMBA = 6;


void setup() {
  Serial.begin(9600); // Este nos habilita el monitor serie para poder visualizar los datos.
  for(int i = 2; i < 7; i++) pinMode(i,OUTPUT); // Programamos la variable Relevador como puerto de salida.
  initUltrasonico();
}

void loop() {
  Start();
  delay(500);                        // Dejamos pasar un tiempo poara que vuelva a tomar la lectura.
}

bool bomba = false, linea1 = false, linea2 = false, linea3 = false, linea4 = false;
void runSensorHumedad(){
  if(console(analogRead(A6)) || console(analogRead(A7)) 
  || console(analogRead(A8)) || console(analogRead(A9)) 
  || console(analogRead(A10)) || console(analogRead(A11))
  || console(analogRead(A12)) || console(analogRead(A13))){
    
    if(console(analogRead(A6)) || console(analogRead(A7)))  linea1 = true; else linea1 = false;
    if(console(analogRead(A8)) || console(analogRead(A9))) linea2 = true; else linea2 = false;
    if(console(analogRead(A10)) || console(analogRead(A11))) linea3 = true; else linea3 = false;
    if(console(analogRead(A12)) || console(analogRead(A13))) linea4 = true; else linea4 = false;
    bomba = true;
  } else{
      bomba = false;
  }
  if(bomba) digitalWrite(BOMBA,HIGH); else digitalWrite(BOMBA,LOW);
  if(linea1) digitalWrite(BS1, HIGH); else digitalWrite(BS1, LOW); // Linea 1
  if(linea2) digitalWrite(BS2, HIGH); else digitalWrite(BS2, LOW); // Linea 2
  if(linea3) digitalWrite(BS3, HIGH); else digitalWrite(BS3, LOW); // Linea 3
  if(linea4) digitalWrite(BS4, HIGH); else digitalWrite(BS4, LOW); // Linea 4
} 

void ApagarBS(){
  bomba = false;
  linea1 = false;
  linea2 = false;
  linea3 = false;
  linea4 = false;
  for(int i = 2; i < 7; i++) digitalWrite(i,LOW);
}

boolean console(uint8_t pinA){ // Metodo para debugear la salida del sensor de humedad retorna el % de humedad
  int value = analogRead(pinA);
  Serial.print("Debug: ");
  Serial.print(value);
  Serial.println(" humedad");
  return value >= MAX_TMP;
}


void initUltrasonico(){
  pinMode(ECHO, INPUT);
  pinMode(TRIGG, OUTPUT);
}

bool runStateUltrasonico(){
  #define MAX_PROFUNDIDAD 50
  unsigned int tiempo, distancia;
  digitalWrite(TRIGG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGG, LOW);
  tiempo = pulseIn(ECHO, HIGH);
  distancia = tiempo / 58;
  float nivel = (100 - (distancia*100)/MAX_PROFUNDIDAD);
  Serial.print("Nivel de agua: ");
  Serial.print(nivel);
  Serial.println(" %");
  if (nivel > 10) {
    Serial.println("Aun hay agua");
    return true;
  } else {
    Serial.println("No hay agua");
    return false;
  }
}

void Start(){
  if(runStateUltrasonico()) runSensorHumedad(); else ApagarBS();
  Serial.println("----------------------------------");
}
