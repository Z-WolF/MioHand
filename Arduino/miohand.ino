#include<SoftwareSerial.h>

/*stby siempre activo: stby = 10kOhm 3,3V
stby = */

/* direccion CW = 1 direccion con el tiempo; CCW = 1 direccion contra tiempo. 
CW y CCW = 1 short brake CW y CCW = 0 brake */

const int dirCW_indice = 4;
const int dirCCW_indice = 8;
const int dirCW_medio = 12;
const int dirCCW_medio = 13;
const int dirCW_pulgar = 14;
const int dirCCW_pulgar = 15;
const int dirCW_pulgar_dos = 16;
const int dirCCW_pulgar_dos = 17;
const int dirCW_minique_anular = 18;
const int dirCCW_minique_anular = 19;

/*5 interrupt pins encoder, declarar como interrupt en setup*/

const int indice = 1;
const int medio = 0;
const int pulgar = 2;
const int pulgar_dos = 3;
const int minique_anular = 7;

/*pwm pins aplicar PWM uC*/

const int pwm_pulgar = 5;
const int pwm_pulgar_dos = 6;
const int pwm_indice = 9;
const int pwm_medio = 10;
const int pwm_minique_anular = 11;

/*serial input USB valores del raspberry*/

byte comm_rasp;
byte comm_rasp_raw;

/*variables que se ocupan en el sketch*/

long rotacion_pulgar = 0;
long rotacion_pulgar_dos = 0;
long rotacion_indice = 0;
long rotacion_medio = 0;
long rotacion_minique_anular = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //declarar pins PWM como Output:
  pinMode(pwm_pulgar, OUTPUT);
  pinMode(pwm_pulgar_dos, OUTPUT);
  pinMode(pwm_indice, OUTPUT);
  pinMode(pwm_medio, OUTPUT);
  pinMode(pwm_minique_anular, OUTPUT);
  //todo pwm 0
  analogWrite(pwm_pulgar, 0);
  analogWrite(pwm_pulgar_dos, 0);
  analogWrite(pwm_indice, 0);
  analogWrite(pwm_medio, 0);
  analogWrite(pwm_minique_anular, 0);
  //declarar pins direccion como Output:
  pinMode(dirCW_pulgar, OUTPUT);
  pinMode(dirCCW_pulgar, OUTPUT);
  pinMode(dirCW_pulgar_dos, OUTPUT);
  pinMode(dirCCW_pulgar_dos, OUTPUT);
  pinMode(dirCW_indice, OUTPUT);
  pinMode(dirCCW_indice, OUTPUT);
  pinMode(dirCW_medio, OUTPUT);
  pinMode(dirCCW_medio, OUTPUT);
  pinMode(dirCW_minique_anular, OUTPUT);
  pinMode(dirCCW_minique_anular, OUTPUT);
  //declarar pins encoders
  pinMode(pulgar, INPUT);  
  pinMode(pulgar_dos, INPUT);
  pinMode(indice, INPUT);  
  pinMode(medio, INPUT);
  pinMode(minique_anular, INPUT);
  //declarar pins Interrupts
  attachInterrupt(digitalPinToInterrupt(pulgar), enc_pulgar, FALLING);
  attachInterrupt(digitalPinToInterrupt(pulgar_dos), enc_pulgar_dos, FALLING);
  attachInterrupt(digitalPinToInterrupt(indice), enc_indice, FALLING);
  attachInterrupt(digitalPinToInterrupt(medio), enc_medio, FALLING);
  attachInterrupt(digitalPinToInterrupt(minique_anular), enc_minique_anular, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Communicacion con el raspberry
  serial_comm();
  //Si llega un 1 significa pinzabidigital
  switch(comm_rasp){
  case 49:
    pinzabidigital();
    break;
  //Si llega un 2...
  case 50:
    pinzatripode();
    break;
  //Si llega un 3...
  case 51:
    semiflexion_indice();
    break;
  //Si llega un 4...
  case 52:
    presapalmar();
    break;
  //Si llega un 5...
  case 53:
    extension();
    break; 
  //Si llega un 6...
  case 54:
    flexion();
    break;
  //Si llega un 7...
  case 55:
    stopp();
    break;
  }
}

//Serial comunicacion
void serial_comm(){
    if( Serial.available() ){
    comm_rasp_raw = Serial.read();
    if(comm_rasp_raw <= 55 && comm_rasp_raw >= 49){
      comm_rasp = comm_rasp_raw;
    }
    Serial.print("Recibido: ");
    Serial.println(comm_rasp);
  }
  return comm_rasp;
}

