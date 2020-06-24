#include <FastLED.h>

#define NUM_LEDS 64
#define DATA_PIN 13

CRGB leds[NUM_LEDS];

char avaliable[] = {0x3c,
                    0x7e,
                    0xfb,
                    0xf7,
                    0xaf,
                    0xdf,
                    0x7e,
                    0x3c};
int avaliable_color = 0x040000;

char busy[] = { 0x3c,
                0x7e,
                0xff,
                0x81,
                0x81,
                0xff,
                0x7e,
                0x3c};
int busy_color = 0x000400;

char away[] = { 0x3c,
                0x6e,
                0xef,
                0xef,
                0xef,
                0xf7,
                0x7e,
                0x3c};
int away_color = 0x030400;

char count = 0;
char mask = 0x80;
int current_color = avaliable_color;
char *array_icon = avaliable;

void setup(){
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);


}

void loop(){
    for(char line = 0; line < 8; line++){
        mask = 0x80;
        for(char column = 0; column < 8; column++){
            if(busy[line] & mask)
                leds[count++] = busy_color;
            else 
                leds[count++] = 0;
            mask >>= 1;
        }
    }
    FastLED.show();
} 
