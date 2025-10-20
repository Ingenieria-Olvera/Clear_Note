/*
Ight lets get it, based off of everything that i know
I must create a UI top priorty, button and screen update as second priority
third being like app states, maybe bluetooth connectivity or wifi which I don't
think they need anything
like that but still

Lets get it

*/

/*
// GxEPD_MinimumExample by Jean-Marc Zingg

#include <GxEPD.h>

// select the display class to use, only one, copy from GxEPD_Example
#include <GxGDEH0154D67/GxGDEH0154D67.h>  // 1.54" b/w

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
*/
// constructor for AVR Arduino, copy from GxEPD_Example else
// GxIO_Class io(SPI, /*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9); // arbitrary selection of 8, 9 selected for default of GxEPD_Class
 // GxEPD_Class display(io, /*RST=*/ 9, /*BUSY=*/ 7); // default selection of (9), 7

// void setup()
// {
  // display.init();
  // display.eraseDisplay();
  // comment out next line to have no or minimal Adafruit_GFX code
  // display.drawPaged(drawHelloWorld); // version for AVR using paged drawing, works also on other processors
// }

// void drawHelloWorld()
// {
  // display.setTextColor(GxEPD_BLACK);
  // display.print("Hello World!");
// }

// void loop() {};

// INITALIZORS ---------------------------------------------------------------------

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <SD.h>
#include <SPI.h>
#include <GxEPD2_3C.h>

GxEPD2_3C<GxIO_Class> display(io, /*RST=*/ 9, /*BUSY=*/ 7); // Copilot made this one for me I got no clue


void UI_HANDLER(*pvParameters);
void SCREEN_HANDLER(*pvParameters);
void KEYBOARD_HANDLER(*pvParameters);
void APP_HANDLER(*pvParameters);
void ON_STARTUP_HANDLER(*pvParameters);
void OFF_SHUTDOWN_HANDLER(
    *pvParameters); // THESE 2 ARE GOING TO BE HIGHEST IN DEMAND

volatile APP_STATE current_app_state = APP_MENU;


// ON BUTTON -- OFF BUTTON ------------------------------------------------------------------------
#define POWER_BUTTON 82
// I need to look at the P-MOSFET Things data sheet to determine if it needs speciifc pins


// Keyboard Stuff | Fake pins ------------------------------------------------------------------------
// That is now how I do it.

/*
const int keyboard_alph[29] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28
};
*/

#define COLMS 10
#define ROWS 3

const int keyboard_rows[3] = {2, 3, 4};
const int keyboard_cols[10] = {5, 6, 7, 8, 9, 10, 11, 12, 13, 14};


// Basically everything from 0-26 are letters and some other stuff are like
// enter and arrow keys and shit, I might have to change that

#define ESCAPE_KEY = 46;
#define SPACE_KEY = 32;
#define SHIFT_KEY = 33;

// Also having the ; button and maybe . and , so actually like 29
// I think I have to do the same thing here or just call [I][J] for that to make a 2 vector array
const char *Alphabet_CAPS[3][10] = {
    "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
    "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", 
    "Z", "X", "C", "V", "B", "N", "M", ",", "."};
const char spaceChar = ' ';

// --Buffers
char sentence[256];
int sentence_point = 0;

// --Screen Stuff
#define CHAR_WIDTH 12
#define CHAR_HEIGHT 16
#define SCREEN_WDITH //This numnber is made up for right now
#define SCREEN_HEIGHT
#define SENTENCE_SIZE (SCREEN_WIDTH / CHAR_WIDTH)
#define FULL_PAGE (SCREEN_HEIGHT / CHAR_HEIGHT) && (SCREEN_WIDTH / CHAR_WIDTH)

// Cursor Blinking
unsigned long lastBlink = 0; // This will probably now become a vTaskDelay(TICK)
                             // because I am now using FreeRTOS
bool cursorVisible = true;

// -- Debounce
int debounce =
    200; // miliseconds again will probablyu now have to chnage to ticks and shi



// SD CARD / AUDIOSTUFF ------------------------------------------------------------------------
#define SD_CS 10
File myFile;
// As far as I know that is it.

bool is_music_playing;

bool is_audio_jack_connected; // I have to read the status of the headphone jack
int volume_level;             // from 0 to 100 probably
#define VOLUMNE_SLIDER  A0 // Analog pin because I am reading from the potentiometer

#define AUDIO_JACK_DETECT_PIN  4 // Digital pin because its just a high or low signal