//acciones de los motores
void pinzabidigital(){
  if(rotacion_pulgar<695){
    digitalWrite(dirCW_pulgar,LOW);
    digitalWrite(dirCCW_pulgar,HIGH);
    analogWrite(pwm_pulgar, 30);
  }
  if(rotacion_pulgar>705){
    digitalWrite(dirCW_pulgar,HIGH);
    digitalWrite(dirCCW_pulgar,LOW);
    analogWrite(pwm_pulgar, 30);
  }
  if(rotacion_pulgar<=705 && rotacion_pulgar>=695){
    digitalWrite(dirCW_pulgar,LOW);
    digitalWrite(dirCCW_pulgar,LOW);
    analogWrite(pwm_pulgar, 0);
  }
  if(rotacion_pulgar_dos>505){
    digitalWrite(dirCW_pulgar_dos,HIGH);
    digitalWrite(dirCCW_pulgar_dos,LOW);
    analogWrite(pwm_pulgar_dos, 30);
   }
  if(rotacion_pulgar_dos<495){
    digitalWrite(dirCW_pulgar_dos,LOW);
    digitalWrite(dirCCW_pulgar_dos,HIGH);
    analogWrite(pwm_pulgar_dos, 30);
  }  
  if(rotacion_pulgar_dos>=495 && rotacion_pulgar_dos<=505){
    digitalWrite(dirCW_pulgar_dos,LOW);
    digitalWrite(dirCCW_pulgar_dos,LOW);
    analogWrite(pwm_pulgar_dos, 0);
  }
  //Serial.print("pinzabidigital");
}

void pinzatripode(){
  Serial.print("pinzatripode");
}

void semiflexion_indice(){
  Serial.print("semflexion_indice");
}

void presapalmar(){
  Serial.print("presapalmar");
}

void extension(){
  if(rotacion_pulgar>0){
    digitalWrite(dirCW_pulgar,HIGH);
    digitalWrite(dirCCW_pulgar,LOW);
    analogWrite(pwm_pulgar, 30);
  }
  if(rotacion_pulgar<=0){
    digitalWrite(dirCW_pulgar,LOW);
    digitalWrite(dirCCW_pulgar,LOW);
    analogWrite(pwm_pulgar, 0);
  }
  if(rotacion_pulgar_dos>0){
    digitalWrite(dirCW_pulgar_dos,HIGH);
    digitalWrite(dirCCW_pulgar_dos,LOW);
    analogWrite(pwm_pulgar_dos, 30);
  }
  if(rotacion_pulgar_dos<=0){
    digitalWrite(dirCW_pulgar_dos,LOW);
    digitalWrite(dirCCW_pulgar_dos,LOW);
    analogWrite(pwm_pulgar_dos, 0);
  }
 // Serial.print("extension");
}

void flexion(){
  Serial.print("flexion");
}
void stopp(){
  //Serial.print("para los motores");
  analogWrite(pwm_pulgar, 0);
  analogWrite(pwm_pulgar_dos, 0);
}


//funciones de los encoders
void enc_pulgar(){
  if(digitalRead(dirCW_pulgar) == LOW && digitalRead(dirCCW_pulgar) == HIGH){
      rotacion_pulgar++;
      Serial.println(rotacion_pulgar);
  return rotacion_pulgar;
    }
  else if(digitalRead(dirCW_pulgar) == HIGH && digitalRead(dirCCW_pulgar) == LOW){
      rotacion_pulgar--; 
     Serial.println(rotacion_pulgar);
  return rotacion_pulgar;
    } 
}

void enc_pulgar_dos(){
   if(digitalRead(dirCW_pulgar_dos) == LOW && digitalRead(dirCCW_pulgar_dos) == HIGH){
      rotacion_pulgar_dos++;
      Serial.print("no2: ");
      Serial.println(rotacion_pulgar_dos);
  return rotacion_pulgar;
    }
  else if(digitalRead(dirCW_pulgar_dos) == HIGH && digitalRead(dirCCW_pulgar_dos) == LOW){
      rotacion_pulgar_dos--;
      Serial.print("no2: ");
      Serial.println(rotacion_pulgar_dos);
  return rotacion_pulgar;
    } 
}

void enc_indice(){
   if(digitalRead(dirCW_indice) == HIGH && digitalRead(dirCCW_indice) == LOW){
      rotacion_indice++;
  return rotacion_pulgar;
    }
  else if(digitalRead(dirCW_indice) == LOW && digitalRead(dirCCW_indice) == HIGH){
      rotacion_indice--;
  return rotacion_pulgar;
    } 
}

void enc_medio(){
   if(digitalRead(dirCW_medio) == HIGH && digitalRead(dirCCW_medio) == LOW)
    {
      rotacion_medio++;
  return rotacion_pulgar;
    }
  else if(digitalRead(dirCW_medio) == LOW && digitalRead(dirCCW_medio) == HIGH){
      rotacion_medio--;
  return rotacion_pulgar;
    } 
}

void enc_minique_anular(){
   if(digitalRead(dirCW_minique_anular) == HIGH && digitalRead(dirCCW_minique_anular) == LOW)
    {
      rotacion_minique_anular++;
  return rotacion_pulgar;
    }
  else if(digitalRead(dirCW_minique_anular) == LOW && digitalRead(dirCCW_minique_anular) == HIGH){
      rotacion_minique_anular--;
  return rotacion_pulgar;
    } 
}
