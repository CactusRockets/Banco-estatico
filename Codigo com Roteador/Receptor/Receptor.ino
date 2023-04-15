
#include <SPI.h>
#include <ESP8266WiFi.h>    // The Basic Function Of The ESP NOD MCU

//------------------------------------------------------------------------------------
// Definindo pinos I/O 
//------------------------------------------------------------------------------------
#define       LedBoard   2        
#define       SWITCH    D3        

//====================================================================================

//------------------------------------------------------------------------------------
// Configuração do modulo de Wifi
//------------------------------------------------------------------------------------
char ssid[] = "teste";                  // SSID 
char pass[] = "testesestatico";               // Senha

WiFiServer server(80);

IPAddress ip(192, 168, 1, 10);    // IP - configurar de acordo com roteador conectado e inserir no Transmissor 
IPAddress gateway(192, 168, 1, 254);  // IP padrão do roteador - configurar de acordo com roteador 
IPAddress subnet(255, 255, 255, 0);        // subnet 

//====================================================================================

void setup() {
  Serial.begin(115200);                   
  Serial.println(".");
  Serial.println("Modulo Incializado"); 
  
  pinMode(SWITCH, INPUT_PULLUP); 
  pinMode(LedBoard, OUTPUT);
  digitalWrite(LedBoard, LOW); 

  WiFi.config(ip, gateway, subnet);       // Força a usar IP fixo
  WiFi.begin(ssid, pass);                 // Status da coneção
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  server.begin();                         // Inicia o server
  Serial.println("Connected to wifi");
  Serial.print("Status: ");   Serial.println(WiFi.status());        // Parametros do wifi conectado
  Serial.print("IP: ");       Serial.println(WiFi.localIP());
  Serial.print("Subnet: ");   Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");  Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: ");     Serial.println(WiFi.SSID());
  Serial.print("Signal: ");   Serial.println(WiFi.RSSI());
  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());

  digitalWrite(LedBoard, HIGH);
 
}

void loop() {

    WiFiClient client = server.available();
  if (!client) {
    return;
  }
 String request = client.readStringUntil('\r');
  Serial.println(request);
  digitalWrite(LedBoard, LOW);
 client.flush();  
 digitalWrite(LedBoard, HIGH);
}
