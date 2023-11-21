#include <EEPROM.h>

const int MIN_INTERVAL = 1;
const int MAX_INTERVAL = 10;
const int EEPROM_INTERVAL_ADDRESS = 0;
const int EEPROM_ULTRASONIC_ADDRESS = 10;
const int EEPROM_LRD_ADDRESS = 20;
int intervalUltrasonic = 0;

const int trigPin = 9;
const int echoPin = 10;
const int photocellPin = A0;

long duration = 0;
long distance = 0;
int photocellValue = 0;

int minLRD = 0;
int maxLRD = -1;

int minUltrasonic = 0;
int maxUltrasonic = -1;

unsigned long ultrasonicMillis = 0;
int ultrasonicEEPROMAddress = 0;
int lrdEEPROMAddress = 20;

int currentMenu = 0;
bool currentWriteContinue = true;

void displayMainMenu();
void displaySensorSettingsMenu();
void handleSerialInput();

void setup() {
  Serial.begin(9600);
  displayMainMenu();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  if (Serial.available() > 0) {
    handleSerialInput();
  }
  printSensorsReadings();
}

void displayMainMenu() {
  Serial.println(F("\nMain Menu:"));
  Serial.println(F("1. Sensor Settings"));
  Serial.println(F("2. Reset Logger Data"));
  Serial.println(F("3. System Status"));
  Serial.println(F("4. RGB LED Control"));
  Serial.println(F("5. Exit"));
  Serial.println(F("Enter the number of your choice:"));
}

void displaySensorSettingsMenu() {
  Serial.println(F("\nSensor Settings Menu:"));
  Serial.println(F("1. Sensors Sampling Interval"));
  Serial.println(F("2. Ultrasonic Alert Threshold"));
  Serial.println(F("3. LDR Alert Threshold"));
  Serial.println(F("4. Back to Main Menu"));
  Serial.println(F("Enter the number of your choice:"));
}

void displayResetMenu() {
  Serial.println(F("\n Reset Menu1:"));
  Serial.println(F("1. Yes"));
  Serial.println(F("2. No"));
}

void displaySystemStatusMenu() {
  Serial.println(F("\n SystemStatus Menu:"));
  Serial.println(F("1. Current sensor Readings"));
  Serial.println(F("2. Current Sensor Settings"));
  Serial.println(F("3. Display Logged Data"));
  Serial.println(F("4. Back to Main Menu"));
}

void displayRgbLedMenu(){
  Serial.println(F("\n RGB LED Control Menu:"));
  Serial.println(F("1. Manual Color Control"));
  Serial.println(F("2. Toggle Automatic ON/OFF"));
}


void intervalSettings() {
  Serial.println(F("Enter a value between 1 and 10 seconds for the sampling rate: "));
  delay(1000);

  int intervalLRD;
  int intervalUltrasonic;

  int userValue = Serial.parseInt();

  if (userValue >= MIN_INTERVAL && userValue <= MAX_INTERVAL) {
    intervalLRD = userValue;
    intervalUltrasonic = userValue;

    EEPROM.update(EEPROM_INTERVAL_ADDRESS, intervalLRD);
    EEPROM.update(EEPROM_INTERVAL_ADDRESS + sizeof(intervalLRD), intervalUltrasonic);

    Serial.println(F("Interval LRD set to: "));
    Serial.println(intervalLRD);
    Serial.println(F("Interval Ultrasonic set to: "));
    Serial.println(intervalUltrasonic);
  } else {
    Serial.println(F("Invalid input. Please enter a value between 1 and 10 seconds."));
  }
}

void setUltrasonicThreshold() {
  int ultrasonicThreshold;

  Serial.println("Enter the Ultrasonic Sensor Threshold value: ");
  delay(3000);

  if (Serial.available() > 0) {
    ultrasonicThreshold = Serial.parseInt();
    EEPROM.put(EEPROM_ULTRASONIC_ADDRESS, ultrasonicThreshold);
    Serial.println("Ultrasonic Threshold set to: " + String(ultrasonicThreshold));
  }

  int userChoice = 0;

  Serial.println("Do you want to set this value as:");
  Serial.println("1. Minimum Threshold");
  Serial.println("2. Maximum Threshold");

  

  userChoice = Serial.parseInt();
  if (Serial.available() > 0) {

    delay(3000);
    if (userChoice == 1) {
      minUltrasonic = ultrasonicThreshold;
      Serial.println("Minimum Ultrasonic Threshold set to: " + String(minUltrasonic));
    } else if (userChoice == 2) {
      maxUltrasonic = ultrasonicThreshold;
      Serial.println("Maximum Ultrasonic Threshold set to: " + String(maxUltrasonic));
    } else {
      Serial.println("Invalid choice. Threshold not set as minimum or maximum.");
    }
  }
  
}

void setLRDThreshold() {
  int LRDThreshold;


  Serial.println("Enter the Ultrasonic Sensor Threshold value: ");
  delay(3000);

  if (Serial.available() > 0) {
    LRDThreshold = Serial.parseInt();
    EEPROM.put(EEPROM_LRD_ADDRESS, LRDThreshold);
    Serial.println("Ultrasonic Threshold set to: " + String(LRDThreshold));
  }

  int userChoice = 0;

  Serial.println("Do you want to set this value as:");
  Serial.println("1. Minimum Threshold");
  Serial.println("2. Maximum Threshold");

  // Așteaptă până când utilizatorul introduce date prin portul serial

  userChoice = Serial.parseInt();
  if (Serial.available() > 0) {

    delay(3000);
    if (userChoice == 1) {
      minLRD = LRDThreshold;
      Serial.println("Minimum LRD Threshold set to: " + String(minLRD));
    } else if (userChoice == 2) {
      maxLRD = LRDThreshold;
      Serial.println("Maximum LRD set to: " + String(maxLRD));
    } else {
      Serial.println("Invalid choice. Threshold not set as minimum or maximum.");
    }
  }
}

