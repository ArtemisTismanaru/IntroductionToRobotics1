
#include <LedControl.h>
#include <LiquidCrystal.h>

// Pin configurations for LCD
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Pin configurations for components
const int buzzerPin = 8;
const int boomFrequency = 500;

// LED Matrix pin configuration
const int DIN_PIN = 12;
const int CLK_PIN = 11;
const int CS_PIN = 10;
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

const int BOMB_LED_PIN = 8;
const int JOYSTICK_DEADZONE = 50;
const int MOVE_INTERVAL = 300;
const int BOMB_INTERVAL = 3000;
unsigned long lastMoveTime = 0;
unsigned long bombLastPlacedTime = 0;

const int MATRIX_SIZE = 8;

// Joystick and button pins
const int xPin = A0;
const int yPin = A1;
const int buttonPin = 2;

// Initial player position
int playerX = MATRIX_SIZE / 2;
int playerY = MATRIX_SIZE / 2;

// Matrix to represent walls
bool walls[MATRIX_SIZE][MATRIX_SIZE];
bool wallsGenerated = false; // Flag to check if walls have been generated
bool displayHiFlag = true;   // Flag to display a greeting message
unsigned long previousMillis = 0;
const long interval = 200;

// Bomb-related variables
bool bombPlaced = false;
bool detonateFlag = false;
int bombX, bombY;

bool flagBomb = false;

unsigned long bombPreviousMillis = 0;
const long bombInterval = 100;
int score = 0;

bool displayWelcome = true;

unsigned long timerGame;

bool displayFlagMenu = false;
int menuState = 0; // 0 - Main Menu, 1 - Settings Menu, 2 - About Menu
char selectedOption;

int currentMessageIndex = 0;

bool hiDisplayed = false;
bool buttonPressed = false;
bool gameStarted = false;
bool hiFlag = false;

unsigned long gameStartTime;

const char messages[][20] = {
  "BOMBERMAN"
};

bool displayAboutMenu = false;
const char* githubUsername = "tinutzaa";
const char* fullName = "Tina";

bool displaySettings = false;

int lcdBrightness = 128; // Initial LCD brightness value
int matrixBrightness = 8; // Initial matrix brightness value
int subMenuState = 0;

void setup() {
  Serial.begin(9600);

  // LED Matrix setup
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  lcd.begin(16, 2);
  pinMode(3, OUTPUT); // Contrast pin for LCD
  analogWrite(3, 128);

  // Component pins setup
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(BOMB_LED_PIN, OUTPUT);

  // Random seed based on analog input
  randomSeed(analogRead(A2));

  // Generate initial walls
  generateRandomWalls();

  // Record game start time
  gameStartTime = millis();
  analogWrite(3, 128);
}


void loop() {
  if (displayWelcome) {
    // Display welcome messages and initiate menu
    displayHi();
    readJoystickMenu();
    displayScrollingMessage();
    displayFlagMenu = true;
    displayWelcome = false;
    hiDisplayed = false;
  } else if (displayFlagMenu) {
    // Display menu and read joystick input
    displayMenu();
    readJoystickMenu();
  } else {
    // Check if the joystick button is pressed to start the game
    if (digitalRead(buttonPin) == LOW && !wallsGenerated && !buttonPressed) {
      // Initialize game logic and countdown
      displayCountdown(3);
      generateRandomWalls();
      wallsGenerated = true;
      displayHiFlag = false;
      score = 0;
      bombPlaced = false;
      detonateFlag = false;
      gameStartTime = millis();
      buttonPressed = true;
    }

    if (!gameStarted) {
      // Display countdown and update game start flag
      for (int count = 3; count >= 0; count--) {
        displayCountdown(count);
        delay(1000);
      }
      gameStarted = true;
    }

    // Update and display game details
    displayGameDetails();

    // Check for joystick movement to move the player
    int xValue = analogRead(xPin);
    int yValue = analogRead(yPin);

    if (abs(xValue - 512) > JOYSTICK_DEADZONE || abs(yValue - 512) > JOYSTICK_DEADZONE) {
      // Move the player if enough time has passed or a bomb is placed
      if (millis() - lastMoveTime >= MOVE_INTERVAL || bombPlaced) {
        updatePlayerPosition(xValue, yValue);
        lastMoveTime = millis();
      }
    }

    // Check if the joystick button is pressed to place a bomb
    if (digitalRead(buttonPin) == LOW && wallsGenerated) {
      if (!bombPlaced && flagBomb) {
        placeBomb();
      }
    } else {
      // Check if it's time to detonate the bomb
      if (bombPlaced && detonateFlag && millis() - bombLastPlacedTime > BOMB_INTERVAL && wallsGenerated) {
        detonateBomb();
        updateScore();
        handleButtonPress();
      }
    }

    // Display walls, player, and bomb state
    displayWalls();
    displayBlinkingPlayer();
    displayBlinkingBomb();

    // Check for reset condition (no more walls)
    if (noWallsLeft()) {
      wallsGenerated = false;
      bombPlaced = false;
      detonateFlag = false;
    }
  }
}

