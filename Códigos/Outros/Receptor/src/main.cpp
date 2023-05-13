//==================================================================================//
// SERVER BANCO ESTATICO                                                                          //
//----------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------
// Libraries Needed For This Project
//------------------------------------------------------------------------------------
#include <SPI.h>
#include <ESP8266WiFi.h>    // The Basic Function Of The ESP NOD MCU

//------------------------------------------------------------------------------------
// WIFI Module Config
//------------------------------------------------------------------------------------
char ssid[] = "Wifi";                  // SSID of your home WiFi
char pass[] = "wanc21del";               // password of your home WiFi
WiFiServer server(80);

IPAddress ip(192, 168, 0, 110);           // IP address of the server
IPAddress gateway(192, 168, 0, 1);        // gateway of your network
IPAddress subnet(255, 255, 255, 0);        // subnet mask of your network

//------------------------------------------------------------------------------------
// Defining I/O Pins
//------------------------------------------------------------------------------------
#define       LedBoard   2        // WIFI Module LED
#define       botao    0        // Button

char modo;
String request;
//====================================================================================
void setup() {
  Serial.begin(9600);                   // only for debug
  WiFi.config(ip, gateway, subnet);       // forces to use the fix IP
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  server.begin();                         // starts the server
  Serial.println("Connected to wifi");
  Serial.print("Status: ");   Serial.println(WiFi.status());        // some parameters from the network
  Serial.print("IP: ");       Serial.println(WiFi.localIP());
  Serial.print("Subnet: ");   Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: ");  Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: ");     Serial.println(WiFi.SSID());
  Serial.print("Signal: ");   Serial.println(WiFi.RSSI());
  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());

  pinMode(LedBoard, OUTPUT);
  pinMode(botao, INPUT_PULLUP);
  Serial.println("Selecione o Modo");
  Serial.println("Digite C para Calibrar, L para Leitura e G para Gravar");
  Serial.print("Digite o Modo:");
  delay(5000);
  modo = Serial.read();
  Serial.println(modo);
  
}



String leStringSerial(){
  String conteudo = "";
  char caractere;
  
  // Enquanto receber algo pela serial
  while(Serial.available() > 0) {
    // Lê byte da serial
    caractere = Serial.read();
    // Ignora caractere de quebra de linha
    if (caractere != '\n'){
      // Concatena valores
      conteudo.concat(caractere);
    }
    // Aguarda buffer serial ler próximo caractere
    delay(10);
  }
  Serial.print("Recebi: ");
  Serial.println(conteudo);
    
  return conteudo;
  }
void loop() {
WiFiClient client = server.available();


if(modo == 'C'){
   Serial.println("Modo de Calibração");
  
  client.print(modo);
  
  while (modo == 'C')
  {
      
 // if (!client) {
 //   return;
 // }
    request = client.readStringUntil('\r');
    client.flush();
    if(request == "leitura"){
      Serial.println("Calibrado, entrando no modo Leitura");
      modo = 'L';
      
    }
    Serial.println(request);
  }
  }

if(modo == 'L'){
  Serial.println("Modo de Leitura");
  Serial.println("Para gravar no SD digite 'I'");
    client.print(modo);
  
  while (modo == 'L')
  {
      WiFiClient client = server.available();
  //if (!client) {
   // return;
  //}
    request = client.readStringUntil('\r');
    //client.flush();
    Serial.println(request);
    //char sta = Serial.read();
    //if(sta == 'I'){
    //  Serial.println("Iniciando armazenamneto no SD");
    //  modo = 'G';
   // }
}


 if(modo == 'G'){
  
  Serial.println("Gravando no SD");
  Serial.println("Digite 'P' para finalizar");
  client.print(modo);
  
  while (modo == 'G')
  {
    WiFiClient client = server.available();
    request = client.readStringUntil('\r');
    client.flush();
    Serial.println(request);
    char sta = Serial.read();
    if(sta == 'P'){
      Serial.println("Parando armazenamneto no SD");
      modo = 'P';
    }
}
 }

}else{
  Serial.println("Entrada Invalida, Digite Novamente");
  Serial.print("Digite o Modo:");
  delay(1000);
  modo = Serial.read();
  Serial.println(modo);
  delay(4000);
}

 
}