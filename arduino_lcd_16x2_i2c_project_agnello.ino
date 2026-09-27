// =========================================================================
// PROJETO FIAP: SISTEMA AUTOMATIZADO DE MONITORAMENTO DE LUMINOSIDADE PARA A EMPRESA FICTÍCIA VINHERIA AGNELLO
// GRUPO: CTRL + 5
// INTEGRANTES DO GRUPO: Eduardo, Flávia, Gabriel, Lirity e Nicolle
//
// PLACA UTILIZADA: Arduino Uno R3 com Display LCD 16x2 I2C (Endereço 0x20)
// EQUIPAMENTOS 
// 1x Arduino Uno R3
// 1x Display LCD 16x2 com módulo I2C
// 1x Breadboard
// 16x Fios Jumpers (Macho-Macho)
// 4x Fios Jumper (Macho-Fêmea)
// 2x Sensores de Luminosidade (Fotorresistores LDR)
// 3x LEDs (Vermelho, Amarelo e Verde)
// 1x Buzzer
// 5x Resistores (300 Ω)
//
// Observação: no vídeo é usado apenas 1 LDR.
// 
// BIBLIOTECAS
// * Wire
// * LiquidCrystal_I2C by Martin Kubovčík, Frank de Brabander
// =========================================================================
//
// O QUE ACONTECE NO CIRCUITO (PASSO A PASSO):
//
// 1. INICIALIZAÇÃO E ANIMAÇÃO:
//    Ativa a comunicação Serial (115200 baud) e o display LCD I2C.
//    Otimiza o uso da RAM guardando textos estáticos na memória Flash via F().
//    Ao ligar o circuito, o Arduino roda uma animação gráfica exclusiva no LCD
//    desenhando uma garrafa de vinho e nossa logo quadro a quadro. Logo após, exibe um
//    letreiro rolante com a mensagem de "Bem vindo(a) a Adega Agnello!".
//
// 2. LEITURA DOS SENSORES (LDRs):
//    O código lê continuamente a luminosidade de dois sensores LDR conectados
//    nas portas analógicas "A0" e "A1". Ele calcula a média das duas leituras para
//    evitar alarmes falsos caso ocorra uma sombra rápida em apenas um sensor.
//
// 3. CONVERSÃO PARA PORCENTAGEM:
//    O valor bruto de luz é convertido matematicamente para uma escala de
//    0% a 100%, facilitando o entendimento visual do monitoramento.
//
// 4. LÓGICA DE DECISÃO DOS PARÂMETROS DE SEGURANÇA:
//    * ESTADO CRÍTICO (Abaixo de 20% ou Acima de 90%):
//    - Ocorre se a adega estiver no escuro crítico ou com claridade excessiva.
//    - Ação: Desliga os outros LEDs, acende o LED Vermelho, ativa a buzina
//      (Buzzer) em nível lógico ALTO e mostra "CRITICO!" na segunda linha do LCD.
//
//    * ESTADO DE AVISO (Entre 20% e 39% ou Entre 70% e 89%):
//    - Ocorre quando a luz está quase fora do parâmetro ideal de conservação.
//    - Ação: Desliga os outros LEDs, acende o LED Amarelo, mantém a buzina
//      desligada em nível BAIXO e mostra "AVISO" na segunda linha do LCD.
//
//    * ESTADO SEGURO (Entre 40% e 69%):
//    - Ocorre quando a luminosidade está perfeita para armazenar os vinhos.
//    - Ação: Desliga os outros LEDs, acende o LED Verde, mantém a buzina
//      desligada em nível BAIXO e mostra "SEGURO" na segunda linha do LCD.
//
// =========================================================================



// Importa a biblioteca Wire e LiquidCrystal_I2Cpara o Display LCD 16x2 I2C - Reconhece novos comandos
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configuração do display I2C no Tinkercad: endereço 0x20, 16 colunas e 2 linhas
LiquidCrystal_I2C lcd(0x20, 16, 2);

// --- MAPEAMENTO DOS PINOS NO ARDUINO UNO R3 ---
const int pinoLDR1 = A1;       // Primeiro sensor LDR - Pino Analógico A1
const int pinoLDR2 = A0;       // Segundo sensor LDR - Pino Analógico A0
const int ledVermelho = 5;     // LED de Alarme (Muito claro ou muito escuro)
const int ledAmarelo = 6;      // LED de Crítico (Quase ideal)
const int ledVerde = 7;        // LED Seguro (Luminosidade perfeita para adega)
const int pinoBuzzer = 8;      // Buzzer/Buzina - Pino Digital 8

