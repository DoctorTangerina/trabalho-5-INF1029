#include <TimerOne.h> // Bibliotec TimerOne
#include <Wire.h>
#include <MultiFuncShield.h> // Biblioteca Multifunction shield

#define RANGE_MAX_NUMBER 9999
#define RANGE_MIN_NUMBER 0000

enum GeneratorModeValues
{
  GENERATOR_STOPPED,
  GENERATOR_STARTED,
  SETTING_RANGE_MAX_NUM_STARTED,
  SETTING_RANGE_MIN_NUM_STARTED
};
enum RangeStatusValues
{
  RANGE_NOT_SET,
  RANGE_SET
};
enum ledModeValues
{
  LED_ALL_OFF,
  LED_1_ON,
  LED_2_ON,
  LED_3_ON,
  LED_4_ON
};
enum AnimationModeValues
{
  ANIMATION_STOPPED,
  ANIMATION_STARTED,
  ANIMATION_STAGE1,
  ANIMATION_STAGE2,
  ANIMATION_STAGE3,
  ANIMATION_STAGE4,
  ANIMATION_INTERRUPTED
};

AnimationModeValues currentAnim = ANIMATION_STOPPED;
GeneratorModeValues currentState = GENERATOR_STOPPED;
RangeStatusValues rangeFlag = RANGE_SET;
ledModeValues currentLedMode = LED_ALL_OFF;
int currentMax = RANGE_MAX_NUMBER;
int currentMin = RANGE_MIN_NUMBER;

void DisplayRange(int range) {
  MFS.write(range);
}

void IncrementRange(int range) {
  range = range + 100;
  if (range > RANGE_MAX_NUMBER)
    range = RANGE_MAX_NUMBER;
}

void DecrementRange(int range) {
  range = range - 100;
  if (range < RANGE_MIN_NUMBER)
    range = RANGE_MIN_NUMBER;
}
 
void Buzzer()
{
  // 4 bips curtos, repetidos 3 vezes
  MFS.beep(5,       // Define um bip de 50 ms (5 unidades de 10ms)
           5,       // Define um silencio de 50 ms (5 unidades de 10ms)
           4,       // Define o loop de reprodução do bip (4 vezes)
           3,       // Define a quantidade de execuções do loop (3 vezes)
           50);     // Define o tempo de espera entre os loops (500 ms)
}
 
void Interrupt()
{
  MFS.write("Intr");
}

void Raffle()
{
  
}

void shortAction3()
{
  switch (currentState)
  {
    case GENERATOR_STOPPED: DisplayRange(currentMin); break;
    case GENERATOR_STARTED: Serial.println("Not implemented"); break;
    case SETTING_RANGE_MAX_NUM_STARTED: DecrementRange(currentMax); break;
    case SETTING_RANGE_MIN_NUM_STARTED: DecrementRange(currentMin); break;
  }
}

void longAction3()
{
  switch (currentState)
  {
    case GENERATOR_STOPPED: currentState = SETTING_RANGE_MIN_NUM_STARTED; break;
    case GENERATOR_STARTED: Serial.println("Not implemented"); break;
    case SETTING_RANGE_MAX_NUM_STARTED: DecrementRange(currentMax); break;
    case SETTING_RANGE_MIN_NUM_STARTED: DecrementRange(currentMin); break;
  }
}

void shortAction2()
{
  switch (currentState)
  {
    case GENERATOR_STOPPED: DisplayRange(currentMax); break;
    case GENERATOR_STARTED: Serial.println("Not implemented"); break;
    case SETTING_RANGE_MAX_NUM_STARTED: IncrementRange(currentMax); break;
    case SETTING_RANGE_MIN_NUM_STARTED: IncrementRange(currentMin); break;
  }
}

void longAction2()
{
  switch (currentState)
  {
    case GENERATOR_STOPPED: currentState = SETTING_RANGE_MAX_NUM_STARTED; break;
    case GENERATOR_STARTED: Serial.println("Not implemented"); break;
    case SETTING_RANGE_MAX_NUM_STARTED: IncrementRange(currentMax); break;
    case SETTING_RANGE_MIN_NUM_STARTED: IncrementRange(currentMin); break;
  }
}

void shortAction1()
{
  switch (currentState)
  {
    case GENERATOR_STOPPED: Raffle(); break;
    case GENERATOR_STARTED: Interrupt(); break;
    case SETTING_RANGE_MAX_NUM_STARTED: currentState = GENERATOR_STOPPED; break;
    case SETTING_RANGE_MIN_NUM_STARTED: currentState = GENERATOR_STOPPED; break;
  }
}

void longAction1()
{
  switch (currentState)
  {
    case GENERATOR_STOPPED: Serial.println("Not implemented"); break;
    case GENERATOR_STARTED: Serial.println("Not implemented"); break;
    case SETTING_RANGE_MAX_NUM_STARTED: Serial.println("Not implemented"); break;
    case SETTING_RANGE_MIN_NUM_STARTED: Serial.println("Not implemented"); break;
  }
}

void botao3(byte buttonAction)
{
  switch (buttonAction) {
    case BUTTON_SHORT_RELEASE_IND: shortAction3(); break;
    case BUTTON_LONG_RELEASE_IND: longAction3(); break;
    default: return;
  }
}

void botao2(byte buttonAction)
{
  switch (buttonAction) {
    case BUTTON_SHORT_RELEASE_IND: shortAction2(); break;
    case BUTTON_LONG_RELEASE_IND: longAction2(); break;
    default: return;
  }
}

void botao1(byte buttonAction)
{
  switch (buttonAction) {
    case BUTTON_SHORT_RELEASE_IND: shortAction1(); break;
    case BUTTON_LONG_RELEASE_IND: longAction1(); break;
    default: return;
  }
}

void botoes()
{
  byte btn = MFS.getButton();                            // verifica o status dos botões
 
  if (btn)                                               // se valor <> zero, botão pressionado  
  {
    byte buttonNumber = btn & B00111111;                 // AND bit a bit para identificar o botão
    byte buttonAction = btn & B11000000;                 // AND bit a bit para identificar o evento
 
    Serial.print("BOTAO_");                              // imprime mensagem
    Serial.write(buttonNumber + '0');                    // imprime o caractere da tabela ASCII correspondente ao código fornecido
    Serial.print("_");                                   // imprime caracttere sublinhado
    char num = buttonNumber + '0';
    switch (num) {
      case '1': botao1(buttonAction); break;
      case '2': botao2(buttonAction); break;
      case '3': botao3(buttonAction); break;
      default: return;
    }
  }
}

void setup()
{
  Serial.begin(9600);
  Timer1.initialize(); // inicializa o Timer 1
  MFS.initialize(&Timer1); // initializa a biblioteca Multi função
}

void loop()
{
  botoes();
}
