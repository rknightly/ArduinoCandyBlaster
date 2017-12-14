struct channel {
  int inputPin;
  int pwmOutPin;
  int dirOutPin;

  int currentPulseWidth;
  
  int analogOut;  /* analogOut represents the magnitude of the motor speed (0-255)*/
  int digitalOut; /* digitalOut represents the sign of the motor speed */
                  /* 0 for negative rotation and 1 for positive rotation */
  
  String channelName;

  channel(int input, int pwm, int dir, String channelName):
    inputPin(input),
    pwmOutPin(pwm),
    dirOutPin(dir),
    currentPulseWidth(0),
    analogOut(0),
    digitalOut(0),
    channelName(channelName) {
    }
};
typedef struct channel Channel;

// Create channels
Channel leftWheel     = channel(1, 5, 9, "Left Wheel");
Channel rightWheel    = channel(2, 6, 10, "Right Wheel");
Channel highFive      = channel(3, 7, 11, "High Five");
Channel candyShooter  = channel(4, 8, 12, "Candy Shooter");

#define CHANNEL_COUNT 4
// Make a list of pointers to each of the channels
channel *channels[CHANNEL_COUNT] = {
  &leftWheel, &rightWheel, &highFive, &candyShooter
};

// Initialize the pin modes and initial outputs
void setup() {
  // Loop through each channel
  for (int i = 0; i<CHANNEL_COUNT; i++) {
    // Set pin modes
    pinMode(channels[i]->inputPin, INPUT);
    pinMode(channels[i]->pwmOutPin, OUTPUT); 
    pinMode(channels[i]->dirOutPin, OUTPUT);
    // Initialize outputs
    analogWrite(channels[i]->pwmOutPin, 0);
    digitalWrite(channels[i]->dirOutPin, 0);
  }
  
  Serial.begin(9600); // Initialize Serial (for debugging)
}

// Update loop, called repeatedly and rapidly while the bot is running
void loop() {
  // Loop through each channel
  for (int i=0; i<CHANNEL_COUNT; i++) {
    // Read input
    int pin = channels[i]->inputPin;
    // Read the current pulse width of each of the channels
    int currentPulseWidth = pulseIn(pin, HIGH, 25000); // Should be between 1000 & 2000
    channels[i]->currentPulseWidth = currentPulseWidth;
    int mappedPulseWidth = map(currentPulseWidth, 1000, 2000, -500, 500); // center over zero
    mappedPulseWidth = constrain(mappedPulseWidth, -255, 255); // cut off the values to range from -255 to 255

    if (mappedPulseWidth >= 0) {
      channels[i]->digitalOut = 1; 
    } 
    else {
      channels[i]->digitalOut = 0;
    }
    channels[i]->analogOut = abs(mappedPulseWidth);

    // Set motor speed
    digitalWrite(channels[i]->dirOutPin, channels[i]->digitalOut);
    analogWrite(channels[i]->pwmOutPin, channels[i]->analogOut); 
  }
}

// Function to print out connection statuses and values in the channels for debugging
void testConnections() {
  // Print the data being received through each channel
  for (int i=0; i<CHANNEL_COUNT; i++) {
    if (channels[i]->currentPulseWidth > 1000) {
      Serial.print(channels[i]->channelName + " Channel: ");
      Serial.println(channels[i]->currentPulseWidth);
    } else {
      Serial.println(channels[i]->channelName + " Channel: Disengaged");
    }
  }
  
  Serial.println();
  
  delay(100);
}
