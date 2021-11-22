/*
###############################################################################
## File: KTM_Pendant.ino
## Desc: Main Arduino program 
## Project: LaserControlSystem for Element14 Presents
## License: GNU General Public License V3
## URL: https://github.com/KalebTheMaker/CNC_ControlPanel
##
## By: Kaleb Clark (KalebTheMaker)
###############################################################################
*/
#include <Arduino.h>
#include <RotaryEncoder.h>
#include <NoDelay.h>
#include <Keyboard.h>
#include <EasyButton.h>

// Encoder Pins
#define ENC_PIN1 9
#define ENC_PIN2 10

// Rotary Switch Pins
#define ROT_POS_0 14
#define ROT_POS_1 15
#define ROT_POS_2 16
#define ROT_POS_3 17

// Button Pins
#define BTN_UP_PIN 3
#define BTN_DOWN_PIN 4
#define BTN_LEFT_PIN 5
#define BTN_RIGHT_PIN 6
#define BTN_PGUP_PIN 7
#define BTN_PGDN_PIN 8

// LED Pins
#define LED_RED_PIN 18
#define LED_BLUE_PIN 19
#define LED_GREEN_PIN 20

// Toggle Switch
#define TOG_SWITCH 13

// Globals
const int key_press_delay = 10;
const int key_hold_delay = 30;
int enc_axis = -1;

// Instantiate Objects
//KTM_Keyboard kb;
RotaryEncoder *encoder = nullptr;
EasyButton btn_up(BTN_UP_PIN, 40, true, true);
EasyButton btn_down(BTN_DOWN_PIN, 40, true, true);
EasyButton btn_right(BTN_RIGHT_PIN, 40, true, true);
EasyButton btn_left(BTN_LEFT_PIN, 40, true, true);
EasyButton btn_pgup(BTN_PGUP_PIN, 40, true, true);
EasyButton btn_pgdn(BTN_PGDN_PIN, 40, true, true);

noDelay upHoldDelay(key_hold_delay);
//noDelay upPressDelay(key_press_delay);
noDelay downHoldDelay(key_hold_delay);
noDelay leftHoldDelay(key_hold_delay);
noDelay rightHoldDelay(key_hold_delay);
noDelay pgupHoldDelay(key_hold_delay);
noDelay pgdnHoldDelay(key_hold_delay);


// Encoder Callback
void checkPos() { encoder->tick(); }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Keyboard Setup
  //kb.init();
  Keyboard.begin();

  // Rotary Switch
  pinMode(ROT_POS_0, INPUT_PULLUP);
  pinMode(ROT_POS_1, INPUT_PULLUP);
  pinMode(ROT_POS_2, INPUT_PULLUP);
  pinMode(ROT_POS_3, INPUT_PULLUP);

  // RGB LED
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(LED_BLUE_PIN, LOW);

  // Toggle Switch
  pinMode(TOG_SWITCH, INPUT_PULLUP);
  
  // Buttons
  btn_up.begin();
  btn_down.begin();
  btn_left.begin();
  btn_right.begin();
  btn_pgup.begin();
  btn_pgdn.begin();

  // Encoder Setup
  encoder = new RotaryEncoder(ENC_PIN1, ENC_PIN2, RotaryEncoder::LatchMode::FOUR0);
  attachInterrupt(digitalPinToInterrupt(ENC_PIN1), checkPos, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_PIN2), checkPos, CHANGE);

}


