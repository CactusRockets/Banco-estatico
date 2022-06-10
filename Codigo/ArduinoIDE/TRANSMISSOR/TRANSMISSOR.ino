#include <ESP8266WiFi.h>
#include <espnow.h> //Bliblioteca do ESP-NOW

//Estrutura para envio dos dados. Deve ser a mesma tanto no emissor como no receptor.
typedef struct struct_message
{
  String tempo;
  String força;
} struct_message;

//Definicoes led
#define pino_led 2

//Cria uma struct_message chamada myData
struct_message myData;

//Funcao de Callback executada quando a mensagem for recebida
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len)
{
  memcpy(&myData, incomingData, sizeof(myData));
  digitalWrite(pino_led, LOW);
  //Serial.println();
  //Serial.print("Bytes recebidos: ");
  //Serial.println(len);
  Serial.print("Tempo: ");
  Serial.print(myData.tempo);
  Serial.print(",Força: ");
  Serial.println(myData.força);
  digitalWrite(pino_led, HIGH);

}

void setup()
{
  Serial.begin(115200);

  pinMode(pino_led, OUTPUT);
  digitalWrite(pino_led, LOW);

  //Coloca o dispositivo no modo Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Inicializa o ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Erro ao inicializar o ESP-NOW");
    return;
  }
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);
  digitalWrite(pino_led, HIGH);
}

void loop()
{
  //Neste programa nao temos comandos no loop
}
