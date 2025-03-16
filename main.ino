#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BUTTON_SELECT 2
#define BUTTON_MODE 12
#define BUTTON_RED 3
#define BUTTON_GREEN 4
#define BUTTON_BLUE 5
#define BUTTON_YELLOW 6

#define LED_RED 7
#define LED_GREEN 8
#define LED_BLUE 9
#define LED_YELLOW 10

LiquidCrystal_I2C lcd(0x27, 16, 2);
int mode = 1;
int sequence[100], userInput[100];
int seqLength = 3;
unsigned long lastButtonPress = 0;

void setup() {
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();

    for (int i = 2; i <= 10; i++) pinMode(i, INPUT_PULLUP);
    for (int i = 7; i <= 10; i++) pinMode(i, OUTPUT);

    lcd.setCursor(0, 0);
    lcd.print("Modo: ");
    updateMode();
}

void loop() {
    if (millis() - lastButtonPress > 300) {
        if (digitalRead(BUTTON_MODE) == LOW) {
            lastButtonPress = millis();
            mode = (mode == 3) ? 1 : mode + 1;
            updateMode();
        }
        if (digitalRead(BUTTON_SELECT) == LOW) {
            lastButtonPress = millis();
            seqLength = 3;
            startGame();
        }
    }
}

void updateMode() {
    lcd.setCursor(6, 0);
    lcd.print("                ");
    lcd.setCursor(6, 0);
    
    if (mode == 1) {
        lcd.print("Normal");
    } else if (mode == 2) {
        lcd.print("Inverso");
    } else {
        lcd.print("Mem.Extrema");
    }
}

void startGame() {
    while (true) {
        generateSequence();
        showSequence();
        delay(2000);

        bool correct = (mode == 2) ? getPlayerInputReverse() : getPlayerInput();

        if (!correct) {
            gameOver();
            break;
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Acertou!");
        delay(1000);

        if (mode == 3) {
            seqLength += 2;
        } else {
            seqLength++;
        }

        if (seqLength > 100) seqLength = 100;

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Proxima rodada!");
        delay(1500);
    }
}

void generateSequence() {
    for (int i = 0; i < seqLength; i++) {
        sequence[i] = random(1, 5);
    }
}

void showSequence() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Memorize...");

    for (int i = 0; i < seqLength; i++) {
        lightUpLED(sequence[i]);
        delay(600);
        turnOffLEDs();
        delay(300);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sua vez...");
}

void lightUpLED(int num) {
    int leds[] = {LED_RED, LED_GREEN, LED_BLUE, LED_YELLOW};
    
    digitalWrite(leds[num - 1], HIGH);
    delay(500);
    digitalWrite(leds[num - 1], LOW);
}

void turnOffLEDs() {
    for (int i = 7; i <= 10; i++) digitalWrite(i, LOW);
}

bool getPlayerInput() {
    for (int i = 0; i < seqLength; i++) {
        int buttonPressed = waitForButtonPress();
        if (buttonPressed == -1 || buttonPressed != sequence[i]) return false;
    }
    return true;
}

bool getPlayerInputReverse() {
    for (int i = seqLength - 1; i >= 0; i--) {
        int buttonPressed = waitForButtonPress();
        if (buttonPressed == -1 || buttonPressed != sequence[i]) return false;
    }
    return true;
}

int waitForButtonPress() {
    unsigned long startTime = millis();
    while (millis() - startTime < 10000) {
        for (int i = 0; i < 4; i++) {
            int buttons[] = {BUTTON_RED, BUTTON_GREEN, BUTTON_BLUE, BUTTON_YELLOW};
            if (digitalRead(buttons[i]) == LOW) {
                delay(50);
                if (digitalRead(buttons[i]) == LOW) {
                    lightUpLED(i + 1);
                    delay(100);
                    return i + 1;
                }
            }
        }
        delay(10);
    }
    return -1;
}

void gameOver() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Errou! Game Over");

    for (int i = 0; i < 3; i++) {
        for (int j = 7; j <= 10; j++) digitalWrite(j, HIGH);
        delay(300);
        turnOffLEDs();
        delay(300);
    }

    delay(1500);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Modo: ");
    updateMode();
}
