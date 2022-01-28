#include <Arduino.h>
#include <directADC.h>

#define M_PIN 6       // пин ленты
#define M_WIDTH 16    // ширина матрицы
#define M_HEIGHT 16    // высота матрицы
#define NUM_LEDS (M_WIDTH * M_HEIGHT) // для удобства запомним и количство ледов

#define SOUND_PIN ADC_A0         // аналоговый пин вход аудио
#define ARU_PIN ADC_A2
#define MAIN_ARU 600 // как часто проеверяем уровень
#define MAIN_LOOP 50        // период основного цикла отрисовки (по умолчанию 40)
//управление переменным резистором
#define CS_PIN 3 // !cs-5 pin --> CS_PIN
#define UD_PIN 4 // u/!d - 8 pin --> UD_PIN
#define T1 10
#define Ti 5 
#define ARU_MIN 230
#define ARU_MAX 240
bool con=true;
int count, count1;
byte counter=64;

// переменные для эффектов
byte mode=5 ;// текущий эффект
#define MAX_MODE 2

//кнопка
#define BTN_PIN 2        // кнопка подключена сюда (BTN_PIN --- КНОПКА --- GND)
#include "GyverButton.h"
GButton butt1(BTN_PIN);

// для частотного анализа звука
#define FHT_N 256         // ширина спектра х2
#define LOG_OUT 1		// use the log output function
#include <FHT.h>         // преобразование Хартли
//граничные частоты в спектре
byte spec[17]= {0, 1,2,3,4,5,6,8,11,15,21,28,38,51,69,94,127};
//byte spec[17]= {1,3,4,5,6,8,9, 11,15, 17,21,28,38,51,69,94,127};
int  Sheight[16];// массив значений для спектра

// WS2812
#define COLOR_DEBTH 2 
#include <microLED.h>
//#include <FastLEDsupport.h>
//microLED<NUMLEDS, STRIP_PIN, MLED_NO_CLOCK, LED_WS2818, ORDER_GRB, CLI_AVER> strip;
microLED<NUM_LEDS, M_PIN, MLED_NO_CLOCK, LED_WS2812, ORDER_GRB, CLI_AVER> matrix(M_WIDTH, M_HEIGHT, ZIGZAG, LEFT_BOTTOM, DIR_RIGHT);
unsigned long time_main = 0, time_aru=0;
int sample ;

// FIRE


// настройки пламени
#define FIRE_SCALE 35   // масштаб огня
#define HUE_GAP 60      // заброс по hue
#define FIRE_STEP 15    // шаг изменения "языков" пламени
#define HUE_START 5     // начальный цвет огня (0 красный, 80 зелёный, 140 молния, 190 розовый)
#define HUE_COEF 0.7    // коэффициент цвета огня (чем больше - тем дальше заброс по цвету)
#define SMOOTH_K 0.15   // коэффициент плавности огня
#define MIN_BRIGHT 50   // мин. яркость огня
#define MAX_BRIGHT 255  // макс. яркость огня
#define MIN_SAT 180     // мин. насыщенность
#define MAX_SAT 255     // макс. насыщенность


unsigned char matrixValue[8][16];
unsigned char line[M_WIDTH];
int pcnt = 0;

// ленивая жопа
#define FOR_i(from, to) for(int i = (from); i < (to); i++)
#define FOR_j(from, to) for(int j = (from); j < (to); j++)



// ********************** огонь **********************
//these values are substracetd from the generated values to give a shape to the animation
const unsigned char valueMask[8][16] PROGMEM = {
  {32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 , 32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 },
  {64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 , 64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 },
  {96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 },
  {128, 64 , 32 , 0  , 0  , 32 , 64 , 128, 128, 64 , 32 , 0  , 0  , 32 , 64 , 128},
  {160, 96 , 64 , 32 , 32 , 64 , 96 , 160, 160, 96 , 64 , 32 , 32 , 64 , 96 , 160},
  {192, 128, 96 , 64 , 64 , 96 , 128, 192, 192, 128, 96 , 64 , 64 , 96 , 128, 192},
  {255, 160, 128, 96 , 96 , 128, 160, 255, 255, 160, 128, 96 , 96 , 128, 160, 255},
  {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}
};

//these are the hues for the fire,
//should be between 0 (red) to about 25 (yellow)
const unsigned char hueMask[8][16] PROGMEM = {
  {1 , 11, 19, 25, 25, 22, 11, 1 , 1 , 11, 19, 25, 25, 22, 11, 1 },
  {1 , 8 , 13, 19, 25, 19, 8 , 1 , 1 , 8 , 13, 19, 25, 19, 8 , 1 },
  {1 , 8 , 13, 16, 19, 16, 8 , 1 , 1 , 8 , 13, 16, 19, 16, 8 , 1 },
  {1 , 5 , 11, 13, 13, 13, 5 , 1 , 1 , 5 , 11, 13, 13, 13, 5 , 1 },
  {1 , 5 , 11, 11, 11, 11, 5 , 1 , 1 , 5 , 11, 11, 11, 11, 5 , 1 },
  {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 , 0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 },
  {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 , 0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 },
  {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 }
};


void setup() {
     analogReference(EXTERNAL);
   ADC_setPrescaler(32);   
   setAnalogMux(SOUND_PIN);
//управление АРУ
   pinMode(CS_PIN, OUTPUT);
   pinMode(UD_PIN, OUTPUT);
   digitalWrite(CS_PIN, HIGH);
   digitalWrite(UD_PIN , HIGH);
// кнопка
  butt1.setDebounce(90);        // настройка антидребезга (по умолчанию 80 мс)
  butt1.setTimeout(300);        // настройка таймаута на удержание (по умолчанию 500 мс)
  butt1.setType(HIGH_PULL);
  butt1.setTickMode(AUTO);

// Leds start
  matrix.setBrightness(150);

  //для огня
  
  generateLine();
  memset(matrixValue, 0, sizeof(matrixValue));
}

void loop() {
    if (millis()-time_main > MAIN_LOOP) {
    time_main=millis();
    ticmode (); // работа с кнопкой
    animation ();
 }
   if (millis()-time_aru > MAIN_ARU) {
time_aru=millis();
    ARU ();
   }

}


// СЕРВИСНЫЕ ФУНУЦИИ

// функция масштабирования спектра
void mashtab ()
{
  int max=0;
  for (byte i = 0; i <  M_WIDTH; i++)
    {
      if (Sheight[i]>max) max= Sheight[i];
    }
    if (max<150)     max=150;
    
    for (byte i = 0; i < M_WIDTH; i++)
    { Sheight[i]=map (Sheight[i], 0, max, 0, 16); }

}
void ticmode ()
{
// работа с кнопкой
  if (butt1.isSingle()) mode=mode+1;       // проверка на один клик
  if (butt1.isDouble()) mode=mode-1;       // проверка на двойной клик
}