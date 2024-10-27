#include <Wire.h>                // Include Wire library for I2C
#include <Servo.h>               // Include the Servo library
#include <LiquidCrystal_I2C.h>   // Include the LCD library

// Create servo objects
Servo horizontalServo;
Servo verticalServo;

// LDR pins (analog)
int ldrTop = A0;       // LDR connected to A0
int ldrBottom = A1;    // LDR connected to A1
int ldrLeft = A2;      // LDR connected to A2
int ldrRight = A3;     // LDR connected to A3

// Servo pins (digital)
int horizontalPin = 11;   // Horizontal servo connected to pin 11
int verticalPin = 10;     // Vertical servo connected to pin 10

// Voltage sensor pin for battery voltage measurement
#define VoltageSensor A1 // Connect the voltage sensor to A1

// Create LCD object (I2C address may vary, check your LCD module)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16x2 LCD

// Threshold for sensitivity
int threshold = 100;  // Increased threshold for less sensitivity

// Calibration factor for voltage measurement
float calibrationFactor = 1.0; // Set to 1.0 for direct measurements; adjust as needed

void setup() {
  // Attach servos to their respective pins
  horizontalServo.attach(horizontalPin);  // Attach the horizontal servo to pin 11
  verticalServo.attach(verticalPin);      // Attach the vertical servo to pin 10
  
  // Start servos at 90 degrees (neutral position)
  horizontalServo.write(90);
  verticalServo.write(90);
  
  // Start serial communication for debugging
  Serial.begin(9600);

  // Initialize LCD
  lcd.begin();
  lcd.backlight();
  
  // Display the static "Light Intensity" message
  lcd.setCursor(0, 0);
  lcd.print("Light Intensity");
}

void loop() {
  // Read LDR values
  int topValue = analogRead(ldrTop);
  int bottomValue = analogRead(ldrBottom);
  int leftValue = analogRead(ldrLeft);
  int rightValue = analogRead(ldrRight);

  // Calculate vertical and horizontal differences
  int verticalDiff = topValue - bottomValue;
  int horizontalDiff = leftValue - rightValue;

  // Get current positions of the servos
  int horizontalPos = horizontalServo.read();
  int verticalPos = verticalServo.read();

  // Adjust horizontal servo based on light difference
  if (abs(horizontalDiff) > threshold) {
    if (horizontalDiff > 0 && horizontalPos > 0) {
      horizontalServo.write(horizontalPos - 1);  // Move left
    } else if (horizontalDiff < 0 && horizontalPos < 180) {
      horizontalServo.write(horizontalPos + 1);  // Move right
    }
  }

  // Adjust vertical servo based on light difference
  if (abs(verticalDiff) > threshold) {
    if (verticalDiff > 0 && verticalPos > 0) {
      verticalServo.write(verticalPos - 1);  // Move down
    } else if (verticalDiff < 0 && verticalPos < 180) {
      verticalServo.write(verticalPos + 1);  // Move up
    }
  }

  // Read voltage from the sensor
  int value = analogRead(VoltageSensor);
  // Convert analog reading to voltage (assuming a 5V reference)
  float vOUT = (value * 5.0) / 1024.0;

  // Apply calibration factor if necessary
  vOUT *= calibrationFactor;

  // Display intensity on the LCD
  lcd.setCursor(0, 1);  // Move to the second row
  lcd.print(" ");
  lcd.print(vOUT, 2);   // Display voltage with 2 decimal places
  lcd.print(" lux");     // Display light intensity unit (lux)

  // Debugging information (optional)
  Serial.print("Top: "); Serial.print(topValue);
  Serial.print(" Bottom: "); Serial.print(bottomValue);
  Serial.print(" Left: "); Serial.print(leftValue);
  Serial.print(" Right: "); Serial.print(rightValue);
  Serial.print(" Intensity: "); Serial.println(vOUT); // Print intensity to Serial Monitor

  delay(10); // Increase delay for smoother movements and updates
}
