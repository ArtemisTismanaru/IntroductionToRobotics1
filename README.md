# IntroductionToRobotics1
Laboratory sssignments for the Robotics Introduction Course, completed during the third year of study at the Faculty of Mathematics and Computer Science, University of Bucharest. Each assignment has got included the instructions we had to follow, implementation guidelines, code samples, and also is accompanied by image files.

# Homeworks

<details>
  <summary> <h2>Homework 1: RGB led control with 3 potentiometers </h2> </summary>


### 💻 Code: [See my code:] (https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/Homework/Code1)

### 🎥 Cum functioneaza (https://youtube.com/shorts/V9qGQQUacq0?si=pEYdPxho2bK9czGy)

### 🔌 Schema electronica (https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/Homework/WhatsApp%20Image%202023-10-25%20at%2000.46.37.jpeg)

### 🖼️ Poza Setup (https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/Homework/WhatsApp%20Image%202023-10-25%20at%2000.45.57.jpeg)

</details>

<details>
  <summary> <h2>Homework 2: Elevator Simulator </h2> </summary>

### 💻 Code: (https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/Homework/code)

### 🎥 Cum functioneaza (https://youtube.com/shorts/H0xt9Ap6oJ8?si=-QcVK73RqbiOr8Gu)

### 🔌 Schema Electronica (https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/WhatsApp%20Image%202023-11-01%20at%2000.47.53.jpeg)

### 🖼️ Poza Setup (https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/WhatsApp%20Image%202023-11-01%20at%2001.11.24.jpeg)

</details>

<details>
  <summary> <h2>Homework 3: 7 segment display drawing 🎮 </h2> </summary>
  <br>

  ### Task: The joystick will be used to control the position of the segment on the display
  ### Components:
  - 7 segment display
  - joystick

  ### The system has the following states:
  1. State 1 (default, but also initiated after a button press in State 2): Current position blinking. Can use the joystick to move from one position to neighbors. 
  Short pressing the button toggles state 2. Long pressing the button in state 1 resets the entire display by turning all the segments OFF and moving the current     position to the decimal point.
  2. State 2 (initiated after a button press in State 1): The current segment stops blinking, adopting the state of the segment before selection (ON or OFF).         Toggling the X axis should change the segment state from ON to OFF or from OFF to ON. Clicking the joystick should save the segment state and exit back to state    1.

  ![image](https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/homework2/WhatsApp%20Image%202023-11-07%20at%2022.45.46.jpeg)
  
  
  ### 💻 Code: (https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/homework2/code1)

  ### 🎥 How it works: (https://youtube.com/shorts/vXZ6uFWi6N8?si=PrDexMHQWC0MWhRt)


  ![schema](https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/homework2/WhatsApp%20Image%202023-11-07%20at%2022.53.27.jpeg)

  </br>
</details>

<details>
  <summary> <h2>Homework 4: Stopwatch Timer ⏰ </h2> </summary>
  <br>

  ### Task: Using the 4 digit 7 segment display and 3 buttons,you should implement a stopwatch timer that counts in 10ths of a secondand has a save lap functionality
  ### Components:
  - 4 digit 7 segment display
  - 3 buttons
  - 1 LED
  - 1 shift register

  
  <img src="https://github.com/tinutzaa/IntroductionToRobotics1/raw/main/WhatsApp%20Image%202023-11-14%20at%2023.33.54.jpeg" alt="Imagine" width="400" height="500">
  
  
  ### 💻 Code: (https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/code_clock)

  ### 🎥 How it works: (https://youtube.com/shorts/kvDU60dixpo?si=LY0Mh3diRHIHFv1o)


  <img src="https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/WhatsApp%20Image%202023-11-14%20at%2023.33.48.jpeg" alt="Imagine" width="500" height="400">

  </br>
</details>


