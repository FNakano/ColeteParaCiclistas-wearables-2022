# Projeto da Matéria de Computação Física e Aplicações - ACH2157

## Descrição

Muitas vezes quando um motociclista necessita realizar uma curva, a seta sinalizadora da moto geralmente não fica razoalvemente visível para os outros condutores. No caso dos ciclistas, nem há uma seta sinalizadora.

Por isso fizemos um colete com setas sinalizadoras para os ciclistas e motociclistas, com o intuito de melhorar a visibilidade deles nas vias.

### Lista de Materiais

| Quantidade | Nome | Link para referência |
| --- | --- | --- |
| 1 | ESP32-WROOM-32 Devkit V1 | https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf |
| 1 | fitas de LEDs endereçáveis (addressable RGB, aRGB) | https://a.aliexpress.com/_mLhYtMw |
| 1 | Módulo RF YK04 | https://www.faranux.com/product/4-channels-rf-remote-control-module-yk04/ |
| 1 | Sensor de Som Modelo MicNakano | https://github.com/FNakano/CFA/tree/master/projetos/sensorDeSom |
| X | Jumpers variados | --- |
| 2 | Power banks | --- |

Obs: Utilizamos 2 power banks, um para o ESP32 e outro para os LEDs, pois com apenas um os LEDs aRGBs estavam roubando muita energia e o ESP ficou instável)

ESP32:

![esp](/img/esp32.jpg)

Sensor de som modelo MicNakano:

![mic_nakano](/img/mic_nakano.jpg)

Pinos:

| Sensor de som | comentário | Jumper
| --- | --- | --- |
| 1 (mais distante do microfone) | VIN 3.3V | Verde
| 2 | Saída | Vinho
| 3 | GND | Cinza

Módulo RF YK04: 

![yk04](/img/yk04.jpg)

Pinos:

| Sensor de som | comentário | Jumper
| --- | --- | --- |
| GND | GND | Cinza
| 5V | 5V | Laranja
| D0 | Saída 1 | Lilás
| D1 | Saída 2 | Marrom
| D2 | Saída 3 | ---
| D3 | Saída 4 | ---
| VT | Emite sinal se apertar qualquer botão do controle | ---

### Conexões

https://docs.google.com/spreadsheets/d/1dG1MfTy3x1gHLY0yxJIRdy7Aiv9Ak8zw/edit?usp=sharing&ouid=111602521034713673280&rtpof=true&sd=true

**Nota**: Vale ressaltar que o GND da fita de LEDs tem que estar junto com o GND do ESP32, se não as cores da fita não irão funcinar corretamente.

## Como montar o dispositivo físico

Fazer as conexões listadas, transferir o código `sketch_oct14a.ino` para o ESP32 via Arduino IDE, e ligar o ESP32 e os LEDs com os power banks.

## Arquitetura e organização

Apenas feito em puro C++. Optamos por essa linguagem pois tinha mais exemplos em C++ do que em Python.

Para piscar uma luz é necessário utilizar a função `delay`, porém a utilização dessa função bloqueia a iteração da função principal loop, impedindo assim a execução simultânea do RF e do microfone. A solução foi se basear num sistema de tempo real, utilizando uma variável que marca o tempo em que o led foi aceso, e depois de X segundos passados, irá desligar, sem parar a execução do loop.

Para controlar a fita de LEDs, optamos por utilizar a biblioteca `FastLED.h` e dividindo a fita de led principal em duas partes: direita e esqueda.

## Como usar o programa

Como nosso projeto não tem nenhum interface gráfica, como por exemplo um web server, basta somente ligar o ESP32 e os LEDs nos power banks para o programa já executar.

## Como os LEDs endereçáveis funcionam?

Ao contrário de um LED RGB comum, os LEDs endereçáveis possuem um microcontrolador em cada célula que é responsável por acencer os LEDs de cada cor (vermelho, verde e azul em cada célula) e os comandos que enviamos para cada microcontrolador é feito por apenas um fio, que é o Din.