void adjustLcdBrightness(int value) {
  lcdBrightness = constrain(value, 0, 255);
  analogWrite(3, lcdBrightness);
}

void adjustMatrixBrightness(int value) {
  matrixBrightness = constrain(value, 0, 15);
  lc.setIntensity(0, matrixBrightness);
}

void displayMessage() {
  // Clear the LCD screen and set the cursor to the top-left position
  lcd.clear();
  lcd.setCursor(0, 0);

  // Print the current message to the LCD
  lcd.print(messages[currentMessageIndex]);

  // Enable autoscroll to display long messages on a single line
  lcd.autoscroll();

  // Stop autoscroll after a delay to allow reading the message
  delay(1000);
  lcd.noAutoscroll();

  // Stop displaying messages after the last message
  if (currentMessageIndex < sizeof(messages) / sizeof(messages[0]) - 1) {
    // Move to the next message index if there are more messages
    currentMessageIndex++;
  } else {
    // Set flags to control the flow (welcome done, flag menu displayed)
    displayWelcome = false;
    displayFlagMenu = true;
    displayHiFlag = false;
  }
}

void displayScrollingMessage() {

  const char *message = "Welcome to the Jungle          ";
  int lcdWidth = 16;  // Number of characters displayed per line on the LCD
  int messageLength = strlen(message);

  // Iterate through the message and scroll it on the LCD
  for (int i = 0; i < messageLength + lcdWidth; ++i) {
    lcd.clear();
    lcd.setCursor(0, 0);

    // Display a portion of the message based on the current iteration
    for (int j = 0; j < lcdWidth; ++j) {
      int index = (i + j) % messageLength;
      lcd.write(message[index]);
    }

    delay(200);  // Wait for 200 milliseconds between each movement
  }

  delay(1000);  // Wait for 1 second after scrolling
  displayMessage();  // Move to the next step: displaying messages
}




void displaySettingsMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SETTINGS");
  switch (subMenuState) {
    case 0:
      lcd.setCursor(0, 1);
      lcd.print("a) LCD Brightness");
      break;
    case 1:
      lcd.setCursor(0, 1);
      lcd.print("b) Matrix Brightness");
      break;
  }
}


void displayMenu() {
  //lcd.clear();
  switch (menuState) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("MAIN MENU");
      lcd.setCursor(0, 1);
      lcd.print("a) Start game");
      break;
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MAIN MENU");
      lcd.setCursor(0, 1);
      lcd.print("b) Settings");
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MAIN MENU");
      lcd.setCursor(0, 1);
      lcd.print("c) About");
      if (displayAboutMenu) {
        // Submenu "About"
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("GitHub: ");
        lcd.print(githubUsername);
        lcd.setCursor(0, 1);
        lcd.print("Name: ");
        lcd.print(fullName);
      }
      break;
  }
}

