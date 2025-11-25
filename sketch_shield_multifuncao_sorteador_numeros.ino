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
  ANIMATION_FINALE,
  ANIMATION_INTERRUPTED
};

AnimationModeValues currentAnim = ANIMATION_STOPPED;
GeneratorModeValues currentState = GENERATOR_STOPPED;
RangeStatusValues rangeMaxFlag = RANGE_NOT_SET;
RangeStatusValues rangeMinFlag = RANGE_NOT_SET;
ledModeValues currentLedMode = LED_ALL_OFF;
int currentMax = RANGE_MAX_NUMBER;
int currentMin = RANGE_MIN_NUMBER;
int countup = 0;
int value = 0;

void AnimateLeds()
{
  switch (currentLedMode)
  {
    case LED_ALL_OFF:
      currentLedMode = LED_1_ON;
      MFS.writeLeds(LED_1, ON);
      break;
    case LED_1_ON:
      MFS.writeLeds(LED_1, OFF);
      currentLedMode = LED_2_ON;
      MFS.writeLeds(LED_2, ON);
      break;
    case LED_2_ON:
      MFS.writeLeds(LED_2, OFF);
      currentLedMode = LED_3_ON;
      MFS.writeLeds(LED_3, ON);
      break;
    case LED_3_ON:
      MFS.writeLeds(LED_3, OFF);
      currentLedMode = LED_4_ON;
      MFS.writeLeds(LED_4, ON);
      break;
    case LED_4_ON:
      MFS.writeLeds(LED_4, OFF);
      currentLedMode = LED_ALL_OFF;
      break;
  }
}

void DisplayRange(int range, bool blink) {
  MFS.write(range);
  if (blink)
    MFS.blinkDisplay(DIGIT_ALL, ON);
  else
    MFS.blinkDisplay(DIGIT_ALL, OFF);
}

void IncrementRange(int &range) {
  range = range + 100;
  if (range > RANGE_MAX_NUMBER)
    range = RANGE_MAX_NUMBER;
  if (range > currentMax)
    range = range - 100;
  MFS.write(range);
}

void DecrementRange(int &range) {
  range = range - 100;
  if (range < RANGE_MIN_NUMBER)
    range = RANGE_MIN_NUMBER;
  if (range < currentMin)
    range = range + 100;
  MFS.write(range);
}

void SetMaxRange() {
  rangeMaxFlag = RANGE_SET;
  MFS.writeLeds(LED_1, ON);
}

void SetMinRange() {
  rangeMinFlag = RANGE_SET;
  MFS.writeLeds(LED_2, ON);
}

void GeneratorStopped()
{
  currentState = GENERATOR_STOPPED;
  MFS.write("off");
  MFS.blinkDisplay(DIGIT_ALL, OFF);
}

void Buzzer(byte bipLength, byte silenceLength, byte bipLoops, byte totalLoops, byte waitBetweenLoops)
{
  MFS.beep(bipLength,           // Define um bip
           silenceLength,       // Define um silencio
           bipLoops,            // Define o loop de reprodução do bip
           totalLoops,          // Define a quantidade de execuções do loop
           waitBetweenLoops);   // Define o tempo de espera entre os loops
}
 
void Interrupt()
{
  currentAnim = ANIMATION_INTERRUPTED;
}

void Raffle()
{
  if (rangeMaxFlag != RANGE_SET || rangeMinFlag != RANGE_SET) {
    return;
  }

  //randomSeed(analogRead(A0));
  value = random(currentMin, currentMax + 1);

  currentState = GENERATOR_STARTED;
  currentAnim = ANIMATION_STARTED;
}

void shortAction3()
{
  switch (currentState)
  {
    case GENERATOR_STOPPED: DisplayRange(currentMin, false); break;
    case GENERATOR_STARTED: Serial.println("Not implemented"); break;
    case SETTING_RANGE_MAX_NUM_STARTED: DecrementRange(currentMax); break;
    case SETTING_RANGE_MIN_NUM_STARTED: DecrementRange(currentMin); break;
  }
}

void longAction3()
{
  switch (currentState)
  {
    case GENERATOR_STOPPED: DisplayRange(currentMin, true); currentState = SETTING_RANGE_MIN_NUM_STARTED; break;
    case GENERATOR_STARTED: Serial.println("Not implemented"); break;
    case SETTING_RANGE_MAX_NUM_STARTED: DecrementRange(currentMax); break;
    case SETTING_RANGE_MIN_NUM_STARTED: DecrementRange(currentMin); break;
  }
}

