#include <SPI.h>                            // Biblioteca de comunicação SPI Nativa
#include <ESP8266WiFi.h>                    // The Basic Function Of The ESP NODEMCU
#include <SD.h>                            // Biblioteca de comunicação com cartão SD Nativa
#include <HX711.h>                         // Biblioteca para Leitura da Celula de Carga
#include <Wire.h>

//------------------------------------------------------------------------------------
// Definindo pinos I/O 
//------------------------------------------------------------------------------------
#define       LedBoard     2        // WIFI Module LED
#define       DT           D1       //DT HX711
#define       SCK          D2       //SCK HX711

//------------------------------------------------------------------------------------
// Declarando Variaveis
//------------------------------------------------------------------------------------
int sdmod=0;
File myFile;
float peso;
String tele;

//------------------------------------------------------------------------------------
// Variaveis de Autenticação do Wifi
//------------------------------------------------------------------------------------
char ssid[] = "teste";                  // SSID 
char pass[] = "testesestatico";               // Senha

//------------------------------------------------------------------------------------
// Variaveis de Autenticação do Wifi
//------------------------------------------------------------------------------------
IPAddress server(192,168,1,10);           // Endereço do Server - Verificar no Roteador ou no inicio da serial do Receptor
WiFiClient client;

//------------------------------------------------------------------------------------


HX711 escala; //declaracao do objeto ESCALA na classe HX711 da biblioteca

const int FATOR_CALIBRACAO = -45000; //esse valor deve ser alterado para o valor de calibracao obtido com o outro codigo


//====================================================================================
void sdini() {
  Serial.print("Inicializando o cartão SD...");
  // verifica se o cartão SD está presente e se pode ser inicializado
    if (!SD.begin(D4)) {                                             //ESP GPIO02 D4
    Serial.println("Falha, verifique se o cartão está presente."); //programa encerrado
    client.connect(server, 80); 
    client.println("Falha, verifique se o cartão está presente \r"); 
    //while (1);        //Continua verificando se SD foi conectado
                      //Comente para Verificar uma vez e continuar o codigo
    return;                                                      
  }

  myFile = SD.open("data.txt", FILE_WRITE);                        //Cria arquivo data.txt e abre
  //Escreve dados no arquivo
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

    Serial.println("-OK");
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
   
  WiFi.begin(ssid, pass);                   // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());

  client.connect(server, 80); 
  client.println("Modulo de Leitura Conectado\r"); 
  //Serial.println("Conectado ao Receptor");

  client.connect(server, 80); 
  client.println("Inicializando cartao SD\r"); 
  
  //sdini(); //Descomente para inicializar a gravação no SD

  client.connect(server, 80); 
  client.println("Inicializando e calibrando Celula de carga\r"); 
  Serial.println("Inicializando e calibrando Celula de carga");  

  escala.begin (DT, SCK); //inicializacao e definicao dos pinos DT e SCK dentro do objeto ESCALA
  escala.tare(); //Tara a balança
  escala.set_scale(FATOR_CALIBRACAO); //ajusta a escala para o fator de calibracao 

  client.connect(server, 80); 
  client.println("Configurações Encerrada, Iniciando Leituras\r"); 
  Serial.println("Configurações Encerrada, Iniciando Leituras"); 

}
//====================================================================================
void loop() {
  digitalWrite(LedBoard, LOW);  
  if (escala.is_ready()){
                    
                peso = (escala.get_units())*9,8;
                tele = String((millis() / 1000.0),3) + "," + String(peso, 3) + '\r';
                Serial.print(tele);
                Serial.println("N");
                //writeOnSD(tele);  //Descomente para gravar no sd              
                client.connect(server, 80); 
                client.println(tele);   
                delay(120); //HX711 Requer que cada leitura seja realizado a no minimo a cada 100ms
                //120ms ideal para não ocorrer travamentos durante as leituras sem cartão SD
                //No caso de utilização do cartão sd verificar a quanto tempo estar realizando as leitura 
                //E inserir um delay para que as leituras fiquem acima de 100ms
                
              }
            else  {
                 client.connect(server, 80); 
                 client.println("Erro de Leitura, Aguarde  . . . \r"); 
                 Serial.println("Erro de Leitura, Aguarde  . . . ");
            }   
  digitalWrite(LedBoard, HIGH);         
  }

