// BIBLIOTECAS
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


// Propriedades sensor DHT11
#define DHTPIN 2          // MUDE para o pino 2 (ou outro digital livre)
#define DHTTYPE DHT11

// Propriedades tela OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Criando objeto dht
DHT dht(DHTPIN, DHTTYPE);

// Declarando as variáveis antes para evitar erros de sequência do compilador
float t = 0;
float h = 0;

// Pinos dos componentes
const int led_umidade = 3;
const int led_temperatura = 4;
const int led_on = 6;

const int b_on = 5;

// Dispositivo ligado/desligado
bool ligado = false;




// INTERFACE GRÁFICA OLED
// [BEGIN lopaka generated]
#include "FreeSerifBoldItalic9pt7b.h"
#include "FreeMonoOblique12pt7b.h"
#include "FreeSans9pt7b.h"
#include "FreeSerifBoldItalic9pt7b.h"

#include "address_32_32_28f.h"

static const unsigned char PROGMEM image_weather_humidity_white_bits[] = {0x04,0x00,0x04,0x00,0x0c,0x00,0x0a,0x00,0x12,0x00,0x11,0x00,0x20,0x80,0x20,0x80,0x41,0x40,0x40,0xc0,0x80,0xa0,0x80,0x20,0x40,0x40,0x40,0x40,0x30,0x80,0x0f,0x00};

static const unsigned char PROGMEM image_weather_temperature_bits[] = {0x1c,0x00,0x22,0x02,0x2b,0x05,0x2a,0x02,0x2b,0x38,0x2a,0x60,0x2b,0x40,0x2a,0x40,0x2a,0x60,0x49,0x38,0x9c,0x80,0xae,0x80,0xbe,0x80,0x9c,0x80,0x41,0x00,0x3e,0x00};

void drawScreen_1(void) {
    display.clearDisplay();
    // weather_humidity_white
    display.drawBitmap(109, 40, image_weather_humidity_white_bits, 11, 16, 1);
    // rect 3
    display.drawRect(1, 3, 109, 22, 1);
    // line 5
    display.drawLine(110, 5, 112, 5, 1);
    // line 6
    display.drawLine(112, 6, 112, 27, 1);
    // line 7
    display.drawLine(65, 27, 3, 27, 1);
    // line 7
    display.drawLine(66, 27, 111, 27, 1);
    // rect 3 copy 1
    display.drawRect(14, 37, 109, 22, 1);
    // line 8
    display.drawLine(3, 26, 3, 25, 1);
    // line 5 copy 1
    display.drawLine(123, 39, 125, 39, 1);
    // line 6 copy 1
    display.drawLine(125, 40, 125, 61, 1);
    // line 7 copy 1
    display.drawLine(78, 61, 16, 61, 1);
    // line 8 copy 1
    display.drawLine(16, 60, 16, 59, 1);
    // line 7 copy 2
    display.drawLine(79, 61, 124, 61, 1);
    // weather_temperature
    display.drawBitmap(4, 6, image_weather_temperature_bits, 16, 16, 1);
    // string 15
    display.setTextColor(1);
    display.setTextWrap(false);
    display.setFont(&FreeSerifBoldItalic9pt7b);
    display.setCursor(36, 18);
    display.print(t, 2);
    display.print("  C");
    // string 15 copy 1
    display.setCursor(34, 52);
    display.print(h, 2);
    display.print("  %");
    // circle 17
    display.drawCircle(84, 7, 2, 1);
    display.display();
}

void updateAnimations() {
    address_32_32_28f_frame = millis() / 42 % 28;
}

void drawAnimation_address_32_32_28f(void) {
    display.fillRect(6, 16, 32, 32, 0);
    display.drawBitmap(6, 16, address_32_32_28f_frames[address_32_32_28f_frame], 32, 32, 1);
    display.display();
}

void drawAnimations() {
    updateAnimations();
    drawAnimation_address_32_32_28f();
}

void drawScreen_2(void) {
    display.clearDisplay();
    display.setTextColor(1);
    display.setTextWrap(false);
    display.setFont(&FreeMonoOblique12pt7b);
    display.setCursor(30, 49);
    display.print("");
    display.setFont(&FreeSerifBoldItalic9pt7b);
    display.setCursor(44, 45);
    display.print("00  C");
    display.drawCircle(71, 33, 2, 1);
    display.drawRect(4, 5, 120, 54, 1);
    display.drawLine(6, 59, 6, 61, 1);
    display.drawLine(124, 8, 126, 8, 1);
    display.drawLine(7, 61, 126, 61, 1);
    display.drawLine(126, 60, 126, 9, 1);
    display.setFont(&FreeSans9pt7b);
    display.setCursor(43, 27);
    display.print("Sorocaba");
    display.display();
}


// [END lopaka generated]


// FUNÇÕES DE CHECAGEM
void checar_temperatura(void) {
  if (t > 30.00) {
    digitalWrite(led_temperatura, HIGH);
  }
  else {
    digitalWrite(led_temperatura, LOW);
  }
}

void checar_umidade(void) {
  if ((h > 60.00 || h < 30.00) && ligado) {
    digitalWrite(led_umidade, HIGH);
  }
  else {
    digitalWrite(led_umidade, LOW);
  }
}



void setup() {
  dht.begin(); // OBRIGATÓRIO: Inicializa o sensor

  // Inicializa o OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true);
  }

  pinMode(led_umidade, OUTPUT);
  pinMode(led_temperatura, OUTPUT);
  pinMode(led_on, OUTPUT);

  pinMode(b_on, INPUT_PULLUP);

  digitalWrite(led_umidade, LOW);
  digitalWrite(led_temperatura, LOW);

  display.ssd1306_command(SSD1306_DISPLAYOFF);
}

void loop() {

  // BOTÃO ON/OFF
  if (digitalRead(b_on) == LOW) {
    if (!ligado) {
      digitalWrite(led_on, HIGH);
      display.ssd1306_command(SSD1306_DISPLAYON);
    }
    else {
      digitalWrite(led_on, LOW);
      digitalWrite(led_temperatura, LOW);
      digitalWrite(led_umidade, LOW);
      display.ssd1306_command(SSD1306_DISPLAYOFF);
    }
    ligado = !ligado;
    delay(1000);
  }

  // DISPOSITIVO LIGADO
  if (ligado) {
    delay(2000); // O DHT11 precisa de ~2 segundos entre leituras

    t = dht.readTemperature();
    h = dht.readHumidity();

    checar_temperatura();
    checar_umidade();

    drawScreen_1();
  }

}