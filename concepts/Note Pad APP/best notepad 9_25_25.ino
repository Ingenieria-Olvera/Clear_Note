#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
// #include <SD.h>

// --- TFT pins (⚠️ confirm wiring!)
#define TFT_CS   13
#define TFT_RST  12
#define TFT_DC   11
#define TFT_MOSI 35
#define TFT_SCK  36
#define TFT_LED  41
#define TFT_MISO 37

const uint16_t LIGHT_GREY = 0x8410;
const uint16_t DARK_GREY  = 0x31C6;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);

// --- Keyboard pins
#define SPACE     2
#define BACKSPACE 40
#define ENTER     39
const int keyboard[] = { 19, 20, 42, 41 };    // demo keys
const char* words[]  = { "N", "O", "T", "E" };
const char spaceChar = ' ';

// --- Buffers
char sentence[256];
int sentence_point = 0;

// --- Cursor math
int cursor_x = 0, cursor_y = 0;
#define CHAR_WIDTH   12
#define CHAR_HEIGHT  16
#define SCREEN_WIDTH 240    // ILI9341 is 320x240, but we’ll use portrait
#define SCREEN_HEIGHT 320
#define SENTENCE_SIZE (SCREEN_WIDTH / CHAR_WIDTH)
#define FULL_PAGE     (SCREEN_HEIGHT / CHAR_HEIGHT)

// --- Cursor blinking
unsigned long lastBlink = 0;
bool cursorVisible = true;

// --- Debounce
int debounce = 200;

// --- App states
enum AppState { APP_MENU, APP_NOTEPAD, APP_DRAW, APP_MUSIC };
AppState currentApp;

// ------------------ SETUP ------------------
void setup() {
  Serial.begin(115200);
  SPI.begin();

  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

  pinMode(SPACE, INPUT_PULLUP);
  pinMode(BACKSPACE, INPUT_PULLUP);
  pinMode(ENTER, INPUT_PULLUP);
  for (int i = 0; i < 4; i++) pinMode(keyboard[i], INPUT_PULLUP);

  tft.begin();
  tft.setRotation(4);
  tft.fillScreen(LIGHT_GREY);
  tft.setTextColor(DARK_GREY, LIGHT_GREY);

  currentApp = APP_MENU;
}

// ------------------ LOOP ------------------
void loop() {
  switch (currentApp) {
    case APP_MENU:    runMenu();    break;
    case APP_NOTEPAD: runNotepad(); break;
    case APP_DRAW:    Serial.println("TODO: Draw App");  break;
    case APP_MUSIC:   Serial.println("TODO: Music App"); break;
  }
}

// ------------------ MENU ------------------
void runMenu() {
  tft.setCursor(20, 40);
  tft.setTextSize(2);
  tft.println("Select App:");
  tft.println("1. Notepad");
  tft.println("2. Draw");
  tft.println("3. Music");

  if (digitalRead(keyboard[0]) == LOW) { // key "N"
    currentApp = APP_NOTEPAD;
    resetNotepad();
    delay(debounce);
  }
}

// ------------------ NOTEPAD ------------------
void runNotepad() {
  // SPACE
  if (digitalRead(SPACE) == LOW && sentence_point < 255) {
    sentence[sentence_point++] = spaceChar;
    sentence[sentence_point]   = '\0';
    tft.print(spaceChar);
    delay(debounce);
  }

  // BACKSPACE
  if (digitalRead(BACKSPACE) == LOW && sentence_point > 0) {
    sentence_point--;
    sentence[sentence_point] = '\0';

    cursor_x = (sentence_point % SENTENCE_SIZE) * CHAR_WIDTH;
    cursor_y = (sentence_point / SENTENCE_SIZE) * CHAR_HEIGHT;
    // maybe at an at edge thing? Where at edge it goes back to the max X character and 1 minus y type shit. With y being one character size
    tft.fillRect(cursor_x, cursor_y, CHAR_WIDTH, CHAR_HEIGHT, LIGHT_GREY);
    tft.setCursor(cursor_x, cursor_y);
    delay(debounce);
  }

  // ENTER
  if (digitalRead(ENTER) == LOW && sentence_point < 255) {
    sentence[sentence_point++] = '\n';
    sentence[sentence_point]   = '\0';
    tft.println();
    delay(debounce);
  }

  // Letter keys
  for (int i = 0; i < 4; i++) {
    if (digitalRead(keyboard[i]) == LOW && sentence_point < 255) {
      sentence[sentence_point++] = words[i][0];
      sentence[sentence_point]   = '\0';
      tft.print(words[i]);
      delay(debounce);
    }
  }

  // Blinking cursor
  if (millis() - lastBlink > 500) {
    cursorVisible = !cursorVisible;
    drawCursor(cursorVisible);
    lastBlink = millis();
  }
}

// ------------------ HELPERS ------------------
void resetNotepad() {
  sentence_point = 0;
  sentence[0] = '\0';
  tft.fillScreen(LIGHT_GREY);
  tft.setCursor(0, 0);
}

void drawCursor(bool visible) {
  cursor_x = (sentence_point % SENTENCE_SIZE) * CHAR_WIDTH;
  cursor_y = (sentence_point / SENTENCE_SIZE) * CHAR_HEIGHT;
  if (visible)
    tft.fillRect(cursor_x, cursor_y, CHAR_WIDTH, CHAR_HEIGHT, DARK_GREY);
  else
    tft.fillRect(cursor_x, cursor_y, CHAR_WIDTH, CHAR_HEIGHT, LIGHT_GREY);
}



/*
Things that don't worry. They are mainly do with the blinking code.
I honestly think that instead an if statement I do a while statement. So while nothing is happenning then blink 
But while something is happening it doesn't blink type shit. 
But it's already in a loop because the while loop would that be needdd?

So the enter works but the blinking cursor does not follow it. 
That is the same for the backspace code stuff.

*/
