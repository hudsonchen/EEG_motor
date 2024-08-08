#include <Servo.h>
#include <FlexiTimer2.h>

// All definitions
#define SAMPFREQ 256 // ADC sampling rate 256 Hz
#define TIMER2VAL (1000 / SAMPFREQ) // Set 256Hz sampling frequency

// Global constants and variables
// These are volatile as they are handled in the ISR
volatile unsigned int ADC_value = 0; // ADC current value

Servo myServo; // Create a Servo object
const int servoPin = 9; // Define the pin to which the servo is connected

void setup() {
  Serial.begin(57600); // Begin serial communications at baud rate of 57600
  delay(100);

  myServo.attach(servoPin); // Attach the servo to the specified pin
  myServo.write(70); // Set the servo to the default position (70 degrees)

  noInterrupts(); // Disable all interrupts before initialization is complete

  // Timer2 is used to setup the analog channels sampling frequency and packet update.
  // Whenever interrupt occurs, the current read packet is sent to the PC
  FlexiTimer2::set(TIMER2VAL, Timer2_Overflow_ISR);
  FlexiTimer2::start();

  interrupts(); // Enable all interrupts after initialization has been completed
}

// Interrupt service routine (ISR) for reading ADC and transmitting packet from buffer
void Timer2_Overflow_ISR() {
  // Read ADC channel 0
  ADC_value = analogRead(A0);

  // Print ADC value over serial
  Serial.println(ADC_value);
}

int angle;

void loop() {
  if (Serial.available() >= 2) {
    // Read the incoming bytes
    byte highByte = Serial.read();
    byte lowByte = Serial.read();
    
    // Combine the two bytes into a single 16-bit integer
    int position = word(highByte, lowByte);
    
    // Write the position to the servo
    myServo.write(position);
  }
  // angle += 30;           // Increase the angle by 15 degrees
  // if (angle > 180) {     // If the angle exceeds 180 degrees, reset to 0
  //   angle = 0;
  // }
  // myServo.write(angle);  // Set the servo to the current angle
  // delay(1000);     
}