// --- ARRAYS DE BYTES PARA A ANIMAÇÃO DA GARRAFA ---
byte image01_f1[] = { B00000, B11111, B11111, B01110, B00100, B00100, B00100, B01110 };
byte image01_f2[] = { B00000, B00000, B11000, B00100, B01111, B11100, B11000, B00000 };
byte image02_f2[] = { B00000, B00000, B00000, B01000, B11000, B01000, B00000, B00000 };
byte image03_f2[] = { B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00001 };
byte image04_f2[] = { B00011, B00010, B00000, B00010, B00000, B00000, B00000, B00000 };
byte image01_f3[] = { B00000, B00000, B11000, B00100, B00011, B00100, B11000, B00000 };
byte image04_f3[] = { B00001, B00011, B00011, B00110, B00010, B00000, B00010, B00000 };
byte image01_f4[] = { B00000, B00001, B00000, B00001, B00010, B00001, B00011, B00110 };
byte image02_f4[] = { B00010, B00000, B00000, B00000, B00000, B00000, B00000, B00000 };
byte image01_f5[] = { B00010, B00100, B00100, B01100, B11000, B01001, B11011, B01110 };
byte image02_f5[] = { B00000, B00000, B00000, B10100, B11000, B10000, B10000, B00000 };
byte image02_f6[] = { B00000, B00000, B00000, B10110, B11001, B10000, B10000, B10000 };
byte image03_f6[] = { B00000, B00000, B00000, B01110, B10001, B01111, B11001, B01111 };
byte image04_f6[] = { B00001, B10001, B01110, B00000, B00000, B00000, B00000, B00000 };
byte image01_f7[] = { B00000, B00000, B00000, B01110, B11001, B01101, B11111, B01110 };
byte image05_f7[] = { B00000, B00000, B00000, B10110, B11001, B10001, B11111, B10000 };
byte image06_f7[] = { B10000, B10000, B10000, B00000, B00000, B00000, B00000, B00000 };
byte image07_f7[] = { B00000, B00000, B00000, B01110, B10001, B11111, B10000, B01111 };

// --- FUNÇÕES DA ANIMAÇÃO ---
void frame1() { 
  lcd.clear(); lcd.createChar(0, image01_f1);  
  lcd.setCursor(7, 0); lcd.write(byte(0)); 
}
void frame2() {
  lcd.clear(); lcd.createChar(0, image01_f2); lcd.createChar(1, image02_f2); lcd.createChar(2, image03_f2); lcd.createChar(3, image04_f2);
  lcd.setCursor(7, 0); lcd.write(byte(0)); lcd.setCursor(8, 0); lcd.write(byte(1)); lcd.setCursor(6, 0); lcd.write(byte(2)); lcd.setCursor(6, 1); lcd.write(byte(3));
}
void frame3() {
  lcd.clear(); lcd.createChar(0, image01_f3); lcd.createChar(1, image02_f2); lcd.createChar(2, image03_f2); lcd.createChar(3, image04_f3);
  lcd.setCursor(7, 0); lcd.write(byte(0)); lcd.setCursor(8, 0); lcd.write(byte(1)); lcd.setCursor(6, 0); lcd.write(byte(2)); lcd.setCursor(6, 1); lcd.write(byte(3));
}
void frame4() {
  lcd.clear(); lcd.createChar(0, image01_f4); lcd.createChar(1, image02_f4);
  lcd.setCursor(6, 0); lcd.write(byte(0)); lcd.setCursor(6, 1); lcd.write(byte(1));
}
void frame5() {
  lcd.clear(); lcd.createChar(0, image01_f5); lcd.createChar(1, image02_f5);
  lcd.setCursor(6, 0); lcd.write(byte(0)); lcd.setCursor(7, 0); lcd.write(byte(1));
}
void frame6() {
  lcd.clear(); lcd.createChar(0, image01_f5); lcd.createChar(1, image02_f6); lcd.createChar(2, image03_f6); lcd.createChar(3, image04_f6);
  lcd.setCursor(6, 0); lcd.write(byte(0)); lcd.setCursor(7, 0); lcd.write(byte(1)); lcd.setCursor(8, 0); lcd.write(byte(2)); lcd.setCursor(6, 1); lcd.write(byte(3));
}
void frame7() {
  lcd.clear(); lcd.createChar(0, image01_f7); lcd.createChar(1, image02_f6); lcd.createChar(2, image03_f6); lcd.createChar(3, image04_f6); lcd.createChar(4, image05_f7); lcd.createChar(5, image06_f7); lcd.createChar(6, image07_f7);
  lcd.setCursor(6, 0); lcd.write(byte(0)); lcd.setCursor(7, 0); lcd.write(byte(1)); lcd.setCursor(8, 0); lcd.write(byte(2)); lcd.setCursor(6, 1); lcd.write(byte(3));
  lcd.setCursor(9, 0); lcd.write(byte(4)); lcd.setCursor(9, 1); lcd.write(byte(5)); lcd.setCursor(10, 0); lcd.write(byte(6));
}

