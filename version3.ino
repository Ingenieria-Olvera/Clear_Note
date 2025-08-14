/*--
THIS VERISON HOLDS MY NOTES FOR WHAT I NEED TO DO NEXT, ALONG WITH A SIMPLE BACKSPACE, ENTER, SENTENCE POINTER STUFF, AND SD CARD SAVING
--*/

#include <SD.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
// #include <SD.h>
#include <SPI.h>
//-- ILI9341 ---
#define TFT_CS 13
#define TFT_RST 12
#define TFT_DC 11

//--Some colors for this
const uint16_t LIGHT_GREY = 0x8410;
const uint16_t DARK_GREY = 0x31C6;
const uint16_t WHITE = 0xF79E;
// Adafruit_ILI9341 tft(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST, TFT_MISO);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
//-- SD CARD -- I do not have much stuff here honestly
#define SD_CS 15
#define SD_MOSI 37
#define SD_SCK 36
#define SD_MISO 35
//-- KEYBOARD
const char* words[] = { "N", "O", "T", "E" };
// In real life it'd be like = {"A", "B", ...whole alphabet...};
const char space[] = { " " };
#define SPACE 2
//Back space is the first long one
#define BACKSPACE 40
//Enter is the second long one
#define ENTER 39
// const char* enter = '\n';
//then a backspace feature
// I also have to define each pin no?
// NO, Since it is columns an rows. I should change my wiring. At a later date right now imma just see if this works

const char keyboard[] = { 19, 20, 42, 41 };
int switchState_space;
int switchState_keys;
int switchState_enter;
int switchState_backspace;
// debounce
float debounce = 200;

int sentence_point;
char sentence[256];

int small_sentnece_pointer;
char small_sentence[] = {};  // does this have to be infinate or undifned since the given point that a person can indent is going to be determined in whatever is. Nvrm I got it

//320x240

#define SCREEN_WIDTH 320

#define CHAR_WIDTH  12
#define CHAR_HEIGHT 16
#define MAX_CHARS_PER_LINE (SCREEN_WIDTH / CHAR_WIDTH)  // e.g., 480 / 12 = 40

int cursor_x;
int cursor_y;

File file;  //  sd card stuff
// So i just add it in here, I just have to copy paste this into the setup already delcared.

void setup() {
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);
  delay(200);
  Serial.begin(115200);
  pinMode(SPACE, INPUT_PULLUP);
  for (int i = 0; i < 4; i++) {
    pinMode(keyboard[i], INPUT_PULLUP);
  }


  tft.begin();
  tft.setCursor(20, 20);
  tft.setTextSize(2);
  tft.setRotation(4);
  tft.setTextColor(DARK_GREY);
  tft.fillScreen(LIGHT_GREY);
  Serial.print("DONE INITALIZING");
  delay(10);
  Serial.println("Initialising SD Card...");
  if (!SD.begin(4)) {  // THIS IS THE COMMAND PIN THE THING IN HERE
    Serial.println("Initalise SD Failed");
    return;
  }

  Serial.println("Initialise SD Successful!");

  Serial.println("Creating File...");
  tft.setCursor(0, 0);  // That way it starts top left and I can start typing from there.

  // I don't think in actual clear note I should make a new file off the bat, since there will be a ui in the beginning asking what you would like to do.

  // those three options being music, drawing, notes, previous notes / drawings. YURT

  // file = SD.open("clearnote.txt", FILE_WRITE); I guess I could have a void thing for this everytime I have a new screen to make a new log along with a new notes thing
  // file.close();

  Serial.println("File Created.");
}

void loop() {  // then you need a null thing after everything so
  if (sentence_point < 255) {
    switchState_enter = digitalRead(ENTER);
    if (switchState_enter == LOW) {
      // sentence_point = 0; // since each enter means a new sentence, it gets tied to where it is, yet saving it would look like.
      // Something here, '\n' eneter
      // Then the rest of your sentence down here? I think so, so that would not have problems when writting it. Or yes?
      sentence[sentence_point++] = '\n';
      sentence[sentence_point] = '\0';  // Holds this in the whole thing.
      // This adds this to the whole sentnece. And I do think that would be easier so maybe I should just not worry about this honestly. and instead just have it as I do currently.
      // Yet then I just have to worry about the sd card stuff which is pretty chill lowkey once its all filled up. Nah nvrm im figuring this out lil bro.
      //small_sentence[] = sentence[sentnece_point]; // So what this does is it holds onto the last thing that was saved as a, just in case.
      // Than after the just in case, for whatever reason it'll just hold onto this. Yeah, and I do not send this to the sd card cause that would be OD, but
      // Just saving it internally would be good yurt.

      // So the idea is that a mini buffer so like, small_sentnece is going to be made so if new line is made than a small_sentence is stored and if anything happens. Than
      // I send that small_sentence to the sd card to save. YURT
      // enter_point = // 20 letters can fit onto a single line before having to make a new line.

      tft.print("\n");
      delay(debounce);
    }

    // ENTER --
    switchState_space = digitalRead(SPACE);
    if (switchState_space == LOW) {
      sentence[sentence_point++] = space[0];
      sentence[sentence_point] = '\0';
      tft.print(space[0]);
      delay(debounce);
    }

    switchState_backspace = digitalRead(BACKSPACE);
    if (switchState_backspace == LOW) {
      sentence_point--;                 // move back one character in array
      sentence[sentence_point] = '\0';  // null terminate string

      // Recalculate cursor position
      cursor_x = (sentence_point % MAX_CHARS_PER_LINE) * CHAR_WIDTH;
      cursor_y = (sentence_point / MAX_CHARS_PER_LINE) * CHAR_HEIGHT;

      // Erase the character visually
      tft.fillRect(cursor_x, cursor_y, CHAR_WIDTH, CHAR_HEIGHT, LIGHT_GREY);

      // Move TFT's cursor to the new position
      tft.setCursor(cursor_x, cursor_y);

      delay(debounce);
    }
    else {
      for (int i = 0; i < 4; i++) {
        switchState_keys = digitalRead(keyboard[i]);  // again just friendly reminder that the code will have rows and column checks and not just straight up checking this.
        // so basically
        // switchState_columns = digitalRead(columns[i]);
        // different for state since there will be different number of each
        // switchState_rows = digitalRead(rows[i]); which ever is low. so in both
        // if(switchState_rows == LOW && switchState_columns == LOW){ tft.print()} // the combination of that. which i would have to figure out. Yet that is neither here nor there.
        if (switchState_keys == LOW) {
          sentence[sentence_point++] = words[i][0];
          sentence[sentence_point] = '\0';
          tft.print(words[i]);
          delay(debounce);
        }
      }
    }
  } else {  // so once the sentence_point is above my 255 or reaching there.
    Serial.println("Sentence is full");
    tft.fillScreen(LIGHT_GREY);  // this is just for right now, yet the scroll stuff would be included here.
    tft.setCursor(0, 0);
    file = SD.open("clearnote.txt", FILE_WRITE);
    if (file) {  // if file availble and able to open
      Serial.println("Saving to SD card");
      delay(100);
      file.write(sentence[sentence_point]);
      delay(100);
      file.close();
    }
    Serial.println("Everything sent to the SD");
    sentence_point = 0;
    // I might have to clean sentence[] but I am not sure how to do that. Yet this is what I have for right now. Lets see how gpt likes or dislikes it.
  }
}
