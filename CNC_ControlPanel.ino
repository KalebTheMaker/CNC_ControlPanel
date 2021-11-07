/*
###############################################################################
## File: KTM_Pendant.ino
## Desc: Main Arduino program 
## Project: LaserControlSystem for Element14 Presents
## License: 
##
## By: Kaleb Clark (KalebTheMaker)
###############################################################################
*/
#include <Arduino.h>
#include <RotaryEncoder.h>
#include <NoDelay.h>
//#include <KTM_Keyboard.h>
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

// Toggle Switch
#define TOG_SWITCH 13

// Globals
const int key_press_delay = 5;
const int key_hold_delay = 30;
int enc_axis = -1;

// Instantiate Objects
//KTM_Keyboard kb;
RotaryEncoder *encoder = nullptr;
EasyButton btn_up(BTN_UP_PIN, 40, true, true);
EasyButton btn_down(BTN_DOWN_PIN, 40, true, true);

noDelay upHoldDelay(key_hold_delay);
noDelay upPressDelay(key_press_delay);
noDelay downHoldDelay(key_hold_delay);


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

  // Toggle Switch
  pinMode(TOG_SWITCH, INPUT_PULLUP);
  
  // Buttons
  btn_up.begin();
  //btn_up.onPressed(handleBtnUpPressed);
  btn_down.begin();
  //btn_down.onPressed(handleBtnDownPressed);

  // Encoder Setup
  encoder = new RotaryEncoder(ENC_PIN1, ENC_PIN2, RotaryEncoder::LatchMode::FOUR0);
  attachInterrupt(digitalPinToInterrupt(ENC_PIN1), checkPos, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_PIN2), checkPos, CHANGE);

}


void loop() {
  // Handle Toggle Switch
  // Doing this at top so we can return if LOW.
  if(digitalRead(TOG_SWITCH) == LOW) {
    return;
  }
  
  // Handle Rotary Switch ==================================================================
  if(digitalRead(ROT_POS_0) == LOW) { enc_axis = 0; } else
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

    if(dir > 0) { handleInc(enc_new_pos); } else  // Handle Increment
    if(dir < 0) { handleDec(enc_new_pos); }       // Handle Decrement
    
    enc_pos = enc_new_pos;                        // Reset Position
  }
  // End Handle Encoder ====================================================================
  
  // Handle Buttons ========================================================================
  btn_up.read();
  btn_down.read();

  // Button Up -------------------------------------
  if(btn_up.isPressed()) {                        // Check to see if button is pressed
    if(upHoldDelay.update()) {                    // Check to see if non-blocking time has passed
      Serial.println("Button Up Pressed");
      Keyboard.press(KEY_UP_ARROW);
//      if(upPressDelay.update()) {
//        Keyboard.release(KEY_UP_ARROW);      
//      }
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
    }
  }


  
}

void handleBtnUpPressed()
{

  
  //Serial.println("Button Up Pressed");
  //Keyboard.press(KEY_UP_ARROW);
  //delay(key_press_delay);
  //Keyboard.release(KEY_UP_ARROW);
}

void handleBtnDownPressed()
{
  Serial.println("Button Down Pressed");
  Keyboard.press(KEY_DOWN_ARROW);
  delay(key_press_delay);
  Keyboard.release(KEY_DOWN_ARROW);
}

void encoderButton() {
  
}

/*
 * handleInc(long) 
 * Handles the encoder Incrementing
 */
void handleInc(long enc_pos) 
{
  switch (enc_axis) {
    case 0: // X+
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_RIGHT_ARROW);
      delay(key_press_delay);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release(KEY_RIGHT_ARROW);
      break;
    case 1: // Y+
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_UP_ARROW);
      delay(key_press_delay);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release(KEY_UP_ARROW);
      break;
    case 2: // Z+
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_PAGE_UP);
      delay(key_press_delay);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_PAGE_UP);
      break;
  }
  //Serial.print("Incremenet: "); Serial.println(enc_pos);
  //Keyboard.press(KEY_LEFT_ARROW);
  //delay(key_press_delay);
  //Keyboard.release(KEY_LEFT_ARROW);
}

/*
 * handleDec(long)
 * Handles the encoder Decrementing
 */
void handleDec(long enc_pos)
{
  switch (enc_axis) {
    case 0: // X-
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_ARROW);
      delay(key_press_delay);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release(KEY_LEFT_ARROW);
      break;
    case 1: // Y-
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_DOWN_ARROW);
      delay(key_press_delay);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release(KEY_DOWN_ARROW);
      break;
    case 2: // Z-
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_PAGE_DOWN);
      delay(key_press_delay);
      Keyboard.release(KEY_LEFT_CTRL);
      Keyboard.release(KEY_PAGE_DOWN);
      break;
  }
  //Serial.print("Decrement: "); Serial.println(enc_pos);
  //Keyboard.press(KEY_RIGHT_ARROW);
  //delay(key_press_delay);
  //Keyboard.release(KEY_RIGHT_ARROW);
}
