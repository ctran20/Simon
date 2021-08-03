//--------------------------------------------------------//
//                        Simon                           //
//                       Cat Tran                         //
//                       02/23/19                         //
//--------------------------------------------------------//

#include "pitches.h"

byte seven_seg_digits[10] = { B11111100,  // = 0
                              B01100000,  // = 1
                              B11011010,  // = 2
                              B11110010,  // = 3
                              B01100110,  // = 4
                              B10110110,  // = 5
                              B10111110,  // = 6
                              B11100000,  // = 7
                              B11111110,  // = 8
                              B11110110   // = 9
                             };

const int latchPin = 5;
const int clockPin = 6;
const int dataPin = 3;

const int LED1 = 8;
const int LED2 = 7;
const int LED3 = 4;
const int LED4 = 2;

const int buttonA = 13;
const int buttonB = 12;
const int buttonC = 11;
const int buttonD = 10;

const int duration = 300;
const int pause = 500;

const int rounds = 10;

const int melody[] = {NOTE_C5, NOTE_E5, NOTE_G5, NOTE_B5, NOTE_C6};

//Non-constant values
int level = 0;
int randomNum[rounds];
int playerInput[rounds];

bool won = false;
bool gameOver = false;
bool pressed;

void setup() {
  
  pinMode(buttonA, INPUT);
  pinMode(buttonB, INPUT);
  pinMode(buttonC, INPUT);
  pinMode(buttonD, INPUT);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  Serial.begin(9600);

  digitalWrite(buttonB, HIGH);
  digitalWrite(buttonC, HIGH);
  digitalWrite(buttonD, HIGH);

  //Shuffle random sequences
  randomSeed(analogRead(0));

  //Input random number for every round
  for(int i = 0; i < rounds; i++)
  {
    randomNum[i] = random(4);
  }



  Start();
}

//--------------------------Main-----------------------------
void loop() {

  //Stop the loop when the game is over
  if(gameOver)
  {
    return;
  }

  //Endless loop when the game is finished
  while(won)
  {
    Finished();
  }

  NumWrite(level-1); 

  game(level);


  //Check to see if player won
  Win(level);

  //Increment level
  level++;
}
//-----------------------------------------------------------



void game(int& level)
{
  int inputN;

  for(int i = 0; i < level; i++)
  {
    Serial.print(randomNum[i]);
    Serial.println(" ");
    
    LightAndSound(randomNum[i]);
    
    delay(400);
  }
  
  for(int i = 0; i < level; i++)
  {

    inputN = ButtonInput();

    LightAndSound(inputN);

    //Check to see if player input is correct
    if(inputN != randomNum[i])
    {
      Lose();
      level = 0;
      break;
    }
  }

  delay(300);
}
//-----------------------------------------------------------


//-----------------------------------------------------------
//
int ButtonInput()
{
  while(!digitalRead(buttonA) == 0 || !digitalRead(buttonB) == 0 || !digitalRead(buttonC) == 0 || !digitalRead(buttonD) == 0)
  {
    if(digitalRead(buttonA) == 0)
    {
      return 0;
    }
    else if(digitalRead(buttonB) == 0)
    {
      return 1;
    }
    else if(digitalRead(buttonC) == 0)
    {
      return 2;
    }
    else if(digitalRead(buttonD) == 0)
    {
      return 3;
    }
  }
}
//-----------------------------------------------------------


//-----------------------------------------------------------
//LightAndSound - Turn on an LED based on the input number
void LightAndSound(int i)
{
  if(i == 0)
  {
    digitalWrite(LED1, HIGH);
    tone(9, melody[0], duration);
  }
  else if(i == 1)
  {
    digitalWrite(LED2, HIGH);
    tone(9, melody[1], duration);
  }
  else if(i == 2)
  {
    digitalWrite(LED3, HIGH);
    tone(9, melody[2], duration);
  }
  else if(i == 3)
  {
    digitalWrite(LED4, HIGH);
    tone(9, melody[4], duration);
  }
  else
  {
    Invalid();
  }
  
  delay(200);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
}
//-----------------------------------------------------------


//-----------------------------------------------------------
//Invalid - Light up all LEDs indicating that there's an error
void Invalid()
{
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);

  tone(9, melody[4], 120);
  delay(120);
  tone(9, melody[3], 150);

  gameOver = true;

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST,B10001110);  
  digitalWrite(latchPin, HIGH);

  //Stop the game and indicate error
  while(gameOver)
  {
    digitalWrite(LED1, LOW);
    delay(500);
    digitalWrite(LED1, HIGH);
    delay(500);
  }
}
//-----------------------------------------------------------


