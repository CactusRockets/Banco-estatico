#include <SPI.h>                           // Biblioteca de comunicação SPI Nativa
#include <ESP8266WiFi.h>                   // The Basic Function Of The ESP NODEMCU
#include <SD.h>                            // Biblioteca de comunicação com cartão SD Nativa
#include <HX711.h>                         // Biblioteca para Leitura da Celula de Carga
#include <Wire.h>

//------------------------------------------------------------------------------------
// Definindo pinos I/O 
//------------------------------------------------------------------------------------
#define       LedBoard     2        // WIFI Module LED
#define       DT           D1       // DT HX711
#define       SCK          D2       // SCK HX711

//------------------------------------------------------------------------------------
// Declarando Variaveis
//------------------------------------------------------------------------------------
int sdmod = 0;
File myFile;
float peso;
String tele;

//------------------------------------------------------------------------------------
// Variaveis de Autenticação do Wifi
//------------------------------------------------------------------------------------
char ssid[] = "Sett";                  // SSID 
char pass[] = "Scts2019LoL";           // Senha

/*
char ssid[] = "LabCCOMP";              // SSID 
char pass[] = "#labccomp#";            // Senha
*/

//------------------------------------------------------------------------------------
// Variaveis de Autenticação do Wifi
//------------------------------------------------------------------------------------
// Endereço do Server - Verificar no Roteador ou no inicio da serial 
IPAddress server(192,168,186,56);
WiFiClient client;

//------------------------------------------------------------------------------------

// declaracao do objeto ESCALA na classe HX711 da biblioteca
HX711 escala;

// esse valor deve ser alterado para o valor de calibracao obtido com o outro codigo
const int FATOR_CALIBRACAO = 15000;


//====================================================================================
void sdini() {
  Serial.print("Inicializando o cartão SD...");
  // verifica se o cartão SD está presente e se pode ser inicializado

  // ESP GPIO02 D4
  if (!SD.begin(D4)) {
    // programa encerrado 
    Serial.println("Falha, verifique se o cartão está presente.");
    client.connect(server, 80); 
    client.println("Falha, verifique se o cartão está presente \r");
    return;                                                      
  }

  // Cria arquivo data.txt e abre
  myFile = SD.open("data.txt", FILE_WRITE);                        
  // Escreve dados no arquivo
  if (myFile) {
    Serial.print("Gravando...");
    client.connect(server, 80); 
    client.println("Gravando...\r"); 

    myFile.println("Tempo, Empuxo");
    myFile.close();

  } else {
    Serial.println("Error ao abrir data.txt");
    client.connect(server, 80); 
    client.println("Error ao abrir data.txt\r"); 
  }
}
//====================================================================================

//------------------------------------------------------------------------------------
// Grava dados em formato de string
//------------------------------------------------------------------------------------

void writeOnSD(String str) {
  myFile = SD.open("data.txt", FILE_WRITE);

  if (myFile) {
    Serial.println("(OK)");
    myFile.println(str);
    myFile.close();

  } else {
    Serial.println("Error ao gravar em data.txt");
    client.connect(server, 80); 
    client.println("Error ao gravar em data.txt\r"); 
  }
}
//====================================================================================

void setup() {
  pinMode(LedBoard, OUTPUT);  
  digitalWrite(LedBoard, LOW);              
  Serial.begin(115200);                     
  Serial.println(".");
  Serial.println("Modulo Incializado");

  // connects to the WiFi router
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to wifi");

  // Network parameters
  Serial.print("Status: "); Serial.println(WiFi.status());
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());

  client.connect(server, 80); 
  client.println("Modulo de Leitura Conectado\r");
  
  // sdini(); 

  // client.connect(server, 80); 
  // client.println("Inicializando e calibrando Celula de carga\r"); 
  Serial.println("Inicializando e calibrando Celula de carga");  

  // inicializacao e definicao dos pinos DT e SCK dentro do objeto ESCALA
  escala.begin (DT, SCK);
  // Tara a balança
  escala.tare();
  // ajusta a escala para o fator de calibracao
  escala.set_scale(FATOR_CALIBRACAO); 

  // client.connect(server, 80); 
  // client.println("Configurações Encerrada, Iniciando Leituras\r"); 
  Serial.println("Configurações Encerrada, Iniciando Leituras"); 

}
//====================================================================================
void loop() {
  digitalWrite(LedBoard, LOW);  
  if (escala.is_ready()) {
                    
      peso = (escala.get_units()) * 9,8;
      tele = String((millis() / 1000.0),3) + "," + String(peso, 3) + '\r';
      Serial.print(tele);
      Serial.println("N");
      // writeOnSD(tele);                
      // client.connect(server, 80); 
      // client.println(tele);   
      delay(80);
      
    } else {
      // client.connect(server, 80); 
      // client.println("Erro de Leitura, Aguarde  . . . \r"); 
      Serial.println("Erro de Leitura, Aguarde  . . . ");
  }   
  digitalWrite(LedBoard, HIGH);         
}
