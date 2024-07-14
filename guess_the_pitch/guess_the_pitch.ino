// Assign the pins for the buttons, buzzers, and LED
const int buzzerPins[5] = {7, 8, 9, 10, 11};
const int buzzerFrequencies[5] = {262, 294, 330, 349, 392};
const int buttonPins[5] = {2, 3, 4, 5, 6};
const int led1 = 12;
const int led2 = 13;

void setup() {
  // Initialize the buttons and LED as inputs/outputs
  for (int i = 0; i < 5; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  randomSeed(analogRead(0));
}

void loop() {
  // Step 1: Buzz each buzzer for do-re-mi-fa-so
  for (int i = 0; i < 5; i++) {
    tone(buzzerPins[i], buzzerFrequencies[i], 500);
    delay(500);
  }

  // Step 2: Wait for 2 seconds and then buzz a random buzzer
  delay(2000);
  int randomBuzzer = random(5);
  tone(buzzerPins[randomBuzzer], buzzerFrequencies[randomBuzzer], 1000);

  // Step 3: Wait for user input and check if it's correct
  bool guessed = false;
  while (!guessed) {
    for (int i = 0; i < 5; i++) {
      if (digitalRead(buttonPins[i]) == LOW) {
        tone(buzzerPins[i], buzzerFrequencies[i], 1000);
        delay(1000);

        if (i == randomBuzzer) {
          digitalWrite(led2, HIGH);
          delay(500);
          digitalWrite(led2, LOW);
          guessed = true;
          break;
        } else {
          digitalWrite(led1, HIGH);
          delay(500);
          digitalWrite(led1, LOW);
        }
      }
    }
  }

  // Step 4: Repeat at Step 1 if the guess is correct
  if (guessed) {
    delay(1000);
  }
}