/*void adjustLcdBrightness(int value) {
  lcdBrightness = constrain(value, 0, 255);
  analogWrite(3, lcdBrightness);
}

void adjustMatrixBrightness(int value) {
  matrixBrightness = constrain(value, 0, 15);
  lc.setIntensity(0, matrixBrightness);
}
*/
void readJoystickMenu() {
  int joystickX = analogRead(xPin);  // Read analog value from the joystick's X axis
  int joystickY = analogRead(yPin);  // Analog read for Y axis

  // Check joystick movement on the Y axis
  if (joystickY < 100) {
    // Move up in the menu
    if (displayAboutMenu) {
      displayAboutMenu = false;
    } else {
      menuState = (menuState - 1 + 3) % 3;
      displayMenu();
      delay(300);
    }
  } else if (joystickY > 900) {
    // Move down in the menu
    if (displayAboutMenu) {
      displayAboutMenu = false;
    } else {
      menuState = (menuState + 1) % 3;
      displayMenu();
      delay(300);
    }
  }

  // Check joystick button press
  if (digitalRead(buttonPin) == LOW) {
    // Handle button press based on the current menu state
    if (menuState == 0 && !displayAboutMenu) {
      displayFlagMenu = false;
      wallsGenerated = false;
      gameStartTime = millis();
    } else if (menuState == 1 && !displayAboutMenu) {
      if (subMenuState == 0 || subMenuState == 1) {
        // "LCD Brightness" or "Matrix Brightness" submenu is selected
        displaySettings = true;
        displaySettingsMenu();
      }
    } else if (menuState == 2 && !displayAboutMenu) {
      displayAboutMenu = true;
    } else if (displayAboutMenu) {
      displayAboutMenu = false;
    }
  }

  // Check if the settings menu is being displayed
  if (displaySettingsMenu) {
    int joystickX = analogRead(xPin);  // Read analog value from the joystick's X axis

    // Adjust settings based on joystick movement on the X axis
    if (joystickX < 100) {
      if (subMenuState == 0) {
        // "LCD Brightness" submenu is selected
        adjustLcdBrightness(lcdBrightness + 10);
      } else if (subMenuState == 1) {
        // "Matrix Brightness" submenu is selected
        adjustMatrixBrightness(matrixBrightness + 1);
      }
      delay(300);
    } else if (joystickX > 900) {
      if (subMenuState == 0) {
        // "LCD Brightness" submenu is selected
        adjustLcdBrightness(lcdBrightness - 10);
      } else if (subMenuState == 1) {
        // "Matrix Brightness" submenu is selected
        adjustMatrixBrightness(matrixBrightness - 1);
      }
      delay(300);
    }
  }
}


void displayCountdown(int count) {
  hiFlag = true;
  const uint64_t countdownImage[] = {
    0x0000000000000000,
    //
    0x0018181c1818187e, // 1
    0x003c6660300c067e, // 2
    0x003c66603860663c, // 3
  };

  if (count >= 0 && count <= 3) {
    displayImage(countdownImage[count]);
    delay(500);
    lc.clearDisplay(0);
  }
}


void handleButtonPress() {
  // Show to the display the countdown images and make decisions 
  if (digitalRead(buttonPin) == LOW && !wallsGenerated ) {
    for (int count = 3; count >= 0; count--) {
      displayCountdown(count);
    }
    wallsGenerated = true;  
  }
}

void displayGameDetails() {
  unsigned long elapsedTime = millis() - gameStartTime;
  int seconds = elapsedTime / 1000;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(seconds);
  lcd.print("s");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(score);
}



// Function to check if the player is surrounded by walls
bool isPlayerSurrounded(int x, int y) {
  if ((x > 0 && walls[y][x - 1]) || (x < MATRIX_SIZE - 1 && walls[y][x + 1]) ||
      (y > 0 && walls[y - 1][x]) || (y < MATRIX_SIZE - 1 && walls[y + 1][x])) {
    return true;
  }
  return false;
}

// Function to play a sound when a bomb detonates
void playBoomSound() {
  tone(buzzerPin, boomFrequency, 1000);
  delay(500);
  noTone(buzzerPin);
}

// Function to generate random walls
void generateRandomWalls() {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      walls[i][j] = random(2) == 0;
    }
  }
  
  do{
  playerX = random(MATRIX_SIZE);
  playerY = random(MATRIX_SIZE);
  }while(walls[playerY][playerX] || isPlayerSurrounded(playerX, playerY));
  gameStartTime = millis();
}

// Function to display walls on the LED Matrix
void displayWalls() {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      if (walls[i][j]) {
        lc.setLed(0, i, j, true);
      }
    }
  }
}

