// Holds the last error. Used when info is printed.
char lastError[100] = "None";



#ifdef ENABLE_LOGGING
// For simple logging.
char logString[300] = {};
char* logPtr = logString;


// Logs a character into logString.
void logChar(char c) {
  *logPtr++ = c;
  if(logPtr >= logString+sizeof(logString))
    logPtr = logString;
}


// Prints the log string to Serial.
void printLog() {
 char* printPtr = logPtr;
  if(*printPtr == 0)
    printPtr = logString;
  char* endPrintPtr = logPtr-1;
  if(endPrintPtr < logString)
    endPrintPtr = logString + sizeof(logString);
    
  Serial.print("Log: ");
  while(char c = *printPtr++) {
    if(printPtr == endPrintPtr)
      break;
    if(printPtr >= logString + sizeof(logString))
      printPtr = logString;   
    Serial.print(c);
  }
  Serial.println();
}

#endif


// We should never get here. But if we do the assumption that the joystick + sampling delay is handled within 
// 1ms (i.e. 900us) is wrong.
// Or some other error occured.
// This is indicated by fast blinking of the LED and also all digital outs will blink.
// The routine will never exit.
void error(const char* text) {
  // Save error
  strcpy(lastError, text);
  
  // Print text to serial
  Serial.print(text);
  Serial.println();
  // blink
  fastBlinking();
}


// Blink fast. Main LED and DOUTs. For error indication.
void fastBlinking() {
  // Endless loop
  while(true) {
#ifdef MAIN_LED_PIN
    digitalWrite(MAIN_LED_PIN, false);
#endif
    for(int i=0; i<COUNT_DOUTS; i++) {
      analogWrite(doutPins[i], 255);
    }
    delay(150);
#ifdef MAIN_LED_PIN
    digitalWrite(MAIN_LED_PIN, true);
#endif
    for(int i=0; i<COUNT_DOUTS; i++) {
      analogWrite(doutPins[i], 0);
    }
    delay(50);
    
    // Handle serial in
    handleSerialIn();
  }
}


// Converts an ascii string into a number.
// Returns the value and also the string pointer to the first
// non-digit character.
uint16_t asciiToUint(const char** s) {
  const char* ptr = *s;
  uint16_t value = 0;
  uint8_t k=0;
  while(char digit = *ptr) {
    // Check digit
    if(digit < '0' || digit > '9') {
      break;
    }
    // check count of digits
    if(++k > 5) {
      Serial.print("Error: too many digits");
      Serial.println();
      value = 65535;
      break;
    }
    value = 10*value + digit-'0';
    ptr++;
  }
  *s = ptr;
  return value;
}
