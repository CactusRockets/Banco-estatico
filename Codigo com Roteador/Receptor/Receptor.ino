#include <SPI.h>
// The Basic Function Of The ESP NOD MCU
#include <ESP8266WiFi.h>

//------------------------------------------------------------------------------------
// Definindo pinos I/O 
//------------------------------------------------------------------------------------
#define       LedBoard   2        
#define       SWITCH    D3        

//====================================================================================

//------------------------------------------------------------------------------------
// Configuração do modulo de Wifi
//------------------------------------------------------------------------------------
// SSID 
char ssid[] = "cactus";
// Senha
char pass[] = "bancokabum";

WiFiServer server(80);

IPAddress ip(192, 168, 186, 56);           // IP 
IPAddress gateway(192, 168, 186, 223);     // gateway
IPAddress subnet(255, 255, 255, 0);        // subnet 

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

  digitalWrite(LedBoard, HIGH);
}

void loop() {

  WiFiClient client = server.available();
  if (!client) {
    Serial.println("Cliente nao detectado!");
    return;
  }
  String request = client.readStringUntil('\r');
  Serial.println(request);
  
  digitalWrite(LedBoard, LOW);
  client.flush();  
  digitalWrite(LedBoard, HIGH);
}
