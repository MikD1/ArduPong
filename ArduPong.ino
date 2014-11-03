#define LEDS_START_POSITION 2
#define RESET_BUTTON 12
#define P1_BUTTON 10
#define P2_BUTTON 11

const int LEDS_COUNT = 7;
const int CENTER_LED = LEDS_COUNT / 2;
const int MIN_WAIT_VALUE = 80;
const int MAX_WAIT_VALUE = 250;
const int WAIT_STEP = (MAX_WAIT_VALUE - MIN_WAIT_VALUE) / (LEDS_COUNT - 1);

int Leds[LEDS_COUNT];
int CurrentPosition = CENTER_LED;
bool Direction = true;
int WaitValue = MAX_WAIT_VALUE;
bool P1ButtonPreviouseState = false;
bool P2ButtonPreviouseState = false;

void StartedBlink();
void HandleButtons();
bool IsButtonPressed(int pin, bool *previouseState);
void Reset();
void SetWaitValue();
void Finish(int led);

void setup()
{
  for (int i = 0; i < LEDS_COUNT; ++i)
  {
    Leds[i] = LEDS_START_POSITION + i;
    pinMode(Leds[i], OUTPUT);
  }
  
  pinMode(RESET_BUTTON, INPUT_PULLUP);
  pinMode(P1_BUTTON, INPUT_PULLUP);
  pinMode(P2_BUTTON, INPUT_PULLUP);
  
  StartedBlink();
}

void loop()
{
  digitalWrite(Leds[CurrentPosition], LOW);  

  Direction ? ++CurrentPosition : --CurrentPosition;
  if (CurrentPosition < 0)
  {
    Finish(Leds[0]);
  }
  else if (CurrentPosition == LEDS_COUNT)
  {
    Finish(Leds[LEDS_COUNT - 1]);
  }
  else
  {
    digitalWrite(Leds[CurrentPosition], HIGH);
    for (int i = 0; i < WaitValue; ++i)
    {
      HandleButtons();
      delay(1);
    }
  }
}

void StartedBlink()
{
  bool value = true;
  while (true)
  {    
    for (int i = 0; i < LEDS_COUNT; ++i)
    {
      digitalWrite(Leds[i], value);
    }
    
    for (int i = 0; i < 200; ++i)
    {
      if (!digitalRead(RESET_BUTTON))
      {
        Reset();
        return;
      }
      
      delay(1);
    }
    
    value = !value;
  }
}

void HandleButtons()
{
  if (!digitalRead(RESET_BUTTON))
  {
    Reset();
    return;
  }
  
  if (IsButtonPressed(P1_BUTTON, &P1ButtonPreviouseState))
  {
    Direction = true;
    WaitValue = MIN_WAIT_VALUE + CurrentPosition * WAIT_STEP;
  }
  
  if (IsButtonPressed(P2_BUTTON, &P2ButtonPreviouseState))
  {
    Direction = false;
    WaitValue = MIN_WAIT_VALUE + (LEDS_COUNT - CurrentPosition - 1) * WAIT_STEP;
  }
}

bool IsButtonPressed(int pin, bool *previouseState)
{
  bool result = false;
  
  if (!*previouseState && !digitalRead(pin))
  {
    delay(10);
    if (!digitalRead(pin))
    {
      result = true;
    }
  }
  
  *previouseState = !digitalRead(pin);
  return result;
}

void Reset()
{  
  CurrentPosition = CENTER_LED;
  Direction = random(0, 2);
  WaitValue = MAX_WAIT_VALUE;
  P1ButtonPreviouseState = false;
  P2ButtonPreviouseState = false;
  
  for (int i = 0; i < LEDS_COUNT; ++i)
  {
    digitalWrite(Leds[i], LOW);
  }
  
  digitalWrite(Leds[CENTER_LED], HIGH);
  delay(800);
  digitalWrite(Leds[CENTER_LED], LOW);
}

void SetWaitValue()
{
  
}

void Finish(int led)
{
  bool value = true;
  while (true)
  {
    digitalWrite(led, value);
    
    for (int i = 0; i < 200; ++i)
    {
      if (!digitalRead(RESET_BUTTON))
      {
        Reset();
        return;
      }
      
      delay(1);
    }
    
    value = !value;
  }
}
