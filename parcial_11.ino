/************************************
* Matriz de leds 8x8 con 2 74HC595
* Parcial 1 de Informatica 2
* Mateo Cardona
*************************************/

// pines del 74HC595
const int ClockPin_1 = 5; //SHCP
const int LatchPin_1 = 4; //STCP
const int DataPin_1 = 3;  //DS

const int ClockPin_2 = 12; //SHCP
const int LatchPin_2 = 11; //STCP
const int DataPin_2 = 10;  //DS

String a = "";
int set = 0, t = 0;
uint8_t aux[8];

// Algunas figuras para mostrar en la matriz en formato hexadecimal
uint8_t demo1[8] = { // H
  0xe7,
  0xe7,
  0xe7,
  0xff,
  0xff,
  0xff,
  0xe7,
  0xe7,
};

uint8_t demo2[8] = { // E
  0xff,
  0xff,
  0xe0,
  0xff,
  0xff,
  0xe0,
  0xff,
  0xff,
};

uint8_t demo3[8] = { // Y
  0xc3,
  0xe7,
  0x7e,
  0x3c,
  0x3c,
  0x3c,
  0x3c,
  0x3c,
};

uint8_t clearval[8] = { // Matriz apagada
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
};

uint8_t verif[8] = {
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
  0xff,
};

// SETUP con inicializacion de pines y serial en 9600
void setup()
{
  pinMode(ClockPin_1, OUTPUT);
  pinMode(LatchPin_1, OUTPUT);
  pinMode(DataPin_1, OUTPUT);
  
  pinMode(ClockPin_2, OUTPUT);
  pinMode(LatchPin_2, OUTPUT);
  pinMode(DataPin_2, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Bienvenido!");
  Serial.println("Para mostrar el mensaje de ayuda escriba 'h'");
}

// funcion para mostrar data[] en la matriz
void display(uint8_t data[])
{
  for(int j=0; j<8; j++){
    digitalWrite(LatchPin_2, LOW);
    digitalWrite(LatchPin_1, LOW);
    shiftOut(DataPin_2, ClockPin_2, LSBFIRST, ~0x80 >> j);
    shiftOut(DataPin_1, ClockPin_1, LSBFIRST, data[j]);
    digitalWrite(LatchPin_2, HIGH);
    digitalWrite(LatchPin_1, HIGH);
    delay(2);
  }
}

void demo()
{
  int t = 0;
  while(t < 2000){
    display(demo1);
    t += 32;
  }while(t < 4000){
    display(demo2);
    t += 32;
  }while(t < 6000){
    display(demo3);
    t += 32;
  }
}

void imagen(uint8_t* i)
{
  while(Serial.available() <= 0){
    i[0] = 0;
  }
  a = Serial.readString();
  Serial.println(a);
  String aaaa = "", nn = " ";
  int c = 0, x = 0;
  while(a[c]){
    if(a[c] == nn[0]){
      i[x] = aaaa.toInt();
      x++;
      aaaa = "";
    }else{
      aaaa = aaaa + a[c];
    }
    c++;
    if(x == 8)
      return;
  }
  i[x] = aaaa.toInt();
  aaaa = "";
}

void publik()
{
  int i = 0, t = 0, taux = 0;
  Serial.println("Ingrese la cantidad de patrones que quiere en la animacion");
  while(Serial.available() <= 0){
    i = 0;
  }
  a = Serial.readString();
  i = a.toInt();
  Serial.println("Ingrese el tiempo en milisegundos de cada cuanto cambia la imagen (1s = 1000ms)");
  while(Serial.available() <= 0){
    t = 0;
  }
  a = Serial.readString();
  t = a.toInt();
  uint8_t** auxs = new uint8_t*[i];
  for(int j = 0; j < i; j++)
    auxs[j] = new uint8_t[8];
  for(int j = 0; j < i; j++){
    Serial.println("Ingrese la imagen");
    imagen(aux);
    for(int k = 0; k < 8; k++){
      auxs[j][k] = (int) aux[k];
    }
  }
  Serial.println("Iniciando animacion");
  for(int j = 0; j < i; j++){
    while(taux < t*(j+1)){
      display(auxs[j]);
      taux += 32;
    }
  }
  Serial.println("animacion terminada");
  display(clearval);
  for(int j = 0; j < i; j++)
    delete[] auxs[j];
  delete[] auxs;
}

void loop()
{
  while(Serial.available() > 0){
    a = Serial.readString();
    Serial.println(a);

    if(a == "h" || a == "H"){
      Serial.println("Ingrese:");
      Serial.println("'h' para mostrar este mensaje de ayuda");
      Serial.println("'c' para apagar todos los leds");
      Serial.println("'demo' para ver una animacion de ejemplo con duracion de 6 segundos");
      Serial.println("'verificar' para verificar que todos los leds funcionan");
      Serial.println("'imagen' para ingresar una imagen personalizada y mostrarla en la matriz");
      Serial.println("'publik' para crear una animacion con n imagenes");
    }else if(a == "c" || a == "C"){
      Serial.println("Apagando todos los leds");
      set = 0;
    }else if(a == "verificar"){
      Serial.println("Encendiendo todos los leds para verificar el funcionamiento");
      set = 3;
    }else if(a == "demo"){
      Serial.println("Iniciando demostracion de la animacion de la palabra 'HEY'");
      demo();
      set = 0;
    }else if(a == "imagen"){
      Serial.println("Ingrese en formato decimal (0, 255), puede ayudarse de un conversor de binario a decimal.");
      display(clearval);
      imagen(aux);
      set = 1;
    }else if(a == "publik"){
      publik();
    }else{
      Serial.println("No es un comando valido");
    }
  }

  if(set == 3)
    display(verif);
  else if(set == 1)
    display(aux);
  else
    display(clearval);
}

  