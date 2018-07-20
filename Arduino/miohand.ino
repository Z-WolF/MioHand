#include<SoftwareSerial.h>

//definicion de direcciones
const byte CCW = 0;
const byte CW = 1;
const byte SHORT_BRAKE = 2;
const byte STOP = 3;

//definicion de dedos
const byte METACARPO = 1;
const byte FALANGE = 0;
const byte INDICE = 2;
const byte MEDIO = 3;
const byte M_A = 4;

//definicion de indices (usa CW y CCW de las direcciones como indice de array tambien).
const byte ISR = 2;
const byte PWM = 3;

//definicion de PINS por dedo (x) e indices (y).
const byte MANO[][4] = {
  {A1, A0, 3, 10},
  {13, 12, 7, 11},
  {A3, 4, 2, 9},
  {8, A2, 1, 6},
  {A5, A4, 0, 5}
};

//definicion de poses
const byte DESCANSO = 0;
const byte PUNO = 1;
const byte W_IN = 2;
const byte W_OUT = 3;
const byte ABIERTA = 4;
const byte DOBLE_CLICK = 5;

//definicion de indices de pose
const byte DESTINO = 0;
const byte ANTERIOR = 1;
const byte ESTADO = 2;

//definicion de estados de pose
const byte REPOSO = 0;
const byte MOVIMIENTO = 1;

//global de rotacion para interrupts
volatile int rotacion[] = {0, 0, 0, 0, 0};

byte POSE[][4] = {
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0},
  {0, 0, 0, 0}
};

//valores destino de rotaciones para poses
int POSES[][6] = {
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 3000, 0, 0, 0, 0},
  {0, 3000, 0, 0, 0, 0},
  {0, 3000, 0, 0, 0, 0}
};

/*serial input USB valores del raspberry*/

String comm_rasp = "0";
String actual = "0";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //declarar pins PWM como Output:
  pinMode(MANO[METACARPO][PWM], OUTPUT);
  pinMode(MANO[FALANGE][PWM], OUTPUT);
  pinMode(MANO[INDICE][PWM], OUTPUT);
  pinMode(MANO[MEDIO][PWM], OUTPUT);
  pinMode(MANO[M_A][PWM], OUTPUT);
  //todo pwm 0
  analogWrite(MANO[METACARPO][PWM], 0);
  analogWrite(MANO[FALANGE][PWM], 0);
  analogWrite(MANO[INDICE][PWM], 0);
  analogWrite(MANO[MEDIO][PWM], 0);
  analogWrite(MANO[M_A][PWM], 0);
  //declarar pins direccion como Output:
  pinMode(MANO[METACARPO][CW], OUTPUT);
  pinMode(MANO[METACARPO][CCW], OUTPUT);
  pinMode(MANO[FALANGE][CW], OUTPUT);
  pinMode(MANO[FALANGE][CCW], OUTPUT);
  pinMode(MANO[INDICE][CW], OUTPUT);
  pinMode(MANO[INDICE][CCW], OUTPUT);
  pinMode(MANO[MEDIO][CW], OUTPUT);
  pinMode(MANO[MEDIO][CCW], OUTPUT);
  pinMode(MANO[M_A][CW], OUTPUT);
  pinMode(MANO[M_A][CCW], OUTPUT);
  //declarar pins encoders
  pinMode(MANO[METACARPO][ISR], INPUT);  
  pinMode(MANO[FALANGE][ISR], INPUT);
  pinMode(MANO[INDICE][ISR], INPUT);  
  pinMode(MANO[MEDIO][ISR], INPUT);
  pinMode(MANO[M_A][ISR], INPUT);
  //declarar pins Interrupts
  attachInterrupt(digitalPinToInterrupt(MANO[METACARPO][ISR]), isr_motor_metacarpo, FALLING);
  attachInterrupt(digitalPinToInterrupt(MANO[FALANGE][ISR]), isr_motor_falange, FALLING);
  attachInterrupt(digitalPinToInterrupt(MANO[INDICE][ISR]), isr_motor_indice, FALLING);
  attachInterrupt(digitalPinToInterrupt(MANO[MEDIO][ISR]), isr_motor_medio, FALLING);
  attachInterrupt(digitalPinToInterrupt(MANO[M_A][ISR]), isr_motor_m_a, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Communicacion con el raspberry
  serial_comm();
  if(comm_rasp != actual){
    if(comm_rasp == "1"){
      POSE[FALANGE][DESTINO] = PUNO;
      POSE[METACARPO][DESTINO] = PUNO;
      POSE[INDICE][DESTINO] = PUNO;
      POSE[MEDIO][DESTINO] = PUNO;
      POSE[M_A][DESTINO] = PUNO;
    } else if(comm_rasp == "4"){
      POSE[FALANGE][DESTINO] = ABIERTA;
      POSE[METACARPO][DESTINO] = ABIERTA;
      POSE[INDICE][DESTINO] = ABIERTA;
      POSE[MEDIO][DESTINO] = ABIERTA;
      POSE[M_A][DESTINO] = ABIERTA;
    } else if(comm_rasp == "6"){
      full_stop();
      Serial.println("Full Stop!");
      actual = comm_rasp;
    } else if (comm_rasp == "valores"){
      Serial.println("Falange: " + String(rotacion[FALANGE]) + ", Metacarpo: " + String(rotacion[METACARPO]) + ", Indice: " + String(rotacion[INDICE]) + ", Medio: " + String(rotacion[MEDIO]) + ", A_M: "+ String(rotacion[M_A]));
      actual = comm_rasp;
    } else {
      debug();
      actual = comm_rasp;
    }
  }
  check_dedo(FALANGE);
  check_dedo(METACARPO);
  check_dedo(INDICE);
  check_dedo(MEDIO);
  check_dedo(M_A);
}

