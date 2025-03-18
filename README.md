# Genius-PIC
Esse é o projeto final da disciplina Projeto Integrado de Computação I, do curso de engenharia de computação da UFES. São contribuidores desse projeto: Silvio Junior, Enrico Massariol e Matheus Gon.

Resumo:

    Nosso projeto será um jogo inspirado no clássico Genius, desenvolvido com Arduino. Ele contará com LEDs coloridos, botões físicos e um alto-falante para emitir sons correspondentes às cores. O objetivo é reproduzir corretamente as sequências de luzes e sons geradas pelo sistema. A dificuldade será aumentada gradativamente a cada resposta correta, adicionando mais elementos à sequência. O hardware será programado em C/C++ com a placa Arduino Uno e incluirá feedback sonoro e visual, além de um sistema de pontuação exibido em um display. O design será portátil e pensado para oferecer uma experiência desafiadora e interativa.

Modificações:

    Foram adicionados dois novos modos ao jogo para aumentar a variedade e o desafio. O primeiro, "Memória Extrema", adiciona duas novas cores a cada dois acertos, tornando a memorização mais difícil. O segundo, "Modo Inverso", exige que o jogador reproduza a sequência ao contrário, adicionando uma nova cor a cada acerto.

    Além disso, decidimos remover o buzzer, pois o som não era tão eficiente nem agradável, resultando em um jogo sem áudio e focado exclusivamente no feedback visual e na interação com os botões. Essa mudança proporcionou uma experiência mais limpa, sem distrações sonoras, além de simplificar o circuito e evitar possíveis interferências no funcionamento do sistema.

Componentes Utilizados:

    Arduino (Modelo UNO): Controlador principal do jogo.
    LCD 16x2: Para exibir o modo de jogo e mensagens como "Memorize", "Sua vez", "Acertou!" e "Game Over".
    LEDs (Vermelho, Verde, Azul, Amarelo): Representam as cores da sequência que o jogador precisa memorizar e repetir.
    Botões (6 Botões, um para cada cor): Usados para o jogador interagir com o jogo, repetindo a sequência apresentada pelos LEDs.
    Resistores: Para os botões e LEDs.

Funcionamento do Jogo:

    Início do Jogo:

        O jogo começa em Modo Normal, onde o jogador deve memorizar uma sequência crescente de cores.
        O jogador pode escolher entre três modos:
            Normal: O jogador deve repetir a sequência apresentada pelos LEDs.
            Inverso: O jogador deve repetir a sequência, mas ao contrário.
            Memória Extrema: A cada rodada, a sequência aumenta em duas cores, tornando o jogo mais desafiador.
            
    Aumentando a Dificuldade:

        A cada rodada, se o jogador acertar a sequência, o comprimento da sequência aumenta.
        No modo Memória Extrema, a sequência cresce em 2 cores a cada rodada, enquanto nos outros modos, ela cresce apenas em 1 cor.

    Como Jogar:

        O LCD 16x2 exibe instruções como "Memorize..." e "Sua vez...".
        O jogador deve pressionar o botão correspondente à cor correta da sequência mostrada pelos LEDs.
        Se o jogador pressionar a sequência corretamente, o jogo continuará e a sequência será aumentada.
        Se o jogador errar, o jogo termina e uma mensagem de "Game Over" é exibida.

    Esquemático dos LEDs e Botões:

        LCD 16x2 com I2C:

            VSS → GND
            VDD → 5V
            V0 → Potenciômetro (controle de contraste, pino central)
            RS → Pino 7 do Arduino
            RW → GND
            E → Pino 8 do Arduino
            D4 → Pino 9 do Arduino
            D5 → Pino 10 do Arduino
            D6 → Pino 11 do Arduino
            D7 → Pino 12 do Arduino
            A (Anodo) → 5V (para o backlight)
            K (Cátodo) → GND

        LEDs (Vermelho, Verde, Azul, Amarelo):

            LED_RED → Pino 7 do Arduino
            LED_GREEN → Pino 8 do Arduino
            LED_BLUE → Pino 9 do Arduino
            LED_YELLOW → Pino 10 do Arduino
            Resistor de 220Ω em série com cada LED para limitar a corrente.

        Botões:

            BUTTON_SELECT → Pino 2 do Arduino (inicia o jogo)
            BUTTON_MODE → Pino 12 do Arduino (alternar entre os modos)
            BUTTON_RED → Pino 3 do Arduino
            BUTTON_GREEN → Pino 4 do Arduino
            BUTTON_BLUE → Pino 5 do Arduino
            BUTTON_YELLOW → Pino 6 do Arduino
            Resistores de 10kΩ para PULLDOWN em cada botão (pino entre o botão e o GND).

        Alimentação:

            Arduino alimentado com 5V da fonte USB ou fonte externa.
            LCD e LEDs também alimentados com 5V do Arduino.

        Dependências necessárias para rodar o código:

            LiquidCrystal_I2C:
                Biblioteca para controlar o LCD com interface I2C.

            Wire:
                Biblioteca padrão para comunicação I2C.
                Já incluída no IDE do Arduino por padrão.

                    