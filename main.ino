#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Inclui a biblioteca para controlar o LCD via I2C

// Definindo os pinos usados para os botões de controle
#define BUTTON_SELECT 2   // Botão para iniciar ou reiniciar o jogo
#define BUTTON_MODE 12    // Botão para alternar entre os modos
#define BUTTON_RED 3      // Botão para a cor vermelha
#define BUTTON_GREEN 4    // Botão para a cor verde
#define BUTTON_BLUE 5     // Botão para a cor azul
#define BUTTON_YELLOW 6   // Botão para a cor amarela

// Definindo os pinos para os LEDs das cores
#define LED_RED 7
#define LED_GREEN 8
#define LED_BLUE 9
#define LED_YELLOW 10

// Inicializando o LCD com o endereço I2C 0x27, tamanho 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// Definindo as variáveis principais do jogo
int mode = 1;  // Variável para controlar o modo (1: Normal, 2: Inverso, 3: Memória Extrema)
int sequence[100], userInput[100]; // Vetores para armazenar a sequência gerada e a entrada do jogador
int seqLength = 3;  // Começa com 3 cores na sequência
unsigned long lastButtonPress = 0; // Variável para controlar o tempo entre pressões de botões

void setup() {
    Serial.begin(9600);  // Inicializa a comunicação serial (opcional para debug)
    lcd.init();  // Inicializa o LCD
    lcd.backlight();  // Liga o backlight do LCD

    // Configura os pinos de botões como entradas com resistência de pull-up (evita interferência elétrica)
    for (int i = 2; i <= 10; i++) pinMode(i, INPUT_PULLUP);
    // Configura os pinos dos LEDs como saídas
    for (int i = 7; i <= 10; i++) pinMode(i, OUTPUT);

    // Exibe o modo de jogo inicial no LCD
    lcd.setCursor(0, 0);
    lcd.print("Modo: ");
    updateMode();  // Atualiza a exibição do modo atual
}

void loop() {
    // Verifica se um botão foi pressionado e controla o tempo entre as pressões
    if (millis() - lastButtonPress > 300) {
        // Alterna o modo de jogo quando o botão "Mode" é pressionado
        if (digitalRead(BUTTON_MODE) == LOW) {
            lastButtonPress = millis();  // Atualiza o tempo da última pressão
            mode = (mode == 3) ? 1 : mode + 1;  // Alterna entre os 3 modos
            updateMode();  // Atualiza a exibição do modo no LCD
        }

        // Inicia ou reinicia o jogo quando o botão "Select" é pressionado
        if (digitalRead(BUTTON_SELECT) == LOW) {
            lastButtonPress = millis();  // Atualiza o tempo da última pressão
            seqLength = 3;  // Reinicia o tamanho da sequência para 3 cores
            startGame();  // Inicia o jogo
        }
    }
}

// Função para atualizar o modo exibido no LCD
void updateMode() {
    lcd.setCursor(6, 0);  // Posiciona o cursor para atualizar o texto no LCD
    lcd.print("                ");  // Limpa a linha
    lcd.setCursor(6, 0);

    // Exibe o nome do modo atual no LCD
    if (mode == 1) {
        lcd.print("Normal");
    } else if (mode == 2) {
        lcd.print("Inverso");
    } else {
        lcd.print("Mem.Extrema");
    }
}

// Função principal do jogo, onde as rodadas ocorrem
void startGame() {
    while (true) {  // O jogo continua enquanto o jogador acerta
        generateSequence();  // Gera uma nova sequência de cores
        showSequence();  // Exibe a sequência para o jogador memorizar
        delay(2000);  // Aguarda 2 segundos para o jogador se preparar

        // Verifica se a entrada do jogador está correta, de acordo com o modo
        bool correct = (mode == 2) ? getPlayerInputReverse() : getPlayerInput();

        if (!correct) {  // Se o jogador errou, termina o jogo
            gameOver();
            break;  // Sai do loop e termina o jogo
        }

        // Exibe "Acertou!" se o jogador acertou a sequência
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Acertou!");
        delay(1000);  // Aguarda 1 segundo

        // Aumenta o tamanho da sequência conforme o modo de jogo
        if (mode == 3) {
            seqLength += 2;  // No modo Memória Extrema, aumenta 2 cores
        } else {
            seqLength++;  // Nos outros modos, aumenta 1 cor
        }

        // Limita o tamanho da sequência para no máximo 100 cores
        if (seqLength > 100) seqLength = 100;

        // Exibe "Próxima rodada!" e aguarda 1,5 segundos
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Proxima rodada!");
        delay(1500);
    }
}

