
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <SD.h>                            // Biblioteca de comunicação com cartão SD Nativa
#include <HX711.h>                         // Biblioteca para Leitura da Celula de Carga
#include <Wire.h>
#include <SPI.h>

//Coloque na linha abaixo o Mac Address do NodeMCU receptor
uint8_t broadcastAddress[] = {0xC8, 0xC9, 0xA3, 0x64, 0x8B, 0xD4};

//Estrutura para envio dos dados. Deve ser a mesma tanto no emissor como no receptor.
typedef struct struct_message
{
  String tempo;
  String força;
} struct_message;

//Definicoes botao
bool inverte_led = 0;
int valor;

//Cria uma struct_message chamada myData
struct_message myData;

//Callback quando os dados sao enviados
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus)
{
  
  if (sendStatus == 0) {
    //Serial.println("Envio ok!");
  }
  else {
    Serial.print("Status dos dados enviados: ");
    Serial.println("Falha no envio!");
  }
}


//------------------------------------------------------------------------------------
// Defining I/O Pins
//------------------------------------------------------------------------------------
#define       LedBoard   2                           // WIFI Module LED
#define       BUTTON     0                           // NodeMCU Button
#define       DT         D1                         //DT HX711
#define       SCK        D2                          //SCK HX711

//------------------------------------------------------------------------------------
//Declarando Variaveis
//------------------------------------------------------------------------------------

File myFile;
float led;
float peso;
String tele;

//------------------------------------------------------------------------------------
//declaracao do intervalo de espera
//------------------------------------------------------------------------------------

HX711 escala; //declaracao do objeto ESCALA na classe HX711 da biblioteca

const int FATOR_CALIBRACAO = -47060; //esse valor deve ser alterado para o valor de calibracao obtido com o outro codigo

//------------------------------------------------------------------------------------
//Função que inicializa o cartão sd
//------------------------------------------------------------------------------------

void sdini() {
  Serial.println("Inicializando o cartão SD...");
  // verifica se o cartão SD está presente e se pode ser inicializado
  //if (!SD.begin(8)) {                                            //Arduino CS Pino 8
  if (!SD.begin(D4)) {                                             //ESP GPIO02 D4
    Serial.println("Falha, verifique se o cartão está presente."); //programa encerrado
    while (1);                                                     //Continua verificando se SD foi conectado
    //return;                                                      //Verifica uma vez e continua o codigo
  }

  myFile = SD.open("data.txt", FILE_WRITE);                        //Cria arquivo data.txt e abre
  //Escreve dados no arquivo
  if (myFile) {
    Serial.print("Writing...");

    myFile.println("Time, Empuxo");
    myFile.close();

    Serial.println("Done.");
  } else {
    Serial.println("Error opening data.txt");
  }
}

//------------------------------------------------------------------------------------
//Função que grava dados em formato de string
//------------------------------------------------------------------------------------
void writeOnSD(float time, float empuxo) {
  String str;
  myFile = SD.open("data.txt", FILE_WRITE);

  if (myFile) {
    Serial.print("Writing...");

    str = String(time, 3) + "," + String(empuxo, 3) + "," + "\n";
    Serial.println(str);
    myFile.println(str);
    myFile.close();

    Serial.println("Done.");
  } else {
    Serial.println("Error opening data.txt");
  }
}
void setup()
{
  Serial.begin(115200);
  sdini();
  
  escala.begin (DT, SCK); //inicializacao e definicao dos pinos DT e SCK dentro do objeto ESCALA
  escala.tare(); //zera a escala
  escala.set_scale(FATOR_CALIBRACAO); //ajusta a escala para o fator de calibracao
  //Inicializa o pino do botao

  //Coloca o dispositivo no modo Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Inicializa o ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Erro ao inicializar o ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);

  //Registra o destinatario da mensagem
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  Serial.println("\n Fim setup");
}

void loop()
{
                if (escala.is_ready()){
                //mensagens de leitura no monitor serial
                peso = escala.get_units(), 10;
                tele =String((millis() / 1000.0),3) + "," + String(peso, 3)+"kg";
                writeOnSD(millis() / 1000.0, peso);
                Serial.println(peso);
                myData.tempo=String((millis() / 1000.0),3) ;
                myData.força=String((peso*9.807), 3)  ;
                //Envia a mensagem usando o ESP-NOW
                esp_now_send(broadcastAddress,(uint8_t *)  &myData, sizeof(myData));
                delay(100);
                }
                 else  {
                     Serial.println(" Aguarde  . . . ");
                }                 

}
