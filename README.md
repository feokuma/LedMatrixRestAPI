# Led Matrix RestAPI
Projeto arduino que conectar a redes wi-fi e carrega um API Rest para receber comandos para acionar uma matrix de leds RGB

## Ferramentas e bibliotecas
- Arduino IDE ([arduino.cc](https://www.arduino.cc))
- Bibliotecas da placa Wemos D1 ([Tutorial para instalação](https://blogmasterwalkershop.com.br/embarcados/wemos/wemos-d1-configurando-a-ide-do-arduino/))
- Biblioteca para controle de leds RGB FastLED ([fastled.io](https://fastled.io))
- Biblioteca ArduinoJson versão 5 (tem que ser essa versão) ([arduinojson.org](https://arduinojson.org/v5/doc/installation))
- Postman ([postman.com](https://www.postman.com/))

## Hardware
- Placa Wemos D1 ([Wemos D1](https://www.filipeflop.com/produto/placa-wemos-d1-r2-wifi-esp8266/)) ou ([Wemos D1 Mini](https://www.filipeflop.com/produto/placa-wemos-d1-mini-pro-wifi-esp8266/))
- Matriz de Leds RGB WS2812b ([WS2812b](https://www.filipeflop.com/produto/painel-de-led-rgb-x16-ws2812-5050/?gclid=CjwKCAjw_qb3BRAVEiwAvwq6VlFsxFiCEGXGkRQ5Az3WoAN2fasJSACHhEla_VCaw_X2dU4IQ4iVERoC_8sQAvD_BwE))
- Conversor de Nível Lógico 3,3V para 5V ([Conversor Step Up](https://www.filipeflop.com/produto/conversor-de-nivel-logico-33-5v-bidirecional/))

## Montagem
O hardware é basicamente constituído de 3 placas, sendo que uma é um conversor step up, para garatir que a saída 3v3 da Wemos não terá problema em pilotar a entrada de sinal 5V do WS2812b. 

![Esquema de montagem do circuito](images/WemosWs2812b.png)

As legenda feiosa indica, pelas cores, qual sua função no circuito. 
Para os testes utilizei uma fonte de celular alimentando pelo conector USB mas dependendo da quantidade de leds que pretende utilizar recomendo usar uma fonte DC ligado na entrada P4 da Wemos. 

## Executando a aplicação Rest API
Ao ligar a placa será possível receber os logs através do monitor serial do Arduino IDE, onde serão será possível verificar os logs do processo de conexão com a rede wireless configurada hardcoded. Para isso, será necessário manter a placa conectada à porta usb, utilizada para fazer a gravação do firmware. Ao final da inicialização, se as configurações de rede estiverem corretas, a saída no monitor serial do Arduino IDE será semelhando a seguinte

```
Connecting to WiFi AP..........
Connected to YourNetworkSSID --- IP:192.168.1.200
HTTP REST Server Started
```

Com esta informação já sabemos onde podemos enviar os comandos para controlar as cores dos leds WS2812b. 

## Consumindo a API
Nossa API tem apenas dois endpoints. O raiz retorna simplesmente um texto de saudação da API que é para mostrar o quando o nosso firmware é simpático.

```
Welcome to the ESP8266 REST Web Server
```

**Enviando Post para alterar cor dos leds**

O json enviado, no método POST, para a Wemos D1 tem duas informações, **numLeds** e **status** indicando a quantidade de leds WS2812b que serão alterardos e a cor respectivamente. Os status possível são:

- 1: Verde;
- 2: Vermelho;
- 3: Amarelo;

Para o nosso exemplo, o nosso endpoint será: **http://192.168.1.200/leds**

```
POST /leds HTTP/1.1
Host: 192.168.1.200
Connection: close
Content-Type: application/json
Content-Length: 35

{
	"numLeds": 16,
	"status": 1
}
```

O método GET retorná o conteúdo do último POST recebido

```
HTTP/1.1 200 Ok
Content-Type: application/json
Content-Length: 30
Connection: close

{"numLeds":16,"status":"1"}
```