void shortAction2()
{
  switch (currentState)
  {
    case GENERATOR_STOPPED: DisplayRange(currentMax, false); break;
    case GENERATOR_STARTED: Serial.println("Not implemented"); break;
    case SETTING_RANGE_MAX_NUM_STARTED: IncrementRange(currentMax); break;
    case SETTING_RANGE_MIN_NUM_STARTED: IncrementRange(currentMin); break;
  }
}

void longAction2()
{
  switch (currentState)
  {
    case GENERATOR_STOPPED: DisplayRange(currentMax, true); currentState = SETTING_RANGE_MAX_NUM_STARTED; break;
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
    case SETTING_RANGE_MAX_NUM_STARTED: GeneratorStopped(); SetMaxRange(); break;
    case SETTING_RANGE_MIN_NUM_STARTED: GeneratorStopped(); SetMinRange(); break;
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

void button3(byte buttonAction)
{
  switch (buttonAction) {
    case BUTTON_SHORT_RELEASE_IND: shortAction3(); break;
    case BUTTON_LONG_PRESSED_IND: longAction3(); break;
    default: return;
  }
}

void button2(byte buttonAction)
{
  switch (buttonAction) {
    case BUTTON_SHORT_RELEASE_IND: shortAction2(); break;
    case BUTTON_LONG_PRESSED_IND: longAction2(); break;
    default: return;
  }
}

void button1(byte buttonAction)
{
  switch (buttonAction) {
    case BUTTON_SHORT_RELEASE_IND: shortAction1(); break;
    case BUTTON_LONG_PRESSED_IND: longAction1(); break;
    default: return;
  }
}

void check_buttons()
{
  byte btn = MFS.getButton();                            // verifica o status dos botões
 
  if (btn)                                               // se valor <> zero, botão pressionado  
  {
    byte buttonNumber = btn & B00111111;                 // AND bit a bit para identificar o botão
    byte buttonAction = btn & B11000000;                 // AND bit a bit para identificar o evento
 
    //Serial.print("BOTAO_");                              // imprime mensagem
    //Serial.write(buttonNumber + '0');                    // imprime o caractere da tabela ASCII correspondente ao código fornecido
    //Serial.print("_");                                   // imprime caracttere sublinhado
    switch (buttonNumber) {
      case 1: button1(buttonAction); break;
      case 2: button2(buttonAction); break;
      case 3: button3(buttonAction); break;
      default: return;
    }
  }
}

void play_animation()
{
  if (currentAnim == ANIMATION_STOPPED)
    return;
  
  AnimateLeds();
  switch (currentAnim)
  {
  case ANIMATION_STARTED:
    Buzzer(5,0,1,1,0);
    currentAnim = ANIMATION_STAGE1;
    countup = 0;
    break;
  case ANIMATION_STAGE1:
    MFS.write(countup*1000 + countup*100 + countup*10 + countup);
    countup++;
    if (countup > 9) {
      countup = 0;
      currentAnim = ANIMATION_STAGE2;
    }
    break;
  case ANIMATION_STAGE2:
    MFS.write(countup*1000 + countup*100 + countup*10 + value % 10);
    countup++;
    if (countup > 9) {
      countup = 0;
      currentAnim = ANIMATION_STAGE3;
    }
    break;
  case ANIMATION_STAGE3:
    MFS.write(countup*1000 + countup*100 + value % 100);
    countup++;
    if (countup > 9) {
      countup = 0;
      currentAnim = ANIMATION_STAGE4;
    }
    break;
  case ANIMATION_STAGE4:
    MFS.write(countup*1000 + value % 1000);
    countup++;
    if (countup > 9) {
      countup = 0;
      currentAnim = ANIMATION_FINALE;
    }
    break;
  case ANIMATION_FINALE:
    MFS.write(value);
    MFS.blinkDisplay(DIGIT_ALL, OFF);
    Buzzer(50,50,3,1,0);
    currentState = GENERATOR_STOPPED;
    currentAnim = ANIMATION_STOPPED;
    break;
  case ANIMATION_INTERRUPTED:
    MFS.write("Intr");
    MFS.writeLeds(LED_ALL, OFF);
    Buzzer(5,5,3,1,0);
    rangeMaxFlag = RANGE_NOT_SET;
    rangeMinFlag = RANGE_NOT_SET;
    currentState = GENERATOR_STOPPED;
    currentAnim = ANIMATION_STOPPED;
    break;
  default:
    break;
  }
  delay(100);
}

void setup()
{
  Serial.begin(9600);
  Timer1.initialize(); // inicializa o Timer 1
  MFS.initialize(&Timer1); // initializa a biblioteca Multi função
  GeneratorStopped();
  MFS.writeLeds(LED_ALL, OFF);
}

void loop()
{
  check_buttons();
  play_animation();
}
