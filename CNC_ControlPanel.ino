/*
###############################################################################
## File: KTM_Pendant.ino
## Desc: Main Arduino program 
## Project: LaserControlSystem for Element14 Presents
## License: 
##
## By: Kaleb Clark (KalebTheMaker)
###############################################################################
* Button Notes:
*   The buttons that emulate a keyboard are setup to mimic holding down a key
*   on the keyboard. Normally here you would get a single keydown and keyup
*   event that would cycle every iteration. I used a keyboard debugger to see
*   exactly what happens on a keyboard when you hold the button down, and this
*   is what I found. There is an initial key down event, followed by repeated
*   key down events (rather than keydown, key up every cycle). This is how I am
*   sending them here. Check for presss, repeat the keydown event non-blocking
*   until released, then send the release signal.
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

// Button Pins
#define BTN_UP_PIN 3
#define BTN_DOWN_PIN 4

// Globals
const int key_press_delay = 5;
const int key_hold_delay = 30;

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
      Keyboard.press(KEY_UP_ARROW);               // Send Keyboard key down press
    }
  }

  if(btn_up.wasReleased()) {                      // Check to see if button was released since last press
    Keyboard.release(KEY_UP_ARROW);               // Release keyboard key.
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

/*
 * handleInc(long) 
 * Handles the encoder Incrementing
 */
void handleInc(long enc_pos) 
{
  Serial.print("Incremenet: "); Serial.println(enc_pos);
  Keyboard.press(KEY_LEFT_ARROW);
  delay(key_press_delay);
  Keyboard.release(KEY_LEFT_ARROW);
}

/*
 * handleDec(long)
 * Handles the encoder Decrementing
 */
void handleDec(long enc_pos)
{
  Serial.print("Decrement: "); Serial.println(enc_pos);
  Keyboard.press(KEY_RIGHT_ARROW);
  delay(key_press_delay);
  Keyboard.release(KEY_RIGHT_ARROW);
}
