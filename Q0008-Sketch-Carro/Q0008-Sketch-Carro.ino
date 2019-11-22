#include <Servo.h>

#include <RF24.h>



//*************** Definição dos Pinos ***********************
#define pinCE  7
#define pinCSN 8
#define pinIN1 10
#define pinIN2 9
#define pinIN3 6
#define pinIN4 5

//*************** Definição do Servo ***********************
Servo servo;

struct tipoDadosRF
{
   int pot1 = 512;
   int pot2 = 512;
   boolean botao = false;
};
tipoDadosRF dadosRF;

RF24 radio(pinCE,pinCSN);

const uint64_t pipeOut = 0xE8F8F1F5E1LL;


void setup() {
  //*************** Controle do RF ***********************
  radio.begin();
  radio.setPALevel( RF24_PA_LOW );     //RF24_PA_MIN  / RF24_PA_LOW / RF24_PA_HIGH / RF24_PA_MAX
  radio.setDataRate( RF24_250KBPS );   //RF24_250KBPS / RF24_1MBPS  / RF24_2MBPS
  radio.openReadingPipe(1, pipeOut);
  radio.startListening();  




  //*************** Controle do Carro ************
  pinMode(pinIN1, OUTPUT);
  pinMode(pinIN2, OUTPUT);
  pinMode(pinIN3, OUTPUT);
  pinMode(pinIN4, OUTPUT);
  
  servo.attach(3);
  Serial.begin(9600); 
  
}


void loop() {
  //*************** Controle do RF ***********************
  if (radio.available()) {                                   
     radio.read( &dadosRF, sizeof(tipoDadosRF) ); 
  }   
  

  //*************** Controle do Carro ************

         
      //*************** Direção - Esquerda ou Direita ************
      int angulo = map(dadosRF.pot2, 0, 1022, 0, 90);
      servo.write(angulo);
      //delay(50);

      Serial.print("Lido: ");
      Serial.print(dadosRF.pot2);
      Serial.print("  Angulo: ");
      Serial.println(angulo);
      //Serial.print(" Rádio Disponivel: ");
      //boolean estaAtivo = radio.available();
      //Serial.println(estaAtivo);
      
    
     
     
     if (dadosRF.pot1 < 512) {
        //Reverso
        int velocidade = map(dadosRF.pot1, 511, 0, 0, 255);

        analogWrite(pinIN1, 0);
        analogWrite(pinIN2, velocidade);
     
        analogWrite(pinIN3, 0);
        analogWrite(pinIN4, velocidade); 
     } else {
        //Para frente
        int velocidade = map(dadosRF.pot1, 512, 1023, 0, 255);

       analogWrite(pinIN1, velocidade);
       analogWrite(pinIN2, 0);
     
       analogWrite(pinIN3, velocidade);
       analogWrite(pinIN4, 0);                         
     }
  }  