// Function to display blinking player on the LED Matrix
void displayBlinkingPlayer() {
  unsigned long currentMillis = millis();
  static bool playerLEDState = false;

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    playerLEDState = !playerLEDState;

    lc.setLed(0, playerY, playerX, playerLEDState);
  }
}

// Function to display blinking bomb on the LED Matrix
void displayBlinkingBomb() {
  unsigned long currentMillis = millis();
  static bool bombLEDState = false;

  if (bombPlaced && detonateFlag) {
    if (currentMillis - bombPreviousMillis >= bombInterval) {
      bombPreviousMillis = currentMillis;
      bombLEDState = !bombLEDState;

      lc.setLed(0, bombY, bombX, bombLEDState);
    }
  }
}

// Function to update player position based on joystick input
void updatePlayerPosition(int xValue, int yValue) {
  int deltaX = xValue - 512;
  int deltaY = yValue - 512;

  int previousX = playerX;
  int previousY = playerY;

  lc.setLed(0, previousY, previousX, false);

  unsigned long currentMillis = millis();

  if (currentMillis - lastMoveTime >= MOVE_INTERVAL) {
    if (abs(deltaX) > JOYSTICK_DEADZONE) {
      if (deltaX > 0 && playerX < MATRIX_SIZE - 1 && !walls[playerY][playerX + 1]) {
        playerX++;
      } else if (deltaX < 0 && playerX > 0 && !walls[playerY][playerX - 1]) {
        playerX--;
      }
    }

    if (abs(deltaY) > JOYSTICK_DEADZONE) {
      if (deltaY > 0 && playerY < MATRIX_SIZE - 1 && !walls[playerY + 1][playerX]) {
        playerY++;
      } else if (deltaY < 0 && playerY > 0 && !walls[playerY - 1][playerX]) {
        playerY--;
      }

      if (playerX != previousX || playerY != previousY) {
        lc.setLed(0, playerY, playerX, true);
        flagBomb = true;
      }

      lastMoveTime = currentMillis;
    }
  }
}

// Function to check if there are no walls left
bool noWallsLeft() {
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      if (walls[i][j]) {
        return false;
      }
    }
  }
  return true;
}

// Function to place a bomb
void placeBomb() {
  if (!bombPlaced) {
    bombX = playerX;
    bombY = playerY;
    bombPlaced = true;
    detonateFlag = true;
    digitalWrite(BOMB_LED_PIN, HIGH);
    bombLastPlacedTime = millis();
  }
}

// Function to detonate a bomb and eliminate nearby walls
void detonateBomb() {
  if (bombPlaced) {
    eliminateFirstWallInDirection(bombX, bombY, 1, 0);
    eliminateFirstWallInDirection(bombX, bombY, -1, 0);
    eliminateFirstWallInDirection(bombX, bombY, 0, 1);
    eliminateFirstWallInDirection(bombX, bombY, 0, -1);

    lc.setLed(0, bombY, bombX, false);
    digitalWrite(BOMB_LED_PIN, LOW);
    bombPlaced = false;
    detonateFlag = false;
    playBoomSound();

    score += (millis() - bombLastPlacedTime) / 1000;
  }
}

// Function to eliminate the first wall in a given direction
void eliminateFirstWallInDirection(int startX, int startY, int dirX, int dirY) {
  for (int x = startX + dirX, y = startY + dirY;
       x >= 0 && x < MATRIX_SIZE && y >= 0 && y < MATRIX_SIZE;
       x += dirX, y += dirY) {
    if (walls[y][x]) {
      walls[y][x] = false;
      lc.setLed(0, y, x, false);
      break;
    }
  }
}

void displayImage(uint64_t image) {
  for (int i = 0; i < 8; i++) {
    byte rowData = (image >> (i * 8)) & 0xFF;
    lc.setRow(0,7- i, rowData);
  }
}

// Function to display a greeting message
void displayHi() {
  const uint64_t HI_IMAGE = 0x005212525e525200;

  unsigned long currentMillis = millis();
  if (millis() - currentMillis < interval && !hiDisplayed) {
    displayImage(HI_IMAGE);
    if(hiFlag)
      lc.clearDisplay(0);
    hiDisplayed = true;
  }
}



// Function to update and display the score
void updateScore() {
  Serial.print("Score: ");
  Serial.println(score);
}