#define SKIP_BUTTON 52
#define PLAY_PAUSE_BUTTON 53
#define BACK_BUTTON 51
// These pins are just made up but it just sounds right.


// HOLOEFFECT STUFF ------------------------------------------------------------------------
#define HOLO_EFFECT_PIN                                                        \
  A1 // Maybe an alnalog signal because I am reading when something does not
     // react to a magnet

bool is_clearnote_closed; // I have to read from the hall effect sensor
                                   // whether the clear note is closed or not



// RTC STUFF ------------------------------------------------------------------------
#define RTC_SDA A4
#define RTC_SCL A5
// I honestly have no clue how to use the RTC model and I don't know if it is digital or analog



// ENUM STATES ------------------------------------------------------------------------
enum APP_STATE {
  APP_MENU,
  APP_NOTEPAD,
  APP_SETTINGS,
  APP_MUSIC,
  APP_CALENDER,
  APP_TIMER,
  APP_CALCULATOR,
  APP_WEATHER,
  APP_GAMES
}

// BATTERY INFO ------------------------------------------------------------------------
bool is_battery_charged;
int battery_percentage; // I get all this info from reading the charging unti
                        // with the D+ and D- pins
// I HAVE TO TALK WITH THE MCP73871-2AAI/ML STate pins and all dat


void setup() {
  // SPI STUFF | General Start -----------------------------------------------------------------------------
  SPI.begin(); // I don't know if that really is what I put here but yuh
  Serial.begin(115200);
  pinMode(POWER_BUTTON, INPUT_PULLUP);
  Serial.println("SPI and Serial Setup");
  // THIS WILL ALSO HOLD MY STUFF FOR SOMETHING I FORGOT 

  
  // KEYBOARD SETUP -----------------------------------------------------------------
  for(int i = 0; i <= ROWS; i++) {
    pinMode(keyboard_rows[i], INPUT_PULLUP); // I think even with the diodes that are there to prevennt my shit to ghost I should still have a pull up pin here in the code
  }
  for (int j = 0; j <= COLMS; i++) {
    pinMode(keyboard_cols[j], INPUT_PULLUP);
  }
  pinMode(SPACE_KEY, INPUT_PULLUP);
  pinMode(ESCAPE_KEY, INPUT_PULLUP);
  pinMode(SHIFT_KEY, INPUT_PULLUP);

  Serial.println("Keyboard is all setup");
  
  // SCREEN SETUP -----------------------------------------------------------------------------
  display.init();
  display.setTextColor(GxEPD_BLACK);
  // id probably have it run like Welcome to clear note with a red bird or
  // something as like a power on screen and only when powered on after a full
  // turn off not like when I open ts
  Serial.println("Display Setup");

  // SD STUFF ---
  if (!SD.begin(SD_CS)) {
    Serial.prnitln("SD Card cant initialize");
  }
  // I don't open a thing until I am asked too. Because once I click on notepad
  // I am brough to a place to either make a new note and give it a name or go
  // back and mess with one I have already made
  Serial.println("SD Card Setup");

  // AUDIO SETUP ----------------------------------------------------------------------------
  pinMode(VOLUMNE_SLIDER, INPUT);
  pinMode(AUDIO_JACK_DETECT_PIN, INPUT);
  pinMode(SKIP_BUTTON, INPUT_PULLUP);
  pinMode(PLAY_PAUSE_BUTTON, INPUT_PULLUP);
  pinMode(BACK_BUTTON, INPUT_PULLUP);
  Serial.println("Audio Setup");

  // HOLOEFFECT SETUP ----------------------------------------------------------------------------
  pinMode(HOLO_EFFECT_PIN, INPUT);
  Serial.println("Holoeffect Setup");
  // Since I can turn this on and off while closed I should check
  if(HOLO_EFFECT_PIN == HIGH) { // Again because of input_PULLUP the inverse is true so high is our new low
    is_clearnote_closed = false;
  } else {
    is_clearnote_closed = true;
  }
  // RTC SETUP
  // ----------------------------------------------------------------------------
  Serial.println("RTC Setup");

  // BATTERY SETUP ----------------------------------------------------------------------------
  Serial.println("Battery Setup");
  // analog or digital read the battery stuff

  // I think that this is all the setup I need for now
  // And now to work on the tasks

  // FreeRTOS Sync stuff
  // ---------------------------------------------------------
  xTaskCreate(ON_STARTUP_HANDLER, "Turn on Task", 2048, NULL, 4,
              NULL); // Honestly if there is a 4 then that should be 4 but if 3
                     // is top dog then its fine
  xTaskCreate(OFF_SHUTDOWN_HANDLER, "Turn off Task", 2048, NULL, 4, NULL);
  // I think that I also have to add in the if menu in esc to turn everything
  // off, if in meny esc = turn off but in apps esc makes it so that you can
  // choose different apps and it just suspends the actual app you are currently
  // in

  xTaskCreate(UI_HANDLER, "Handels the UI", 2048, NULL, 3, NULL); // Thse might actually not be null especially the screen handler
  xTaskCreate(SCREEN_HANDLER, "Handels Screen Updates", 2048, NULL, 2, NULL);
  xTaskCreate(KEYBOARD_HANDLER, "Handels Keyboard Inputs", 2048, NULL, 2, NULL);
  xTaskCreate(APP_HANDLER, "Handels App States", 2048, NULL, 1, NULL);


}