void debug(){
  String dedos[] = {"Falange", "Metacarpo", "Indice", "Medio", "A_M"};
  String direcciones[] = {"CCW", "CW"};
  byte dedo = comm_rasp.substring(0,1).toInt();
  byte dir = comm_rasp.substring(2,3).toInt();
  byte speed = comm_rasp.substring(4).toInt();
  Serial.println(dedos[dedo] + " - " + direcciones[dir] + " - " + speed + " | " + comm_rasp + " | " + String(MANO[dedo][ISR]) + ":" + String(MANO[dedo][dir]) + ":" + String(MANO[dedo][PWM]));
  set_direction(dedo, dir);
  set_speed(dedo, speed);
}

void check_dedo(byte dedo){
  if(POSE[dedo][ESTADO] == REPOSO && POSE[dedo][DESTINO] != POSE[dedo][ANTERIOR]){
    start_dedo(dedo);
  } else if (POSE[dedo][ESTADO] == MOVIMIENTO && get_direction(dedo) == CCW && (POSES[POSE[dedo][DESTINO]][dedo] - rotacion[dedo]) < 0 ){
    stop_dedo(dedo);
  } else if (POSE[dedo][ESTADO] == MOVIMIENTO && get_direction(dedo) == CW && (POSES[POSE[dedo][DESTINO]][dedo] - rotacion[dedo]) > 0 ){
    stop_dedo(dedo);
  }
}

void start_dedo(byte dedo){
  int check = POSE[dedo][DESTINO] - rotacion[dedo];
  byte dir;
  if(check <= 0){
    dir = CW;
  } else {
    dir = CCW;
  }
  set_direction(dedo, dir);
  set_speed(dedo, 250);
  POSE[dedo][ESTADO] = MOVIMIENTO;
}

void stop_dedo(byte dedo){
  set_speed(dedo, 0);
  POSE[dedo][ESTADO] = REPOSO;
  POSE[dedo][ANTERIOR] = POSE[dedo][DESTINO];
}

void full_stop(){
  set_speed(METACARPO, 0);
  set_speed(FALANGE, 0);
  set_speed(INDICE, 0);
  set_speed(MEDIO, 0);
  set_speed(M_A, 0);
}

void isr_motor_metacarpo(){
  isr_motor(METACARPO);
}

void isr_motor_falange(){
  isr_motor(FALANGE);
}

void isr_motor_indice(){
  isr_motor(INDICE);
}

void isr_motor_medio(){
  isr_motor(MEDIO);
}

void isr_motor_m_a(){
  isr_motor(M_A);
}

void isr_motor(int dedo){
  int dir = get_direction(dedo);
  if(dir == CW){
    rotacion[dedo]--;
  } else if(dir == CCW){
    rotacion[dedo]++;
  }
}

int get_direction(int dedo){
  int dedo_cw = digitalRead(MANO[dedo][CW]);
  int dedo_ccw = digitalRead(MANO[dedo][CCW]);
  if(POSE[dedo][PWM] == 0){
    return SHORT_BRAKE;
  } else if(dedo_cw == LOW && dedo_ccw == HIGH){
    return CCW;
  } else if(dedo_cw == HIGH && dedo_ccw == LOW){
    return CW;
  } else if(dedo_cw == LOW && dedo_ccw == LOW){
    return STOP;
  } else {
    return SHORT_BRAKE;
  }
}

void set_direction(int dedo, int dir){
  if(dir == CW){
    digitalWrite(MANO[dedo][CW], HIGH);
    digitalWrite(MANO[dedo][CCW], LOW);
  } else if(dir == CCW){
    digitalWrite(MANO[dedo][CW], LOW);
    digitalWrite(MANO[dedo][CCW], HIGH);
  } else if (dir == STOP){
    digitalWrite(MANO[dedo][CW], LOW);
    digitalWrite(MANO[dedo][CCW], LOW);
  }
}

void set_speed(int dedo, byte speed){
  analogWrite(MANO[dedo][PWM], speed);
  POSE[dedo][PWM] = speed;
}

//Serial comunicacion
void serial_comm(){
  if( Serial.available() ){
    comm_rasp = Serial.readString();
  }
}
