#include <Arduino.h>
#include <WiFi.h>
#include <IRremote.h>
#include "SinricPro.h"
#include "SinricProTV.h"
#include "SinricProSpeaker.h"

// ================= WIFI & SINRIC =================
#define WIFI_SSID       "K506_Asus_Rx6G"
#define WIFI_PASSWORD   "50661466"

#define APP_KEY         "e4fc67cd-d140-4ccf-a4a0-27fcc4dda606"
#define APP_SECRET      "8b247dbc-b5e9-45d3-b79b-cc7aaf42eb57-55fe43a3-a22f-4ab2-b5b8-0544f258ea5e"

#define TV_ID           "6940d2e56dbd335b28f511c6"
#define SPEAKER_ID      "6940d4b66ebb39d664ce0473"

// ================= IR CONFIG =====================
#define IR_SEND_PIN     4

// -------- Samsung TV (Samsung protocol) ----------
#define IR_CMD_POWER    0x02
#define IR_CMD_VOL_UP   0x07
#define IR_CMD_VOL_DN   0x0B
#define IR_CMD_MUTE     0x0F
#define IR_CMD_SOURCE   0x01
#define IR_CMD_CH_UP    0x12
#define IR_CMD_CH_DN    0x10
#define IR_CMD_PLAY     0x47
#define IR_CMD_PAUSE    0x4A
#define IR_CMD_FF       0x48
#define IR_CMD_REWIND   0x45
#define IR_CMD_MEDIA_PL 0x8C

#define SAMSUNG_ADDR    0x0707   // safer than 0x7

// -------- Speaker (NEC protocol) -----------------
#define SPK_CMD_PLAY_PAUSE 0x19
#define SPK_CMD_MODE       0x40
#define SPK_CMD_BLUETOOTH  0x47
#define SPK_CMD_VOL_UP     0x0D
#define SPK_CMD_VOL_DN     0x16
#define SPK_CMD_BACK       0x18
#define SPK_CMD_FORWARD    0x15

#define SPEAKER_ADDR       0x00

// ================= TV CALLBACKS ==================
bool onPowerState(const String &, bool &) {
  IrSender.sendSamsung(SAMSUNG_ADDR, IR_CMD_POWER, 0);
  return true;
}

bool onAdjustVolume(const String &, int &delta, bool) {
  int presses = max(1, abs(delta) / 5);
  uint8_t cmd = (delta > 0) ? IR_CMD_VOL_UP : IR_CMD_VOL_DN;

  for (int i = 0; i < presses; i++) {
    IrSender.sendSamsung(SAMSUNG_ADDR, cmd, 0);
    delay(100);
  }
  return true;
}

bool onMute(const String &, bool &) {
  IrSender.sendSamsung(SAMSUNG_ADDR, IR_CMD_MUTE, 0);
  return true;
}

bool onSelectInput(const String &, String &input) {
  input.toLowerCase();
  if (input == "media player") {
    IrSender.sendSamsung(SAMSUNG_ADDR, IR_CMD_MEDIA_PL, 0);
  } else {
    IrSender.sendSamsung(SAMSUNG_ADDR, IR_CMD_SOURCE, 0);
  }
  return true;
}

bool onMediaControl(const String &, String &control) {
  control.toLowerCase();

  if (control == "play") {
    IrSender.sendSamsung(SAMSUNG_ADDR, IR_CMD_PLAY, 0);
  } else if (control == "pause") {
    IrSender.sendSamsung(SAMSUNG_ADDR, IR_CMD_PAUSE, 0);
  } else if (control == "fastforward") {
    IrSender.sendSamsung(SAMSUNG_ADDR, IR_CMD_FF, 0);
  } else if (control == "rewind") {
    IrSender.sendSamsung(SAMSUNG_ADDR, IR_CMD_REWIND, 0);
  } else if (control == "next") {
    IrSender.sendSamsung(SAMSUNG_ADDR, IR_CMD_CH_UP, 0);
  } else if (control == "previous") {
    IrSender.sendSamsung(SAMSUNG_ADDR, IR_CMD_CH_DN, 0);
  }
  return true;
}

bool onSetVolume(const String &, int &) {
  return true;
}

// ================= SPEAKER CALLBACKS ==============
bool onSpkPowerState(const String &, bool &state) {
  if (state) {
    IrSender.sendNEC(SPEAKER_ADDR, SPK_CMD_MODE, 0);
  }
  return true;
}

bool onSpkAdjustVolume(const String &, int &delta, bool) {
  int presses = max(1, abs(delta) / 5);
  uint8_t cmd = (delta > 0) ? SPK_CMD_VOL_UP : SPK_CMD_VOL_DN;

  for (int i = 0; i < presses; i++) {
    IrSender.sendNEC(SPEAKER_ADDR, cmd, 0);
    delay(100);
  }
  return true;
}

bool onSpkMute(const String &, bool &) {
  return true;
}

bool onSpkSelectInput(const String &, String &input) {
  input.toLowerCase();
  if (input == "bluetooth") {
    IrSender.sendNEC(SPEAKER_ADDR, SPK_CMD_BLUETOOTH, 0);
  } else {
    IrSender.sendNEC(SPEAKER_ADDR, SPK_CMD_MODE, 0);
  }
  return true;
}

bool onSpkMediaControl(const String &, String &control) {
  control.toLowerCase();

  if (control == "play" || control == "pause") {
    IrSender.sendNEC(SPEAKER_ADDR, SPK_CMD_PLAY_PAUSE, 0);
  } else if (control == "fastforward" || control == "next") {
    IrSender.sendNEC(SPEAKER_ADDR, SPK_CMD_FORWARD, 0);
  } else if (control == "rewind" || control == "previous") {
    IrSender.sendNEC(SPEAKER_ADDR, SPK_CMD_BACK, 0);
  }
  return true;
}

bool onSpkSetVolume(const String &, int &) {
  return true;
}

// ================= SETUP =========================
void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(500);
  }
}

void setupSinricPro() {
  SinricProTV &tv = SinricPro[TV_ID];
  tv.onPowerState(onPowerState);
  tv.onAdjustVolume(onAdjustVolume);
  tv.onSetVolume(onSetVolume);
  tv.onMute(onMute);
  tv.onMediaControl(onMediaControl);
  tv.onSelectInput(onSelectInput);

  SinricProSpeaker &spk = SinricPro[SPEAKER_ID];
  spk.onPowerState(onSpkPowerState);
  spk.onAdjustVolume(onSpkAdjustVolume);
  spk.onSetVolume(onSpkSetVolume);
  spk.onMute(onSpkMute);
  spk.onMediaControl(onSpkMediaControl);
  spk.onSelectInput(onSpkSelectInput);

  SinricPro.begin(APP_KEY, APP_SECRET);
}

// ================= MAIN ==========================
void setup() {
  IrSender.begin(IR_SEND_PIN);
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}
