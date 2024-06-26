#include <SPI.h>             // Biblioteca de comunicação SPI Nativa
#include <ESP8266WiFi.h>     // The Basic Function Of The ESP NODEMCU
#include <SD.h>              // Biblioteca de comunicação com cartão SD Nativa
#include <HX711.h>           // Biblioteca para Leitura da Celula de Carga
#include <Wire.h>

//------------------------------------------------------------------------------------
// Definindo pinos I/O 
//------------------------------------------------------------------------------------
#define       LedBoard     2        // WIFI Module LED
#define       DT           D1       // DT HX711
#define       SCK          D2       // SCK HX711

#define USE_STORAGE true
#define CS_SDPIN D4

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

char ssid[] = "teste";                 // SSID 
char pass[] = "testesestatico";        // Senha

//------------------------------------------------------------------------------------
// Variaveis de Autenticação do Wifi
//------------------------------------------------------------------------------------
// Endereço do Server - Verificar no Roteador ou no inicio da serial 
IPAddress server(192,168,1,2);
WiFiClient client;

//------------------------------------------------------------------------------------

// Declaracao do objeto ESCALA na classe HX711 da biblioteca
HX711 escala;

// Esse valor deve ser alterado para o valor de calibracao obtido com o outro codigo
#define FATOR_CALIBRACAO 23500

void successInfo() {
  digitalWrite(LedBoard, HIGH);
}

void errorInfo() {
  digitalWrite(LedBoard, HIGH);
  delay(250);
  digitalWrite(LedBoard, LOW);
  delay(250);
}


//====================================================================================

void setupSd() {
  Serial.println("Inicializando o cartão SD...");
  // Verifica se o cartão SD está presente e se pode ser inicializado
  
  while(!SD.begin(CS_SDPIN)) {
    Serial.println("Falha, verifique se o cartão está presente.");
    client.connect(server, 80); 
    client.println("Falha, verifique se o cartão está presente.\r");
    while(1) {
      errorInfo();
    }
  }

  // Cria arquivo data.txt e abre
  myFile = SD.open("/data.txt", FILE_APPEND);
  // Escreve dados no arquivo
  if (myFile) {
    successInfo();

    Serial.println("Gravando dados iniciais!");
    client.connect(server, 80); 
    client.println("Gravando dados iniciais!\r");

    myFile.println("Tempo, Força, Fator de calibração");
    myFile.close();

  } else {
    Serial.println("Error ao abrir data.txt");
    client.connect(server, 80); 
    client.println("Error ao abrir data.txt\r");
  }
}

void writeOnSD(String str) {
  myFile = SD.open("/data.txt", FILE_APPEND);

  if (myFile) {
    Serial.println("(Dados Gravados!)");
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
  digitalWrite(LedBoard, HIGH);              
  Serial.begin(115200);                     
  Serial.println(".");
  Serial.println("Modulo Incializado");

  // Connects to the WiFi router
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  delay(10000);
  Serial.println("Connected to wifi");

  // Network parameters
  Serial.print("Status: ");
  Serial.println(WiFi.status());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Subnet: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("Signal: ");
  Serial.println(WiFi.RSSI());

  client.connect(server, 80); 
  client.println("Modulo de Leitura Conectado\r");
  client.connect(server, 80); 

  if(USE_STORAGE) {
    setupSd();
  }

  client.connect(server, 80); 
  client.println("Inicializando e calibrando Celula de carga\r"); 
  Serial.println("Inicializando e calibrando Celula de carga");  

  // Inicializacao e definicao dos pinos DT e SCK dentro do objeto ESCALA
  escala.begin (DT, SCK);
  // Tara a balança
  escala.tare();
  // Ajusta a escala para o fator de calibracao
  escala.set_scale(FATOR_CALIBRACAO); 

  client.connect(server, 80); 
  client.println("Configurações Encerrada, Iniciando Leituras\r"); 
  Serial.println("Configurações Encerrada, Iniciando Leituras"); 

}

//====================================================================================

void loop() {
  digitalWrite(LedBoard, LOW);  
  if (escala.is_ready()) {
                    
    peso = (escala.get_units()) * 9.8;
    tele = String((millis() / 1000.0),3) + "," + String(peso, 3) + '\r';
    Serial.print(tele);
    Serial.println("N");

    if(USE_STORAGE) {
      // Escreve no cartão SD
      writeOnSD(tele);
    }                
    client.connect(server, 80); 
    client.println(tele);

    // HX711 Requer que cada leitura seja realizado a no minimo a cada 100ms
    // 120ms é o ideal para não ocorrer travamentos durante as leituras sem cartão SD
    // No caso da utilização do cartão sd verificar a quanto tempo está realizando as leituras
    // E inserir um delay para que as leituras fiquem acima de 100ms 
    delay(120);
      
  } else {
    client.connect(server, 80); 
    client.println("Erro de Leitura, Aguarde...\r"); 
    Serial.println("Erro de Leitura, Aguarde...");
  }   
  digitalWrite(LedBoard, HIGH);         
}
