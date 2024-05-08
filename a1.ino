#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 3

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create a new MFRC522 instance
Servo myServo;                      // Create a new Servo object

void setup() {
  Serial.begin(9600); // Init serial communication
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 reader
  myServo.attach(SERVO_PIN); // Attach the servo to a pin
  myServo.write(0);          // Set initial servo position if needed
}

void loop() {
  // Check for new RFID cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    content.toUpperCase();
    Serial.println(content); // Send the UID to serial
    
    // We assume 'rotate_servo' as command to rotate the servo
    // Let's simulate a UID match just for demonstration purposes
    if (content.equals("UID MATCH FROM CSV")) { // You should replace this with actual logic
      myServo.write(90); // If matched, rotate the servo
      delay(1000);       // Wait for a while
      myServo.write(0);  // Rotate back to initial position
    }
    mfrc522.PICC_HaltA(); // Stop reading
  }

  // If received data from serial (from connected computer)
  if(Serial.available() > 0){
    String cmd = Serial.readString();
    if(cmd == "rotate_servo"){
      myServo.write(90); // Rotate the servo if command received
      delay(1000);
      myServo.write(0);
    }
  }
}