![addressable-led-strip-lights-18](img/addressable-led-strip-lights-18.webp)

No caso do LED que utilizamos, possui o controlador WS2812b, que é o maior retângulo escuro na imagem abaixo.

![webpc-passthru](img/webpc-passthru.webp)

Imagem mostrando o acionamento de cada led individualmente:

![ezgif-3-f3cf9f1843](img/ezgif-3-f3cf9f1843.gif)

Para mais detalhes, acesse o link https://blog.eletrogate.com/leds-enderecaveis-conhecendo-o-ws2812b/

## Como enviar comandos para os LEDs

Para enviar os comandos para os LEDs, utilizamos a biblioteca `FastLED.h` e instanciamos os leds dessa forma:

```C++
FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
```


## Como o programa foi feito

```python
p25 = Pin(25, Pin.OUT)
motor = PWM(p25, freq=50)
motor.duty(40)
```
Fonte: https://docs.micropython.org/en/latest/esp8266/tutorial/pwm.html#control-a-hobby-servo

```python
s.bind(('', 3000))   # bind to port 3000 https://docs.micropython.org/en/latest/library/socket.html#socket.socket.bind
s.listen(2)  # allow for 2 connection before refusing https://docs.micropython.org/en/latest/library/socket.html#socket.socket.listen
```

Uma requisição GET é feita (pelo navegador) concatenando, à URL (que endereça a requisição para o servidor), os parâmetros, no formato `?<id>=<valor>` no texto da requisição.

```python
    request = conn.recv(1024)  # get bytes https://docs.micropython.org/en/latest/library/socket.html#socket.socket.recv
    request = str(request)     # convert to string
```

Sobre o texto (string) da requisição, busca-se o parâmetro do GET:

```python
locker_on = request.find('/?locker=on') # find get request text https://www.w3schools.com/python/ref_string_find.asp
```

Em função do valor, o eixo do motor é girado a mais ou a menos:

```python
    if locker_on == 6:
        print('LOCKER ON')
        motor.duty(110)
        locker_state = "ON"
    if locker_off == 6:
        print('LOCKER OFF')
        motor.duty(40)
        locker_state = "OFF"
```

Ao final, a página é reenviada (não precisaria), para permitir um novo comando abre/fecha, junto com um código de resposta `HTTP-200` que significa OK (https://developer.mozilla.org/en-US/docs/Web/HTTP/Status).

## Referências

https://www.esp32.com/viewtopic.php?t=11904
https://docs.arduino.cc/built-in-examples/digital/BlinkWithoutDelay
https://create.arduino.cc/projecthub/talofer99/arduino-and-addressable-led-b8403f
https://github.com/FNakano/CFA/tree/master/projetos/sensorDeSom
https://blog.eletrogate.com/leds-enderecaveis-conhecendo-o-ws2812b/
https://www.derunledlights.com/pt/the-difference-between-addressable-rgb-led-strip-ws2811-ws2812b-ws2813-ws2815-sk6812-sk9822/

## Colaborar usando github (meta)

A maneira indicada pelo mantenedor do github para colaborar com projetos hospedados nele é através de bifurcação e pull request: https://stackoverflow.com/questions/32750228/how-to-contribute-to-someone-elses-repository.

É possível ser colaborador do projeto e fazer pull/push (https://stackoverflow.com/questions/42619669/how-to-make-branch-from-friends-repository), mas isto pode gerar confusão (quando fiz isso com outra pessoa eu a confundi e ela acabou reiniciando o repositório - `git init` - não foi um bom resultado).

Quando procurei essa informação, nas postagens, encontrei um material interessante sobre a organização (interna) do git: https://stackoverflow.com/tags/git/info

Sobre bifurcação: https://docs.github.com/pt/get-started/quickstart/fork-a-repo, https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/working-with-forks/about-forks.