// Função para gerar a sequência aleatória de cores
void generateSequence() {
    for (int i = 0; i < seqLength; i++) {
        sequence[i] = random(1, 5);  // Gera um número aleatório entre 1 e 4
    }
}

// Função para exibir a sequência de LEDs para o jogador memorizar
void showSequence() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Memorize...");

    // Acende os LEDs da sequência gerada
    for (int i = 0; i < seqLength; i++) {
        lightUpLED(sequence[i]);
        delay(600);  // Tempo de exibição de cada LED
        turnOffLEDs();  // Desliga todos os LEDs
        delay(300);  // Intervalo entre os LEDs
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sua vez...");
}

// Função para acender o LED correspondente à cor
void lightUpLED(int num) {
    int leds[] = {LED_RED, LED_GREEN, LED_BLUE, LED_YELLOW};
    digitalWrite(leds[num - 1], HIGH);  // Acende o LED correspondente
    delay(500);  // Fica aceso por 500ms
    digitalWrite(leds[num - 1], LOW);  // Apaga o LED
}

// Função para apagar todos os LEDs
void turnOffLEDs() {
    for (int i = 7; i <= 10; i++) digitalWrite(i, LOW);  // Desliga os LEDs
}

// Função para verificar a entrada do jogador (modo normal)
bool getPlayerInput() {
    for (int i = 0; i < seqLength; i++) {
        int buttonPressed = waitForButtonPress();  // Espera a entrada do jogador
        if (buttonPressed == -1 || buttonPressed != sequence[i]) return false;  // Se errar, retorna false
    }
    return true;  // Se acertar toda a sequência, retorna true
}

// Função para verificar a entrada do jogador (modo inverso)
bool getPlayerInputReverse() {
    for (int i = seqLength - 1; i >= 0; i--) {
        int buttonPressed = waitForButtonPress();  // Espera a entrada do jogador
        if (buttonPressed == -1 || buttonPressed != sequence[i]) return false;  // Se errar, retorna false
    }
    return true;  // Se acertar toda a sequência ao contrário, retorna true
}

// Função para aguardar a pressão de um botão e retornar a cor correspondente
int waitForButtonPress() {
    unsigned long startTime = millis();  // Marca o tempo de início
    while (millis() - startTime < 10000) {  // Aguarda até 10 segundos para a resposta
        for (int i = 0; i < 4; i++) {
            int buttons[] = {BUTTON_RED, BUTTON_GREEN, BUTTON_BLUE, BUTTON_YELLOW};
            if (digitalRead(buttons[i]) == LOW) {  // Verifica se o botão foi pressionado
                delay(50);  // Pequeno atraso para debouncing
                if (digitalRead(buttons[i]) == LOW) {  // Confirma a pressão do botão
                    lightUpLED(i + 1);  // Acende o LED correspondente
                    delay(100);  // Mantém o LED aceso por 100ms
                    return i + 1;  // Retorna o número da cor pressionada
                }
            }
        }
        delay(10);  // Pequeno atraso para evitar sobrecarga do microcontrolador
    }
    return -1;  // Se o tempo expirar sem entrada, retorna -1
}

// Função chamada quando o jogador erra a sequência
void gameOver() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Errou! Game Over");

    // Pisca os LEDs como uma indicação de erro
    for (int i = 0; i < 3; i++) {
        for (int j = 7; j <= 10; j++) digitalWrite(j, HIGH);
        delay(300);  // LEDs acesos por 300ms
        turnOffLEDs();
        delay(300);  // Espera 300ms antes de piscar novamente
    }

    delay(1500);  // Espera 1,5 segundos antes de reiniciar o jogo
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Modo: ");
    updateMode();  // Atualiza a tela com o modo atual
}
