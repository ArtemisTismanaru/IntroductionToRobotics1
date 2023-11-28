//in video am scos buzzer ul pentru ca era zgomotos si deranja, i m sorry :)))))))
#include <LedControl.h>

// Pin configurations for components
const int buzzerPin = 8;
const int boomFrequency = 500;

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

void setup() {
  Serial.begin(9600);

  // Setup LED Matrix
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  // Setup pins for components
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(BOMB_LED_PIN, OUTPUT);

  // Seed for random number generation based on analog input
  randomSeed(analogRead(A2));

  // Generate initial walls
  generateRandomWalls();
}

void loop() {
  // Check if button is pressed and walls are not generated
  if (digitalRead(buttonPin) == LOW && !wallsGenerated) {
    // Generate new walls and reset game state
    generateRandomWalls();
    wallsGenerated = true;
    displayHiFlag = false;
    score = 0;
    bombPlaced = false;
    detonateFlag = false;
  }

  // Display a greeting message
  if (displayHiFlag) {
    displayHi();
  } else {
    // Read joystick values
    int xValue = analogRead(xPin);
    int yValue = analogRead(yPin);

    // Check joystick movement
    if (abs(xValue - 512) > JOYSTICK_DEADZONE || abs(yValue - 512) > JOYSTICK_DEADZONE) {
      // Move player if enough time has passed or bomb is placed
      if (millis() - lastMoveTime >= MOVE_INTERVAL || bombPlaced) {
        updatePlayerPosition(xValue, yValue);
        lastMoveTime = millis();
      }
    }

    // Check button press for bomb placement
    if (digitalRead(buttonPin) == LOW && wallsGenerated) {
      if (!bombPlaced && flagBomb) {
        placeBomb();
      }
    } else {
      // Detonate bomb if conditions are met
      if (bombPlaced && detonateFlag && millis() - bombLastPlacedTime > BOMB_INTERVAL && wallsGenerated) {
        detonateBomb();
        updateScore();
      }
    }

    // Display walls, player, and bomb status
    displayWalls();
    displayBlinkingPlayer();
    displayBlinkingBomb();

    // Check for reset condition (no walls left)
    if (noWallsLeft()) {
      wallsGenerated = false;  // Reset the state to generate new walls
      bombPlaced = false;
      detonateFlag = false;
    }
  }
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
  
  // Place player at a random position
  playerX = random(MATRIX_SIZE);
  playerY = random(MATRIX_SIZE);
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

// Function to display a greeting message
void displayHi() {
  const uint64_t HI_IMAGE = 0x0052525e52125200;

  unsigned long currentMillis = millis();
  if (millis() - currentMillis < interval) {
    displayImage(HI_IMAGE);
    lc.clearDisplay(0);
  }
}

// Function to display an image on the LED Matrix
void displayImage(uint64_t image) {
  for (int i = 0; i < 8; i++) {
    byte rowData = (image >> (i * 8)) & 0xFF;
    lc.setRow(0, i, rowData);
  }
}

// Function to update and display the score
void updateScore() {
  Serial.print("Score: ");
  Serial.println(score);
}
