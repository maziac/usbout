/* 
USB Joystick using a Teensy LC board.
The features are:
- Requested 1ms USB poll time
- Additional delay of max. 200us
- Indication of the real used USB poll time
*/

#include <Arduino.h>

#include <usb_desc.h>
#include <usb_dev.h>


// *** CONFIGURATION BEGIN ************************************************
// Cocktail table configuration. I.e. for 2 players.
// DOUT 0-2 = Pin 16, 17, 22
// Shadowed to Pin 10, 9, 6
// I.e. Pin 16/10, 17/9 and 22/6 are addressed in pairs.

// The digital out permutation table. Logical outs [0;3] are mapped to physical pins.
uint8_t doutPins[] = { 
  16, 17, 20,   // Player 1 LEDs
  10, 9, 6      // Player 2 LEDs
};  // Use pins capable of PWM



// *** CONFIGURATION END **************************************************


// The SW version.
#define SW_VERSION "1.0"


// Uncomment to allow logging.
//#define ENABLE_LOGGING



// ASSERT macro
#define ASSERT(cond)  {if(!(cond)) error(__FILE__ ", LINE " TOSTRING(__LINE__) ": ASSERT(" #cond ")");}
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)




// SETUP
void setup() {
  // Initialize outs
  initDout();
}




// MAIN LOOP
void loop() {
    
  // Handle serial in
  handleSerialIn();

  // Handle digital out
  handleDout();

  // Call handleDout approx every 1 ms
  delay(1);
  
}
