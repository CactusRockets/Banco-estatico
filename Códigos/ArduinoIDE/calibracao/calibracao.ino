#include <HX711.h>                  // Biblioteca para Leitura da Celula de Carga

//------------------------------------------------------------------------------------
// Definindo pinos I/O 
//------------------------------------------------------------------------------------
#define       LedBoard     2        // WIFI Module LED
#define       DT           D1       // DT HX711
#define       SCK          D2       // SCK HX711

#define Diferenca_maxima 50
#define Diferenca_media 10
#define Diferenca_minima 5

#define Reajuste_maximo 300
#define Reajuste_medio 100
#define Reajuste_minimo 50

#define massa_conhecida 52
#define fator_calibracao_inicial 23500

//------------------------------------------------------------------------------------
// Declarando Variaveis
//------------------------------------------------------------------------------------
float peso;
String informations;

// Declaracao do objeto ESCALA na classe HX711 da biblioteca
HX711 escala;

int FATOR_CALIBRACAO = fator_calibracao_inicial;

// Aqui deve ser colocado o valor do peso conhecido de algum objeto para a calibração
// Em Newtons (9.8m/s^2)
float peso_conhecido = massa_conhecida * 9.8;

void setup() {
  pinMode(LedBoard, OUTPUT);  
  digitalWrite(LedBoard, HIGH);
            
  Serial.begin(115200);
  Serial.println("Modulo Incializado");

  Serial.println("Inicializando e calibrando Celula de carga");  
  // inicializacao e definicao dos pinos DT e SCK dentro do objeto ESCALA
  escala.begin (DT, SCK);
  // Tara a balança
  escala.tare();
  // ajusta a escala para o fator de calibracao
  escala.set_scale(FATOR_CALIBRACAO);

  // Coloque o objeto na balança até o final do tempo
  Serial.println("Coloque o objeto na balança até o final do tempo:");
  int tempo = 10;
  while(tempo > 0) {
    Serial.println(String(tempo) + "...");
    delay(1000);
    tempo -= 1;
  }
  Serial.println("\n");
}
//====================================================================================

void loop() {
  digitalWrite(LedBoard, LOW);
  
  if (escala.is_ready()) {
      peso = (escala.get_units()) * 9.8;
      informations = String(peso_conhecido) + "N" + "," + String(peso, 3) + "N" + "," + String(FATOR_CALIBRACAO);
      Serial.println(informations);

      // Reajuste na calibração
      if((peso - peso_conhecido) > Diferenca_maxima) {
        FATOR_CALIBRACAO += Reajuste_maximo;
        
      } else if((peso - peso_conhecido) > Diferenca_media) {
        FATOR_CALIBRACAO += Reajuste_medio;
        
      }  else if((peso - peso_conhecido) > Diferenca_minima) {
        FATOR_CALIBRACAO += Reajuste_minimo;



      } else if((peso - peso_conhecido) < -Diferenca_maxima) {
        FATOR_CALIBRACAO -= Reajuste_maximo;
        
      } else if((peso - peso_conhecido) < -Diferenca_media) {
        FATOR_CALIBRACAO -= Reajuste_medio;
        
      } else if((peso - peso_conhecido) < -Diferenca_minima) {
        FATOR_CALIBRACAO -= Reajuste_minimo;
        
        
      } else {
        while(1) {
          Serial.println("Fator de calibração ideal:" + String(FATOR_CALIBRACAO));
          delay(500);
        }
      }

      escala.set_scale(FATOR_CALIBRACAO);
        

      // HX711 Requer que cada leitura seja realizado a no minimo a cada 100ms
      // 120ms é o ideal para não ocorrer travamentos durante as leituras sem cartão SD
      // No caso da utilização do cartão sd verificar a quanto tempo está realizando as leituras
      // E inserir um delay para que as leituras fiquem acima de 100ms 
      delay(120);
      
    } else {
      Serial.println("Erro de Leitura");
  }   
  digitalWrite(LedBoard, HIGH);         
}