// Configuração Inicial
void setup() {
  Serial.begin(115200);
  Serial.println(F("INICIALIZANDO"));
  
  // Configuração dos pinos como Saída (OUTPUT)
  pinMode(ledVermelho, OUTPUT);
  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(pinoBuzzer, OUTPUT);

  
  lcd.init(); // Inicialização LCD I2C
  lcd.backlight(); // Ligar a luz de fundo (Backlight)
  lcd.clear(); // Limpa a tela

  // Execução fluida da animação da garrafa (Apenas uma vez ao ligar o LCD)
  // Frame1: Mostra a imagem do frame 1 / Delay: Espera um tempo para o próximo frame
  frame1(); delay(800);
  frame2(); delay(600);
  frame3(); delay(500);
  frame4(); delay(700);
  frame5(); delay(400);
  frame6(); delay(400);
  frame7(); delay(1350);

  // Apresenta o texto de Boas-Vindas
  lcd.clear();
  lcd.setCursor(0, 0);    
  lcd.print("Bem-vindo(a) a"); // Mostra a mensagem em texto na tela do LCD
  lcd.setCursor(0, 1); 
  lcd.print("Adega Agnello!"); 
  delay(1500);
  
  // Rola o letreiro de boas-vindas para a direita
  for(int x = 0; x < 16; x++) { // Repete o comando abaixo dele 16 vezes da mensagem em texto (0 até 15)
    lcd.scrollDisplayRight(); // Move todo o texto 1 coluna para a direita 
    delay(400);
  }
  
  // Limpa a tela para recomeçar o loop da imagem e espera um tempo antes de começar
  lcd.clear();
}

void loop() {
  // 1. LER OS SENSORES: Leituras simultâneas dos dois LDRs analógicos
  int valor1 = analogRead(pinoLDR1);
  int valor2 = analogRead(pinoLDR2);
  int mediaLuminosidade = (valor1 + valor2) / 2; // Calcula a média dos valores luminosos

  // 2. CONVERTER PARA %
  int porcentagemLuz = map(mediaLuminosidade, 6, 679, 0, 100); 
  porcentagemLuz = constrain(porcentagemLuz, 0, 100);

  // Saída de suporte para o Monitor Serial
  Serial.print("Luz Adegaria: ");
  Serial.print(porcentagemLuz);
  Serial.println("%");

  // 3. Mostrar dados fixos no LCD:
  lcd.setCursor(0, 0);       
  lcd.print("Luz Adegaria:   "); 
  
  // --- LÓGICA DE ALERTAS SIMPLIFICADA (Cores, Buzzer simples e Mensagem) ---

  // FORA DO PARâMETRO - CRÍTICO (Vermelho ON e Buzzer ON)
  // Níveis crítico do parâmetro: Abaixo de 20% ou Acima de 90%
  if (porcentagemLuz < 20 || porcentagemLuz > 90) {
    digitalWrite(ledVermelho, HIGH); // Liga Vermelho
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVerde, LOW);
    
    // ATIVA O SOM DO BUZZER
    tone(pinoBuzzer, 1000); // Liga a buzina
    
    // ALERTA NO LCD
    lcd.setCursor(0, 1);       
    lcd.print(porcentagemLuz); 
    lcd.print("% - CRITICO!   ");            
  }

  // QUASE FORA DO PARÂMETRO - AVISO (Amarelo ON e Buzzer OFF)
  // Níveis fora do parâmetro: Entre 20%-39% ou 70%-89%
  else if (porcentagemLuz < 40 || porcentagemLuz > 69) {
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAmarelo, HIGH); // Liga Amarelo
    digitalWrite(ledVerde, LOW);

    // DESATIVA O SOM DO BUZZER
    noTone(pinoBuzzer); // Desliga a buzina
    
    // ALERTA NO LCD
    lcd.setCursor(0, 1);       
    lcd.print(porcentagemLuz); 
    lcd.print("% - ALERTA!    ");            
  }
  // DENTRO DO PARâMETRO - SEGURO (Verde ON e Buzzer OFF) 
  // Níveis ideais do parâmetro: Entre 40% e 69%
  else {
    digitalWrite(ledVermelho, LOW);
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVerde, HIGH); // Liga Verde
    
    noTone(pinoBuzzer); // Desliga a buzina
    
    lcd.setCursor(0, 1);       
    lcd.print(porcentagemLuz); 
    lcd.print("% - SEGURO    ");            
  }
  // Tempo em milissegundos antes de retomar o loop
  delay(250); 
}