void loop() {}

void UI_HANDLER(*pvParameters) {
  (void) pvParameters;
  for(;;) {
    
  }
}

void SCREEN_HANDLER(*pvParameters) {
  (void) pvParameters;
  for(;;) {
    // Handle Screen stuff here
  }
}

void KEYBOARD_HANDLER(*pvParameters) {
  (void) pvParameters;
  for(;;) {
    // Handle Keyboard stuff here
  }
}

void APP_HANDLER(*pvParameters) {
  (void) pvParameters;
  for(;;) {
    switch(current_app_state) {
      case APP_MENU:
        menu_code();
        break;
      case APP_NOTEPAD:
        // notepad_code();
        break;
      case APP_SETTINGS:
        // settings_code();
        break;
      case APP_MUSIC:
        // music_code();
        break;
      case APP_CALENDER:
        // calender_code();
        break;
      case APP_TIMER:
        // timer_code();
        break;
      case APP_CALCULATOR:
        // calculator_code();
        break;
      case APP_WEATHER:
        // weather_code();
        break;
      case APP_GAMES:
        // games_code();
        break;
      default:
            run_menu_code();
        break;
      }
      vTaskDelay(pdMS_TO_TICKS(10)); // yield a bit
  }
}

void ON_STARTUP_HANDLER(*pvParameters) {
  (void) pvParameters;
  for (;;) {
    // Handle On Startup stuff here
    // Honestly this is just everything that is currently in the setup code so I
    // could just move that al here
    
  }
}

void OFF_SHUTDOWN_HANDLER(*pvParameters) {
  (void)  pvParameters;
  for (;;) {
    int save_task = 500; // Because I want this to take about 500 ms to save
    vTaskDelay(pdMS_TO_TICKS(save_task)); // Yield to other tasks
    // Handle Off Shutdown stuff here
    // Here in the shutdown we just have to save everything
    myFile.close(); // This will have to shut down and interup the audio code
                    // too because that has to get turned off
    vTaskDelay(pdMS_TO_TICKS(save_task));

    display.drawBitmap(0, 0, shutdown_image_width, shutdown_image_height,
                       shutdown_image_data); // Display shutdown image
    // This image is going to be 1 of x amount of random backgrounds that I
    // want. I am imagning if this was already finished
    vTaskDelay(pdMS_TO_TICKS(save_task));
    display.off(); // Turn off the display

    // SD off display off, RTC probably also has to go off or maybe that
    // just stays on indefintly since it is being powered by the coid battery. I
    // honestly do not know
    // Bascially I am just gonna have everything turn off here
    // Along with having everything save to either the SD card or something
    // Unless
    if (is_music_playing && is_audio_connected == true) {
      // You keep everything playing additionally in this off thing I guess I
      // would have the holoaffect sensor also do all this power off stuff while
      // also ignoring the sd card unless
    }
    if (is_music_playing == true && is_audio_connected == false ||
        is_music_playing == false && is_audio_connected == true) {
      // everything turns off because if the connection is not there turn off
      // music playing.
      // If music is off but connection jack is connected then there is no need
      // to worry about playing music since music is turned off
      
    }
  }
}


void background_images() {
  // I will have an bit map where I can make custom background stuff that will
  // also be inside of on start
}

void menu_code() {
  // As far as I understand the menu stuff might need to be inside of a thing
  // here instead of inside of a task but I have no clue. I could be wrong
}

void notepad_code() {
  // Notepad code here
}



/*
I think I have the weird skeleton of everything inside of 1 huge function , but
in reality I will probably have other .hs especially the bitmaps and probably
I'd have custom .cpp files too maybe
Honestly I am not too too sure of everything that I am missing and everything that I have so imma just send it to gpt and see what its all looking like

*/
