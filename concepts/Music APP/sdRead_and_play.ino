#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>   // Audio playback library

#define SD_CS   15
#define SD_MOSI 16
#define SD_MISO 17
#define SD_SCK  18

TMRpcm tmrpcm; // audio controller

File root;

enum AppState {
  APP_MENU,
  APP_NOTEPAD,
  APP_DRAW,
  APP_MUSIC
};

String song_choice;

void setup() {
  Serial.begin(115200);

  if (!SD.begin(SD_CS)) {
    Serial.println("SD card got messed up somehow");
    while (1);
  } else {
    Serial.println("It is all set up");
  }

  tmrpcm.speakerPin = 9; // pick an available PWM pin
  tmrpcm.setVolume(5);   // 0–7 range

  root = SD.open("/");
  printDirectory(root, 0);

  Serial.println("Enter the name of a .WAV file to play:");
}

void loop() {
  // Check if user typed something in Serial
  if (Serial.available()) {
    song_choice = Serial.readStringUntil('\n');
    song_choice.trim();  // remove spaces/newlines

    if (song_choice.endsWith(".WAV")) {
      Serial.print("Attempting to play: ");
      Serial.println(song_choice);

      if (SD.exists(song_choice)) {
        tmrpcm.play(song_choice.c_str()); // convert String → C-string
        playing = true;
      } else {
        Serial.println("File not found.");
      }
    } else {
      Serial.println("Not a WAV file. Try again.");
    }
  }
}

void printDirectory(File dir, int numTabs) {
  while (true) {
    File entry = dir.openNextFile();
    if (!entry) break;  // no more files

    String filename = entry.name();
    filename.toUpperCase();

    for (uint8_t i = 0; i < numTabs; i++) Serial.print('\t');

    if (entry.isDirectory()) {
      Serial.print(filename);
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      if (filename.endsWith(".WAV")) {
        Serial.print("Audio file: ");
        Serial.println(filename);
      }
      else if (filename.endsWith(".TXT")) {
        Serial.print("Text file: ");
        Serial.println(filename);
      }
      else if (filename.endsWith(".BMP")) {
        Serial.print("Drawing file: ");
        Serial.println(filename);
      }
    }
    entry.close();
  }
}