void setResetYes(){
  if (Serial.available()){
    int userChoice = 0;

    Serial.println(F("Chose one of the options:"));
    Serial.println(F("1. Reset just the Ultrasonic Sensor"));
    Serial.println(F("2. Reset just the LRD Sensor"));
    Serial.println(F("3. Reset both sensors"));

    if (!userChoice) {

      delay(1000);
      if (Serial.available() > 0) {
        userChoice = Serial.parseInt();
      }
    }

    if (userChoice == 1) {
      // Reset Threshold
      if (minUltrasonic != 0){
        Serial.println(F("Ultrasonic Minimum Threshold reseted"));
        minUltrasonic = 0;
      }
      if (maxUltrasonic != 0){
        Serial.println(F("Ultrasonic Maximum Threshold reseted"));
        maxUltrasonic = 0;
      }
      
    } else if (userChoice == 2) {
      // Reset Threshold
      if (minLRD != 0){
        Serial.println(F("Lrd Minimum Threshold reseted"));
        minLRD = 0;
      }
      if (maxLRD != 0){
        Serial.println(F("Lrd Maximum Threshold reseted"));
        maxLRD = 0;
      }
    }
    else if(userChoice == 3){
      if (minUltrasonic != 0){
        minUltrasonic = 0;
        Serial.println(F("Ultrasonic Minimum Threshold reseted \n"));
        Serial.print(minUltrasonic);
      }
      if (maxUltrasonic != 0){
        maxUltrasonic = 0;
        Serial.println(F("Ultrasonic Maximum Threshold reseted \n"));
        Serial.print(maxUltrasonic);
      }
      if (minLRD != 0){
        minLRD = 0;
        Serial.println(F("Lrd Minimum Threshold reseted \n"));
        Serial.print(minLRD);
      }
      if (maxLRD != 0){
        maxLRD = 0;
        Serial.println(F("Lrd Maximum Threshold reseted \n"));
        Serial.print(maxLRD);
      }
    }
  }
}


void printSensorsReadings() {
  if (currentWriteContinue && ultrasonicMillis <= intervalUltrasonic * 5) {
    ultrasonicMillis++;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;
    photocellValue = analogRead(photocellPin);

    Serial.print(F("Distance for Ultrasonic: "));
    EEPROM.put(EEPROM_ULTRASONIC_ADDRESS, distance);

    Serial.println(distance);

    Serial.print(F("Photocell LDR reading: "));
    EEPROM.put(EEPROM_LRD_ADDRESS, photocellValue);

    Serial.println(photocellValue);
    delay(100);
  }

  if (Serial.available() > 0) {
    char inputChar = Serial.read();
    if (inputChar == 'q' || inputChar == 'Q') {
      Serial.println(F("Exiting loop. 'q' or 'Q' detected."));
      currentWriteContinue = false;
      ultrasonicMillis = 0;
    }
  }
}


void handleSerialInput() {
  String input = Serial.readStringUntil('\n');
  int choice = input.toInt();

  switch (currentMenu) {
    case 0: // Main Menu
      displayMainMenu();
      switch (choice) {
        case 1:
          currentMenu = 1;
          displaySensorSettingsMenu();
          break;
        case 2:
          currentMenu = 2;
          displayResetMenu();
          break;
        case 3:
          currentMenu = 3;
          displaySystemStatusMenu();
          break;
        case 4:
          currentMenu = 4;
          displayRgbLedMenu();
          break;
        case 5:
          Serial.println("Exiting program.");
          // Puteți adăuga aici orice acțiune de închidere a programului
          break;
        default:
          Serial.println("Invalid choice. Please enter a number between 1 and 5.");
          break;
      }
      break;

    case 1: // Sensor Settings Menu
      //displaySensorSettingsMenu();
      switch (choice) {
        case 1:
          Serial.println("Enter Sensors Sampling Interval (1-10): ");
          // Logică pentru citirea și setarea intervalului de eșantionare a senzorilor
          intervalSettings();
          break;
        case 2:
          Serial.println("Enter Ultrasonic Alert Threshold: ");
          // Logică pentru citirea și setarea pragului pentru senzorul ultrasonic
          setUltrasonicThreshold();
          break;
        case 3:
          Serial.println("Enter LDR Alert Threshold: ");
          // Logică pentru citirea și setarea pragului pentru senzorul LDR
          setLRDThreshold();
          break;
        case 4:
          currentMenu = 0; // Revenire la meniul principal
          displayMainMenu();
          break;
        default:
          Serial.println("Invalid choice. Please enter a number between 1 and 4.");
          break;
      }
      break;
    case 2: // Reset Menu
      switch (choice) {
        case 1:
          setResetYes();
          break;
        case 2:
          displayMainMenu();
          break;
      }

    /*default:
      currentMenu = 0;
      displayMainMenu();
      break;
    */
  }
}