<details>
  <summary> <h2>Homework 5: Pseudo-Smart Environment Monitor and Logger </h2> </summary>

  <br>
  
  ### Task: This sys-tem will utilize various sensors to gather environmental data, log this data intoEEPROM, and provide both visual feedback via an RGB LED and user interac-tion through a Serial Menu.  The project focuses on integrating sensor readings,memory management, Serial Communication and the general objective of build-ing a menu.
  ### Components:
    •Arduino Uno Board
    •Ultrasonic Sensor (HC-SR04)
    •LDR (Light-Dependent Resistor) aka Photocell aka Photoresistor aka LightSensor
    •RGB LED•Resistors as needed
    •Breadboard and connecting wires

  ### 💻 Code: (https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/Homework/homework5_copy_20231121214403.ino)

  ### 🎥 How it works: (https://youtube.com/shorts/qBRE4HEZUOc?si=ysTbGnolLoKnQNT_)

  <img src="https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/Homework/IMG_9626%202.JPG" alt="Imagine" width="500" height="400">

 </br>
</details>


<details>
  <summary> <h2>Homework 6: Mini 8x8 LED Matrix game 🎮 </h2> </summary>

  <br>
  
  ### Task: Develop a small game on the 8x8 matrix.  The game must have at least 3 typesof elements:  player (blinks slowly),  bombs/bullets (blinks fast),  wall (doesn’tblink). The basic idea is that you generate walls on the map (50% - 75% of the map) and thenyou move around with the player and destroy them.
  ### Components:
    •Arduino Uno Board
    •Joystick
    •8x8 LED Matrix
    •MAX7219
    •Resistors and capacitors as needed
    •Breadboard and connecting wires
    •Buzzer

  ### 💻 Code: (https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/Homework/matrix1.ino)

  ### 🎥 How it works: (https://youtu.be/IQOrVHIVJLI?feature=shared)

  <img src="https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/Homework/image.png" width="400" height="500">

  <img src="https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/Homework/WhatsApp%20Image%202023-11-28%20at%2022.48.13.jpeg" width="500" height="300">

 </br>
</details>


 
 <details>
  <summary> <h2>Homework 7: Mini 8x8 LED Matrix game part. 2 🎮 </h2> </summary>

  <br>
  
  ### Task: Develop a game on the 8x8 matrix.  The game must have at least 3 typesof elements:  player (blinks slowly),  bombs/bullets (blinks fast),  wall (doesn’tblink). The basic idea is that you generate walls on the map (50% - 75% of the map) and then you move around with the player and destroy them. Also you should have a LCD display with an intro message and a menu.

  ### Components:
    •Arduino Uno Board
    •Joystick
    •8x8 LED Matrix
    •MAX7219
    •Resistors and capacitors as needed
    •Breadboard and connecting wires
    •Buzzer
    •LCD display

  ### Description: "BomberMan Jungle Adventure" is a fast-paced maze game where players strategically navigate through a jungle filled with walls. Using a joystick, they aim to place bombs strategically to clear obstacles and earn points. The game features dynamic menus, LED matrix displays, and a countdown element, adding excitement to the quest for the highest score.

  ### 💻 Code: (https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/Homework/homework_7.ino)

  ### 🎥 How it works: (https://youtu.be/--uN2YxSzXs?feature=shared)

  <img src="https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/Homework/WhatsApp%20Image%202023-12-13%20at%2001.22.01.jpeg" width="400" height="500">


 </br>
</details>


 <details>
  <summary> <h2>Homework 8: Sequence memory game </h2> </summary>

  <br>
  
  ### Description: This adorable code is crafted for an engaging game on the Arduino platform! 🕹️ The game features four colorful buttons (red, green, blue, and yellow) with sparkling LEDs and a cheerful buzzer for musical sounds! 🌈 The delightful goal of the game is to memorize and reproduce randomly generated sequences of colors and sounds. 💡 Start in "MODE_MEMORY" and enjoy the game cycles until you win or lose, all based on how accurately you reproduce the sequences. 🎮

  ### 💻 Code: (https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/primul.ino)

  ### 🎥 How it works: (https://youtu.be/YPsODlUnN0A?si=OlolpkEqH_hKSM79)

  <img src="https://github.com/tinutzaa/IntroductionToRobotics1/blob/main/WhatsApp%20Image%202024-01-27%20at%2000.35.21.jpeg" width="400" height="500">


 </br>
</details>

