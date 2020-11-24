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
// DOUT 0-3 = Pin 16, 17, 22, 23

// The digital out permutation table. Logical outs [0;3] are mapped to physical pins.
uint8_t doutPins[] = {16, 17, 22, 23}; // Use pins capable of PWM

// *** CONFIGURATION END **************************************************


// The SW version.
#define SW_VERSION "1.1"


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
