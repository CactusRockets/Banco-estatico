#include <SPI.h>
// The Basic Function Of The ESP NOD MCU
#include <ESP8266WiFi.h>

//------------------------------------------------------------------------------------
// Definindo pinos I/O 
//------------------------------------------------------------------------------------
#define       LedBoard   2        
#define       SWITCH     3        

//====================================================================================

//------------------------------------------------------------------------------------
// Configuração do modulo de Wifi
//------------------------------------------------------------------------------------

// SSID
char ssid[] = "teste"; 
// Senha
char pass[] = "testesestatico";

/*
// SSID 
char ssid[] = "cactus";
// Senha
char pass[] = "bancokabum";
*/

WiFiServer server(80);

// IP - configurar de acordo com roteador conectado e inserir no Transmissor 
IPAddress ip(192, 168, 1, 2);
// IP padrão do roteador (gateway) - configurar de acordo com roteador conectado 
IPAddress gateway(192, 168, 1, 254);
// subnet
IPAddress subnet(255, 255, 255, 0);

//====================================================================================

void setup() {
  Serial.begin(115200);                   
  Serial.println(".");
  Serial.println("Modulo Inicializado"); 
  
  pinMode(SWITCH, INPUT_PULLUP);
  pinMode(LedBoard, OUTPUT);
  digitalWrite(LedBoard, LOW); 

  // Força a usar IP fixo
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, pass);

  // Status da coneção
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  server.begin();

  delay(5000);
  
  // Inicia o server
  Serial.println("Connected to wifi");

  // Parametros do wifi conectado
  Serial.print("Status: ");   Serial.println(WiFi.status());
  Serial.print("IP: ");       Serial.println(WiFi.localIP());
  Serial.print("Subnet: ");   Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");  Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: ");     Serial.println(WiFi.SSID());
  Serial.print("Signal: ");   Serial.println(WiFi.RSSI());
  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());

  delay(2000);
  digitalWrite(LedBoard, HIGH);
}

void loop() {

  WiFiClient cliente = server.available();
  if (!cliente) {
    return;
  }
  String request = cliente.readStringUntil('\r');
  Serial.println(request);
  
  digitalWrite(LedBoard, LOW);
  cliente.flush();  
  digitalWrite(LedBoard, HIGH);
}
