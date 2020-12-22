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
// DOUT 0-2 = Pin 10, 9, 6
// Shadowed to Pin 16, 17, 22
// I.e. Pin 16/10, 17/9 and 22/6 are addressed in pairs.

// The digital out permutation table. There are 5 outs defined, so 2 more are possible but currently unused.
uint8_t doutPins[] = {
  10, 9, 6,     0, 1,   // P1: O0=Pin10, O1=Pin9, O2=Pin6, Other 2 unused
  16, 17, 20,   18, 19, // P2: O0=Pin16, O1=Pin17, O2=Pin20, Other 2 unused
};



// *** CONFIGURATION END **************************************************


// The SW version.
#define SW_VERSION "1.1.1 Cocktail Table"


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

  // Set pins initially to 50% to indicate: it's working
  setDout(0, 40, 500, 0);
  setDout(1, 20, 500, 200);
  setDout(2, 40, 500, 500);  
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
