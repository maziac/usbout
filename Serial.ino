#include <usb_desc.h>


// Handles the serial input.
// Via serial-in some digital output pins can be driven, e.g. for LED light of the buttons.
void handleSerialIn() {
  static char input[20];  // o12=100,65535,65535 -> 19
  static char* inpPtr = input;
  static bool skipLine = false;
  
  // Check if data available
  while(Serial.available()) {
    // Get data 
    char c = Serial.read();
#ifdef ENABLE_LOGGING
    logChar(c);
#endif

    if(c == '\r') 
      continue;   // Skip windows character
      
    if(c == '\n') {
      // End found
      // Check for skip
      if(!skipLine) {
        *inpPtr = 0;
        inpPtr = input;
        // Decode input
        decodeSerialIn(input);
      }
      // Reset
      inpPtr = input;
      skipLine = false;
    }
    else {
      // Remember character
      *inpPtr = c;   
      inpPtr++;
      // Input too long?
      if(inpPtr > (input+sizeof(input)-1)) {
        // Print warning
        Serial.println("Error: Line too long.");
        // Reset
        inpPtr = input;
        skipLine = true;
      }
    }
  }
}

  
// Takes a string from serial in and decodes it.
// Correct strings look like:
// "o7=0" or "o3=80" or "o1=255" with 255=max and 0=off.
// e.g. for setting ouput 7 to 70% brightness with an attack time of 2000ms and a delay of 1000ms.
// On host die (linux or mac) you can use e.g.:
// echo o0=70,2000,1000 > /dev/cu.usbXXXX
// Supported commands are:
// oN=X[,attack[,delay]] : Set output N to X ([0;100])
// r : Reset
// i : Info. Print version number.
void decodeSerialIn(char* inputStr) {
  // Check for command
  switch(inputStr[0]) {
    
    // Set output
    case 'o':
    {
      // Get output
      int index = inputStr[1] - '0';
      // Check
      if(index < 0 || index >= COUNT_DOUTS) {
        Serial.println("Error: index");
        break;
      }
    
      // Get value [0;100]
      const char* inp = &inputStr[3];
      int value = asciiToUint(&inp);
    
      // Get attack time
      uint16_t attackTime = 0;
      uint16_t delayTime = 0;
      if(*inp++ == ',') {
        attackTime = asciiToUint(&inp);
        // Get delay time
        if(*inp++ == ',') 
           delayTime = asciiToUint(&inp);
      }
      
      // Set pin
      setDout(index, value, attackTime, delayTime);
      
      // Set also shodow pin for 2nd player
      uint16_t shadowIndex = index + COUNT_DOUTS/2;
      if(shadowIndex >= COUNT_DOUTS)
        shadowIndex -= COUNT_DOUTS;
      setDout(shadowIndex, value, attackTime, delayTime);
      
    }
    break;    
      
    // Reset
    case 'r':
      Serial.println("Resetting");
      delay(2000);
#define RESTART_ADDR 0xE000ED0C
#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))
      WRITE_RESTART(0x5FA0004);
    break;
     
    // Info
    case 'i':
    {
      Serial.println("Version: " SW_VERSION);
      Serial.print("Last error: ");Serial.println(lastError);
      Serial.println();
#ifdef ENABLE_LOGGING
      printLog();
#endif
    }
    break;
    
    // Just a newline
    case 0:
    break;
      
    // Unknown command
    default:
      Serial.print("Error: Unknown command: ");
      Serial.println(inputStr);
      //Serial.clear();
    break; 
  }   
}
