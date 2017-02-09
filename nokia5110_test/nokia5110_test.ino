#include "LCD5110_Basic.h"
#include "DS1307.h"
#include "DHT.h"

LCD5110 tela(8,9,10,12,11);
/* Cria objeto da classe LCD5110
CLK – Pino 8
DIN – Pino 9
DC – Pino 10
RST – Pino 12
CE – Pino 11
*/
DS1307 rtc(A4, A5);
/* Cria objeto da classe DS1307
SDA - Pino A4
SCL - Pino A5
*/
DHT dht(A2, DHT11);

int pino_d = 7; //Pino ligado ao D0 do sensor
int pino_a = A1; //Pino ligado ao A0 do sensor
int val_d = 0; //Armazena o valor lido do pino digital
int val_a = 0; //Armazena o valor lido do pino analogico

//Obtendo as fontes
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];
 
void setup()
{
 tela.InitLCD(); //Inicializando o display

 //Aciona o relogio
 rtc.halt(false);
   
 //As linhas abaixo setam a data e hora do modulo e podem ser comentada apos a primeira utilizacao
 //rtc.setDOW(WEDNESDAY);       //Define o dia da semana
 //rtc.setTime(20, 23, 0);   //Define o horario
 //rtc.setDate(8, 2, 2017); //Define o dia, mes e ano
   
 //Definicoes do pino SQW/Out
 rtc.setSQWRate(SQW_RATE_1);
 rtc.enableSQW(true);
   
 Serial.begin(9600);
 dht.begin();
 
 pinMode(pino_d, INPUT);
 pinMode(pino_a, INPUT);
 delay(1000);
}
 
void loop()
{
 //======== INFORMAÇÕES DE TEMPO ==========//
 //tela.setFont(BigNumbers); //Definindo a fonte
 //Escreve a palavra vida alinhada à esquerda começando na linha 0
 tela.setFont(MediumNumbers);
 tela.print(rtc.getTimeStr(FORMAT_SHORT), CENTER, 0);

 tela.setFont(SmallFont);
 tela.print(rtc.getDOWStr()+String(","), LEFT, 30);
 tela.print(rtc.getDateStr(FORMAT_SHORT), RIGHT, 30);

 //======== INFORMAÇÕES DE TEMPERATURA ======//
 //tela.print((int)dht.readHumidity()+String("%"), RIGHT, 36);
 tela.print((int)dht.readTemperature()+String("~C"), LEFT, 40);

 //======== INFORMAÇÕES DE CHUVA =======//
 //Le e arnazena o valor do pino digital
 val_d = digitalRead(pino_d);
 //Le e armazena o valor do pino analogico
 val_a = analogRead(pino_a);

//0 - Tem água / 1 - Não tem água
 if (val_d == 0){    
   tela.clrScr(); 
   if (val_a >900 && val_a <1023){
    tela.print("Sem chuva", CENTER, 0);  
   }
   else if (val_a >600 && val_a <900){
    tela.print("Chuva fraca", CENTER, 0);
   }
   else if (val_a >400 && val_a <600) {
    tela.print("Chuva moderada", CENTER, 0);
   }
   else if (val_a <400) {
    tela.print("Chuva forte", CENTER, 0);
   }    
 }
 
 delay(10000);
 tela.clrScr(); 
}
