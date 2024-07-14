#include <Arduino_FreeRTOS.h>

const int buzzerPins[5] = {7, 8, 9, 10, 11};
const int buzzerFrequencies[5] = {262, 294, 330, 349, 392};
const int buttonPins[5] = {2, 3, 4, 5, 6};
const int led1 = 12;
const int led2 = 13;

int randomBuzzer = 0;
int timercounter = 0;

// for RTOS
TaskHandle_t handler_preview;
TaskHandle_t handler_tone;
TaskHandle_t handler_timer;
TaskHandle_t handler_choose;

void setup() {
  // Initialize the buttons and LED as inputs/outputs using for loop
  for (int i = 0; i < 5; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // for RTOS
  xTaskCreate(Preview, "PreviewTask", 100, NULL, 1, &handler_preview);
  xTaskCreate(Tone, "ToneTask", 100, NULL, 1, &handler_tone);
  xTaskCreate(Timer, "TimerTask", 100, NULL, 1, &handler_timer);
  xTaskCreate(Choose, "ChooseTask", 100, NULL, 1, &handler_choose);

  // rng seed (using analog port of arduino)
  randomSeed(analogRead(0));
}

void loop() 
{  
}

// Step 1: Buzz each buzzer for do-re-mi-fa-so
static void Preview(void* pvParameters){
  while(1){
    vTaskSuspend(handler_timer);
    vTaskSuspend(handler_choose);
    vTaskSuspend(handler_tone);
    
    for (int i = 0; i < 5; i++) {
      tone(buzzerPins[i], buzzerFrequencies[i], 500);
      delay(500);
    }

    vTaskResume(handler_tone);
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

// Step 2: Wait for 2 seconds and then buzz a random buzzer
static void Tone(void* pvParameters){
  while(1){
    vTaskSuspend(handler_preview);

    delay(2000);
    randomBuzzer = random(5);
    tone(buzzerPins[randomBuzzer], buzzerFrequencies[randomBuzzer], 1000);

    vTaskResume(handler_choose);
    vTaskResume(handler_timer);
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

// Step 3a: Wait for user input and check if it's correct
static void Choose(void* pvParameters){
  while(1){
    vTaskSuspend(handler_tone);

    bool guessed = false;
    while (!guessed) {
      for (int i = 0; i < 5; i++) {
        if (digitalRead(buttonPins[i]) == LOW) {

          if (i == randomBuzzer) {
            timercounter = 0;
            vTaskSuspend(handler_timer);
            tone(buzzerPins[i], buzzerFrequencies[i], 1000);
            delay(1000);
            digitalWrite(led2, HIGH);
            delay(3000);
            digitalWrite(led2, LOW);
            guessed = true;
            vTaskResume(handler_preview);
            vTaskDelay(100/portTICK_PERIOD_MS);
          }
          else{
            tone(buzzerPins[i], buzzerFrequencies[i], 1000);
            delay(1000);
          }
        }
      }
    }
  }
}

// Step 3b: Timer for waiting for user input and checking
static void Timer(void* pvParameters){
  while(1){
    vTaskSuspend(handler_tone);
    for(timercounter=0;timercounter<5;timercounter++){
      digitalWrite(led1, HIGH);
      delay(200);
      digitalWrite(led1, LOW);
      delay(800);
      if(timercounter>=4){
        vTaskSuspend(handler_choose);
        digitalWrite(led1, HIGH);
        delay(3000);
        digitalWrite(led1, LOW);
        vTaskResume(handler_preview);
        vTaskDelay(100/portTICK_PERIOD_MS);
      }
    }
  }    
}