//-----------------------------------------------------------
//Start - Play the start sequence
void Start()
{
  digitalWrite(LED1, HIGH);
  tone(9, melody[0], 200);
  delay(300);

  digitalWrite(LED2, HIGH);
  tone(9, melody[1], 100);
  delay(100);

  digitalWrite(LED3, HIGH);
  tone(9, melody[2], 200);
  delay(200);
  
  digitalWrite(LED4, HIGH);
  tone(9, melody[4], 200);
  delay(150);
  digitalWrite(LED4, LOW);
  delay(50);

  digitalWrite(LED4, HIGH);
  tone(9, melody[4], 200);
  delay(300);

  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);

  delay(300);

}
//-----------------------------------------------------------


//-----------------------------------------------------------
//Win - Play victory music when finished all rounds
void Win(int wRounds)
{
  //Stop the game when finish all rounds
  if(wRounds == rounds)
  {
    //-------------------------------
    digitalWrite(LED1, HIGH);
    tone(9, melody[0], 500);
    delay(450);
    digitalWrite(LED1, LOW);
    delay(50);

    digitalWrite(LED1, HIGH);
    tone(9, melody[0], 250);
    delay(200);
    digitalWrite(LED1, LOW);
    delay(50);

    digitalWrite(LED1, HIGH);
    tone(9, melody[0], 250);
    delay(250);

    //-------------------------------
    digitalWrite(LED2, HIGH);
    tone(9, melody[1], 500);
    delay(450);
    digitalWrite(LED2, LOW);
    delay(50);

    digitalWrite(LED2, HIGH);
    tone(9, melody[1], 250);
    delay(200);
    digitalWrite(LED2, LOW);
    delay(50);
  
    digitalWrite(LED2, HIGH);
    tone(9, melody[1], 250);
    delay(250);

    //-------------------------------
    digitalWrite(LED3, HIGH);
    tone(9, melody[2], 250);
    delay(200);
    digitalWrite(LED3, LOW);
    delay(50);
  
    digitalWrite(LED3, HIGH);
    tone(9, melody[2], 250);
    delay(200);
    digitalWrite(LED3, LOW);
    delay(50);

    digitalWrite(LED3, HIGH);
    tone(9, melody[2], 250);
    delay(250);

    //-------------------------------
    digitalWrite(LED4, HIGH);
    tone(9, melody[4], 250);
    delay(200);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    delay(50);

    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    tone(9, melody[4], 500);
    delay(500);

    won = true;
  }
}
//-----------------------------------------------------------


//-----------------------------------------------------------
//Finished - Play a light show
void Finished()
{
  digitalWrite(LED1, LOW);
  delay(100);
  digitalWrite(LED2, LOW);
  delay(100);
  digitalWrite(LED3, LOW);
  delay(100);
  digitalWrite(LED4, LOW);
  delay(100);
  digitalWrite(LED1, HIGH);
  delay(100);
  digitalWrite(LED2, HIGH);
  delay(100);
  digitalWrite(LED3, HIGH);
  delay(100);
  digitalWrite(LED4, HIGH);
  delay(100);
}
//-----------------------------------------------------------


//-----------------------------------------------------------
//Lose - Play the loss sequence and stop the loop
void Lose()
{
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, HIGH);
  digitalWrite(LED3, HIGH);
  digitalWrite(LED4, HIGH);

  tone(9, melody[4], 450);
  delay(500);
  tone(9, melody[2], 300);
  delay(300);
  tone(9, melody[0], 600);
  delay(600);

  digitalWrite(LED1, LOW);
  delay(100);
  digitalWrite(LED2, LOW);
  delay(100);
  digitalWrite(LED3, LOW);
  delay(100);
  digitalWrite(LED4, LOW);
  delay(100);

  gameOver = true;
}
//-----------------------------------------------------------

void NumWrite(byte digit) {
  // set the latchPin to low potential, before sending data
  digitalWrite(latchPin, LOW);
     
  // the original data (bit pattern)
  shiftOut(dataPin, clockPin, LSBFIRST, seven_seg_digits[digit]);  
 
  // set the latchPin to high potential, after sending data
  digitalWrite(latchPin, HIGH);
}
