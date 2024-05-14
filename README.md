# Sistema de leitura de células de carga
Projeto eletrônico desenvolvido para leitura de empuxo dos motores produzidos pela equipe de foguetemodelismo Cactus Rockets Design

### Funcionamento

O sistema consiste em um microcontrolador ESP32 que é responsável por receber as leituras do modulo HX711, o qual converte as alterações de resistência das células de carga. Através dos valores de saída do HX711, são realizados os cálculos de empuxo.

Após esses cálculos, os valores são armazenados num cartão microSD e transmitidos para outro microcontrolador via comunicação Wifi do próprio ESP, para que seja exibido em tempo real.

Foi escolhido colocar uma conexão USB macho e fêmea entre as células de carga e o HX711 para que seja possível alterar o valor máximo de leitura inserindo mais células ou diminuindo sem a necessidade de mexer no resto do circuito. Sendo necessário apenas realizar a calibração das células via código.


### Como usar

Podemos dividir o sistema em duas partes separadas, o transmissor (sistema que faz a leitura das células e transmite os dados) e o receptor (que somente recebe os dados). Para que o sistema como um todo funcione apropriadamente, devemos configurar corretamente essas duas partes.

Para isto, é necessário ter o ArduinoIDE  instalado em seu computador juntamente com a instalação das placas ESP32.

Uma vez configurado, instale as seguintes bibliotecas:

- HX711 (versão 0.7.3)
- SD (versão 1.2.4)

Essas bibliotecas estão disponíveis no Gerenciador de bibliotecas do próprio ArduinoIDE.

#### Configurar calibração
No sistema transmissor, antes de realizar a leitura do empuxo, é necessário calibrá-lo para garantir os valores reais de Força que serão lidos. Para isso, utilize o código "Calibracao.ino" para auxiliar na busca do FATOR_CALIBRACAO ideal.

Conecte a célula de carga no transmissor, faça upload do código e posicione o peso conhecido na célula de carga no momento que o sistema permitir (analise o monitor serial para saber).

Esse código deve fazer uma série de estimativas com base no peso conhecido e retornar um Fator de calibração próximo do valor ideal.

OBS.: Lembre-se de ajustar os parâmetros do código para os valores desejados.

#### Configurar Transmissor dos dados
Uma vez encontrado o FATOR_CALIBRACAO ideal com determinada célula de carga, colocamos o valor encontrado no código de transmissão "Transmitter.ino".

Exemplo:

Se o fator de calibração for 50000, no código devemos ter a seguinte linha.

```C
#define FATOR_CALIBRACAO 50000
```

Configure o uso de armazenamento dos dados com o cartão microSD e o uso da transmissão dos dados por meio do Wifi do ESP.

```C
#define USE_STORAGE true
#define USE_WIFI true
```
Para termos a transmissão e recepção dos dados corretamente devemos configurar o endereço de broadcast. Ou seja, devemos encontrar o endereço MAC do ESP utilizado no sistema de recepção.

Para isso, utilize o código "Get_MAC_address.ino" no sistema de recepção dos dados. Este código retornará o endereço MAC do microcontrolador ESP no sistema de recepção dos dados.

Exemplo:

O código retornará: **30:C6:F7:B6:E0:90**

Transforme a representação acima para **{ 0x30, 0xC6, 0xF7, 0xB6, 0x9E0, 0x90 }**. E atribua esses valores para a variável "broadcastAddress" no código "Transmitter.ino".

```C
uint8_t broadcastAddress[] = { 0x30, 0xC6, 0xF7, 0xB6, 0x9E0, 0x90 };
```

Após isso, faça upload do código "Transmitter.ino" para o sistema transmissor através do software ArduinoIDE.

#### Configurar Receptor dos dados

Faça upload do código "Receiver.ino" no sistema de recepção dos dados.

#### Pronto

Com a célula de carga conectada ao sistema de leitura (que também é o sistema transmissor dos dados), podemos ligar o sistema. Se tudo ocorrer bem, no sistema de recepção dos dados poderemos acompanhar em tempo real os dados de empuxo lidos pela célula de carga.

### Componentes e Ligações


### Links
[Relatorio do sistema antigo](https://drive.google.com/file/d/1Hbn8Wrecm2mFYuo-eOkDm_AL81IvbxrG/view?usp=sharing)

  



































