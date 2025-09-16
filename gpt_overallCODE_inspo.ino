#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <SD.h>

// -- TFT pins
#define TFT_CS 13
#define TFT_RST 12
#define TFT_DC 11
#define TFT_MOSI 35
#define TFT_SCK 36
#define TFT_LED 41
#define TFT_MISO 37

//-- SD CARD -- I do not have much stuff here honestly
#define SD_CS 15
#define SD_MOSI 16
#define SD_MISO 17
#define SD_SCK  18

// -- Colors
const uint16_t LIGHT_GREY = 0x8410;
const uint16_t DARK_GREY  = 0x31C6;

// -- TFT object
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);

// -- File
File file;

// -- Keyboard pins
#define SPACE     2
#define BACKSPACE 40
#define ENTER     39
const int keyboard[] = { 19, 20, 42, 41 };
const char* words[]  = { "N", "O", "T", "E" };
const char spaceChar = ' ';
const int arrows[4]  = { 21, 22, 23, 24 }; // left, up, down, right

// -- Text buffer
char sentence[256];
int sentence_point = 0;

// Cursor
int cursor_x = 0, cursor_y = 0;
#define CHAR_WIDTH   12
#define CHAR_HEIGHT  16
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320
#define SENTENCE_SIZE (SCREEN_WIDTH / CHAR_WIDTH)
#define FULL_PAGE     (SCREEN_HEIGHT / CHAR_HEIGHT)

// Blinking cursor
unsigned long lastBlink = 0;
bool cursorVisible = true;

// -- Debounce
int debounce = 200;

// -- App state machine
enum AppState {
  APP_MENU,
  APP_NOTEPAD,
  APP_DRAW,
  APP_MUSIC
};

AppState currentApp = APP_MENU;

void setup() {
  Serial.begin(115200);
  SPI.begin();

  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);

  if (!SD.begin(SD_CS)) {
    Serial.println("SD card init failed");
  } else {
    Serial.println("SD card OK");
  }

  // Keyboard
  pinMode(SPACE, INPUT_PULLUP);
  pinMode(BACKSPACE, INPUT_PULLUP);
  pinMode(ENTER, INPUT_PULLUP);
  for (int i = 0; i < 4; i++) pinMode(keyboard[i], INPUT_PULLUP);
  for (int i = 0; i < 4; i++) pinMode(arrows[i], INPUT_PULLUP);

  // TFT
  tft.begin();
  tft.setRotation(4);
  tft.fillScreen(LIGHT_GREY);
  tft.setTextColor(DARK_GREY, LIGHT_GREY);

  // Start in menu
  currentApp = APP_MENU;
}

void loop() {
  switch (currentApp) {
    case APP_MENU:   runMenu();   break;
    case APP_NOTEPAD: runNotepad(); break;
    case APP_DRAW:    runDraw();    break;
    case APP_MUSIC:   runMusic();   break;
  }
}

void runDraw(){
  Serial.println("Bruh, I have not finished the drawing thing yet");
}

void runMusic(){
  Serial.println("Again, I have not finished this music stuff yet");
}

void runMenu() {
  tft.setCursor(20, 40);
  tft.setTextSize(2);
  tft.println("Select App:");
  tft.println("1. Notepad");
  tft.println("2. Draw");
  tft.println("3. Music");

  // For now: pick app with keyboard buttons
  if (digitalRead(keyboard[0]) == LOW) { // N
    currentApp = APP_NOTEPAD;
    resetNotepad();
    delay(debounce);
  }
}

void runNotepad() {
  // Handle input
  if (digitalRead(SPACE) == LOW) {
    addChar(spaceChar);
    delay(debounce);
  }

  if (digitalRead(BACKSPACE) == LOW && sentence_point > 0) {
    backspaceChar();
    delay(debounce);
  }

  if (digitalRead(ENTER) == LOW) {
    addChar('\n');
    tft.println();
    delay(debounce);
  }

  for (int i = 0; i < 4; i++) {
    if (digitalRead(keyboard[i]) == LOW) {
      addChar(words[i][0]);
      tft.print(words[i]);
      delay(debounce);
    }
  }

  // Blink cursor
  if (millis() - lastBlink > 500) {
    cursorVisible = !cursorVisible;
    drawCursor(cursorVisible);
    lastBlink = millis();
  }

  // If page full → save and reset
  if (sentence_point >= SENTENCE_SIZE * FULL_PAGE - 1) {
    saveSentence();
    resetNotepad();
  }
}

void resetNotepad() {
  sentence_point = 0;
  sentence[0] = '\0';
  tft.fillScreen(LIGHT_GREY);
  tft.setCursor(0, 0);
}

void addChar(char c) {
  if (sentence_point < 255) {
    sentence[sentence_point++] = c;
    sentence[sentence_point] = '\0';
    tft.print(c);
  }
}

void backspaceChar() {
  sentence_point--;
  sentence[sentence_point] = '\0';
  cursor_x = (sentence_point % SENTENCE_SIZE) * CHAR_WIDTH;
  cursor_y = (sentence_point / SENTENCE_SIZE) * CHAR_HEIGHT;
  tft.fillRect(cursor_x, cursor_y, CHAR_WIDTH, CHAR_HEIGHT, LIGHT_GREY);
  tft.setCursor(cursor_x, cursor_y);
}

void drawCursor(bool visible) {
  cursor_x = (sentence_point % SENTENCE_SIZE) * CHAR_WIDTH;
  cursor_y = (sentence_point / SENTENCE_SIZE) * CHAR_HEIGHT;
  if (visible) {
    tft.fillRect(cursor_x, cursor_y, CHAR_WIDTH, CHAR_HEIGHT, DARK_GREY);
  } else {
    tft.fillRect(cursor_x, cursor_y, CHAR_WIDTH, CHAR_HEIGHT, LIGHT_GREY);
  }
}

void saveSentence() {
  file = SD.open("note.txt", FILE_APPEND);
  if (file) {
    file.println(sentence);
    file.close();
    Serial.println("Saved: " + String(sentence));
  }
}