void loop() {
  // Handle Toggle Switch
  // Doing this at top so we can return if LOW.
  if(digitalRead(TOG_SWITCH) == HIGH) {
    digitalWrite(LED_BLUE_PIN, LOW);
    digitalWrite(LED_RED_PIN, HIGH);
  } else if(digitalRead(TOG_SWITCH) == LOW) {
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_BLUE_PIN, HIGH);
    return;
  }
  
  // Handle Rotary Switch ==================================================================
  if(digitalRead(ROT_POS_0) == LOW) { enc_axis = 0; } else  // X
  if(digitalRead(ROT_POS_1) == LOW) { enc_axis = 1; } else  // Y
  if(digitalRead(ROT_POS_2) == LOW) { enc_axis = 2; } else  // Z
  if(digitalRead(ROT_POS_3) == LOW) { enc_axis = 3; }       // A
  // END Handle Rotary Switch ==============================================================
  
  // Handle Encoder ========================================================================
  long enc_pos = 0;                               // Encoder Position variable
  encoder->tick();                                // tick the encoder
  long enc_new_pos = encoder->getPosition();      // Get new encoder Position

  // Check to see if encoder value is different
  if(enc_pos != enc_new_pos) {
    int dir = (int)(encoder->getDirection());     // Get Direction as Integer

    if(dir < 0) { handleInc(enc_new_pos); } else  // Handle Increment
    if(dir > 0) { handleDec(enc_new_pos); }       // Handle Decrement

    enc_pos = enc_new_pos;                        // Reset Position
  }
  // End Handle Encoder ====================================================================
  
  // Handle Buttons ========================================================================
  btn_up.read();
  btn_down.read();
  btn_left.read();
  btn_right.read();
  btn_pgup.read();
  btn_pgdn.read();

  // Button Up -------------------------------------
  if(btn_up.isPressed()) {                        // Check to see if button is pressed
    if(upHoldDelay.update()) {                    // Check to see if non-blocking time has passed
      Serial.println("Button Up Pressed");
      Keyboard.press(KEY_UP_ARROW);
    }
  }

  if(btn_up.wasReleased()) {
    Serial.println("Button Up Released");
    Keyboard.release(KEY_UP_ARROW);
  }

  // Button Down -----------------------------------
  if(btn_down.isPressed()) {
    if(downHoldDelay.update()) {
      Serial.println("Button down Pressed");
      Keyboard.press(KEY_DOWN_ARROW);
    }
  }

  if(btn_down.wasReleased()) {
    Keyboard.release(KEY_DOWN_ARROW);
  }

  // Button Right -----------------------------------
  if(btn_right.isPressed()) {
    if(rightHoldDelay.update()) {
      Serial.println("Button right Pressed");
      Keyboard.press(KEY_RIGHT_ARROW);
    }
  }

  if(btn_right.wasReleased()) {
    Keyboard.release(KEY_RIGHT_ARROW);
  }

  // Button Left -----------------------------------
  if(btn_left.isPressed()) {
    if(leftHoldDelay.update()) {
      Serial.println("Button left Pressed");
      Keyboard.press(KEY_LEFT_ARROW);
    }
  }

  if(btn_left.wasReleased()) {
    Keyboard.release(KEY_LEFT_ARROW);
  }

  // Button Page Up -----------------------------------
  if(btn_pgup.isPressed()) {
    if(pgupHoldDelay.update()) {
      Serial.println("Button pgup Pressed");
      Keyboard.press(KEY_PAGE_UP);
    }
  }

  if(btn_pgup.wasReleased()) {
    Keyboard.release(KEY_PAGE_UP);
  }

  // Button Page Down -----------------------------------
  if(btn_pgdn.isPressed()) {
    if(pgdnHoldDelay.update()) {
      Serial.println("Button pgdn Pressed");
      Keyboard.press(KEY_PAGE_DOWN);
    }
  }

  if(btn_pgdn.wasReleased()) {
    Keyboard.release(KEY_PAGE_DOWN);
  }

  
}

/*
 * ctrlKeyPress(uint8_t)
 * Sequence for control modifier
 */
void ctrlKeyPress(uint8_t k)
{
  Keyboard.press(KEY_RIGHT_CTRL);
  delay(2);
  Keyboard.press(k);
  delay(key_press_delay);
  Keyboard.releaseAll();
}

/*
 * handleInc(long) 
 * Handles the encoder Incrementing
 */
void handleInc(long enc_pos) 
{
  Serial.println(enc_axis);
  switch (enc_axis) {
    case 0: // X+
      ctrlKeyPress(KEY_RIGHT_ARROW);
      break;
    case 1: // Y+
      ctrlKeyPress(KEY_UP_ARROW);
      break;
    case 2: // Z+
      ctrlKeyPress(KEY_PAGE_UP);
      break;
  }
}

/*
 * handleDec(long)
 * Handles the encoder Decrementing
 */
void handleDec(long enc_pos)
{
  Serial.println(enc_axis);
  switch (enc_axis) {
    case 0: // X-
      ctrlKeyPress(KEY_LEFT_ARROW);
      break;
    case 1: // Y-
      ctrlKeyPress(KEY_DOWN_ARROW);
      break;
    case 2: // Z-
      ctrlKeyPress(KEY_PAGE_DOWN);
      break;
  }
}
