#include <Adafruit_CircuitPlayground.h>
#include "pitches.h"

// basic seasonal colors
#define NEO_RED   0xFF0000
#define NEO_GREEN 0x00FF00
#define NEO_BLUE  0x0000FF
#define NEO_WHITE 0xFFFFFF
#define NEO_BLACK 0x000000

// color scheme: https://www.schemecolor.com/christmas-hex-color-codes.php
#define NEO_DARK_GREEN 0x165B33
#define NEO_LIGHT_GREEN 0x146B3A
#define NEO_ORANGE_YELLOW 0xF8B229
#define NEO_CINNEBAR 0xEA4630
#define NEO_FIREBRICK 0xBB2528

// color scheme: https://www.schemecolor.com/mistletoe-kiss-christmas-color.php
#define NEO_LINC_GREEN 0x215B01
#define NEO_AVOCADO 0x538B06
#define NEO_APPL_GREEN 0x86BB00
#define NEO_ENG_INT_ORANGE 0xD11E00
#define NEO_CRIM_RED 0x990B00

// holiday color patterns, add more below
uint32_t patternArray[][10] = {
  {NEO_RED, NEO_GREEN, NEO_WHITE, NEO_RED, NEO_GREEN, NEO_WHITE, NEO_RED, NEO_GREEN, NEO_WHITE, NEO_BLACK},
  {NEO_BLACK, NEO_WHITE, NEO_BLACK, NEO_WHITE, NEO_BLACK, NEO_WHITE, NEO_BLACK, NEO_WHITE, NEO_BLACK, NEO_WHITE},
  {NEO_RED, NEO_WHITE, NEO_RED, NEO_WHITE, NEO_RED, NEO_WHITE, NEO_RED, NEO_WHITE, NEO_RED, NEO_WHITE},
  {NEO_RED, NEO_GREEN, NEO_RED, NEO_GREEN, NEO_RED, NEO_GREEN, NEO_RED, NEO_GREEN, NEO_RED, NEO_GREEN},
  {NEO_BLUE, NEO_WHITE, NEO_BLUE, NEO_WHITE, NEO_BLUE, NEO_WHITE, NEO_BLUE, NEO_WHITE, NEO_BLUE, NEO_WHITE},
  {NEO_LINC_GREEN, NEO_LINC_GREEN, NEO_AVOCADO, NEO_AVOCADO, NEO_APPL_GREEN, NEO_APPL_GREEN, NEO_ENG_INT_ORANGE, NEO_ENG_INT_ORANGE, NEO_CRIM_RED, NEO_CRIM_RED},
  {NEO_DARK_GREEN, NEO_DARK_GREEN, NEO_LIGHT_GREEN, NEO_LIGHT_GREEN, NEO_ORANGE_YELLOW, NEO_ORANGE_YELLOW, NEO_CINNEBAR, NEO_CINNEBAR, NEO_FIREBRICK, NEO_FIREBRICK}
};

// Jingle Bells
int jingle_melody[] = {
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5,
  NOTE_E5,
  NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5,
  NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5,
  NOTE_D5, NOTE_G5
};

int jingle_tempo[] = {
  8, 8, 4,
  8, 8, 4,
  8, 8, 8, 8,
  2,
  8, 8, 8, 8,
  8, 8, 8, 16, 16,
  8, 8, 8, 8,
  4, 4
};

// We wish you a merry Christmas

int wish_melody[] = {
  NOTE_B3,
  NOTE_F4, NOTE_F4, NOTE_G4, NOTE_F4, NOTE_E4,
  NOTE_D4, NOTE_D4, NOTE_D4,
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_F4,
  NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4,
  NOTE_F4, NOTE_D4, NOTE_B3, NOTE_B3,
  NOTE_D4, NOTE_G4, NOTE_E4,
  NOTE_F4
};

int wish_tempo[] = {
  4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 8, 8,
  4, 4, 4,
  2
};

int offset = 0;
int pos;
int pattern;
int numPatterns = 7;

void setup() {
  CircuitPlayground.begin();
  CircuitPlayground.clearPixels();
  CircuitPlayground.setBrightness(10);

  // Seed the random function with noise
  int seed = 0;
  seed += analogRead(12);
  seed += analogRead(7);
  seed += analogRead(9);
  seed += analogRead(10);
  randomSeed(seed);

  Serial.begin(9600);
  Serial.println("Best Holiday Decoration, Ever!");


  pattern = random(0, numPatterns + 1); //pick a random pattern
  Serial.print("patterns:"); Serial.print(numPatterns); Serial.print(" picked:"); Serial.println(pattern);

}

void animatePixelPattern(int pattern, int loopDelay) {
  // light up the neopixels following the order in the patternArray
  // the incrementing offset animates it around the pixels, wrapping around

  for (int i = 0; i < 10; i++) {
    pos = i + offset;
    if (pos >= 10) {
      pos = pos - 10 ;
    }

    CircuitPlayground.setPixelColor(i, patternArray[pattern][pos]);
    //Serial.print("i="); Serial.print(i); Serial.print(" offset="); Serial.print(offset); Serial.print(" pos="); Serial.println(pos); Serial.println();
    delay(loopDelay);
  }

  // animate
  offset++;
  if (offset > 10) {
    offset = 0;
  };
}

int thisNote = 0;

void playMusic(int melody[], int tempo[], int size) {
  if (thisNote >= size) {
    thisNote = 0;
  }

  // to calculate the note duration, take one second
  // divided by the note type.
  //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
  int noteDuration = 1000 / tempo[thisNote];

  buzz(melody[thisNote], noteDuration);

  // to distinguish the notes, set a minimum time between them.
  int pauseBetweenNotes = noteDuration * 0.30;
  delay(pauseBetweenNotes);

  // stop the tone playing:
  buzz(0, noteDuration);

  thisNote++;

}

void buzz(long frequency, long length) {

  long delayValue = 1000000 / frequency ; 
  long numCycles = frequency * length / 1000; 
  CircuitPlayground.playTone(frequency, numCycles);
}

int music = 0;
int numMusic = 2;

void loop() {
  int motion; 
  int brightness;
  boolean leftButtonPressed = false;
  boolean rightButtonPressed = false;
  boolean switchIsLeft = false;

  // vary the brightness depending on the motion
  motion = CircuitPlayground.motionX() + CircuitPlayground.motionY() + CircuitPlayground.motionZ();
  brightness = map(motion, -10, 10, 0, 255);
  CircuitPlayground.setBrightness(brightness);

  leftButtonPressed = CircuitPlayground.leftButton();
  rightButtonPressed = CircuitPlayground.rightButton();
  switchIsLeft = CircuitPlayground.slideSwitch();

  if (leftButtonPressed) {
    pattern++;
    if (pattern > numPatterns) {
      pattern = 0;
    }
  }

  if (rightButtonPressed) {
    music++;
    if (music >= numMusic) {
      music = 0;
    }
  }

  if (switchIsLeft) {
    if (music == 0) {
      int size = sizeof(jingle_melody) / sizeof(int);
      playMusic(jingle_melody, jingle_tempo, size);
    }
    if (music == 1) {
      int size = sizeof(wish_melody) / sizeof(int);
      playMusic(wish_melody, wish_tempo, size);
    }
  }

  animatePixelPattern(pattern, 10);

}
