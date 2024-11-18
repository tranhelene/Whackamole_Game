/* Name: Helene Tran            
 * Student Number: 98092190
 * CWL & UBC email: htran31@student.ubc.ca
 * Lab Section: L1Y
 * Assignment: Lab 8 - Whack a Mole      
 */      

//Define game parameters (Modify the numbers only)
#define NUM_PROMPTS   5    //modifies the amount of prompts per round
#define ROUNDS        5    //rounds per game
#define T_ON          1000 //the amount of time the LEDs are ON
#define K             0.95  //the constant to multiply by every successive round

//Define Pins (DO NOT CHANGE)

//LEDs
#define LEDR 12
#define LEDY 11
#define LEDB 10
#define LEDG 9

//push buttons
#define PB_RED 		7
#define PB_YELLOW 	6
#define PB_BLUE		5
#define PB_GREEN 	4
#define PB_START 	13

//define functions prototypes
int check(int button, int light, int roundCount);

void result(int roundCount, int score);

//initialization
void setup(){
  //configure pin modes (inputs, outputs) etc.
  
  //LEDs
  pinMode(LEDR, OUTPUT);
  pinMode(LEDY, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDG, OUTPUT);
  
  //push buttons
  pinMode(PB_RED, INPUT);
  pinMode(PB_YELLOW, INPUT);
  pinMode(PB_BLUE, INPUT);
  pinMode(PB_GREEN, INPUT);
  pinMode(PB_START, INPUT);
  
  //serial monitor
  Serial.begin(9600);
  
  //random function
  randomSeed(analogRead(0));
}

//main program loop
void loop(){
  int promptCount, roundCount;
  int on = T_ON, off = T_ON;
  int light, button;
  int score = 0, scoring = -1;
  int rightAnswers = 0;
  unsigned long currentTime, endTime;
  
  //begins game when start button is pressed
  if(digitalRead(PB_START) == HIGH){ 
    
    //displays light pattern at start of game
    for(light = LEDG; light <= LEDR; light++){
      digitalWrite(light, HIGH);
      delay(200);
      
      digitalWrite(light, LOW);
    }
    
    //pause before game starts
    delay(1000);
    
    //keeps track of the rounds
    for(roundCount = 1; roundCount <= ROUNDS; roundCount++){ 
      
      //keeps track of the prompts
      for(promptCount = 1; promptCount <= NUM_PROMPTS; promptCount++){
        
        //randomizes light
        light = random(9, 13);
        button = light - 5;
        
      	digitalWrite(light, HIGH);
        
        currentTime = millis();
        endTime = currentTime + on;
        
        while(millis() < endTime){
          
          //if any button is pressed
          if(digitalRead(PB_RED) == HIGH || digitalRead(PB_YELLOW) == HIGH || digitalRead(PB_BLUE) == HIGH || digitalRead(PB_GREEN) == HIGH){
            
            //checks if correct button was pressed
            scoring = check(button, light, roundCount);
          }
        }
        digitalWrite(light, LOW);
        
        //checks for correct button again if user did not press a button while light was on
        if(scoring == 0){
          currentTime = millis();
          endTime = currentTime + off;
          
          while(millis() < endTime){
            if(digitalRead(PB_RED) == HIGH || digitalRead(PB_YELLOW) == HIGH || digitalRead(PB_BLUE) == HIGH || digitalRead(PB_GREEN) == HIGH){
              scoring = check(button, light, roundCount);
            }
          }
        }
        
        //if any button was pressed
        if(scoring != -1){
          
          //adds to total score
          score += scoring;
          
          //if correct button was pressed
          if(scoring != 0){
            rightAnswers++;
          }
        }
 
        //terminates game if no button was pressed
        else{
          promptCount = NUM_PROMPTS;
          roundCount = ROUNDS + 2;
        } 
        
        //reinitialize after each prompt
        scoring = -1;
          
        //prints current score on serial monitor
        result(roundCount, score);
        
        //pause between prompts
        delay(500);
      }
      
      //pause between rounds
      delay(500);
        
      //shortens light on and off times
      on = off *= K;
        
      //displays light pattern after a successful round
      if(rightAnswers == 5){
        for(light = LEDR; light >= LEDG; light--){
          digitalWrite(light, HIGH);
          delay(200);
          
          digitalWrite(light, LOW);
        }
        delay(500);
      }
        
      //pause between rounds
      delay(1000);
        
      //reinitialize after each round
      rightAnswers = 0;
    }
      
    //displays light pattern at the end of a game
    for(light = LEDG; light <= LEDR; light++){
      digitalWrite(light, HIGH);
      delay(200);
      
      digitalWrite(light, LOW);
    }
  //prints final score on serial monitor
  result(roundCount, score);
  }
}

//checks if correct button was pressed
int check(int button, int light, int roundCount){
  
  //if no button was pressed
  int score = -1;
  
  //if correct button was pressed
  if(digitalRead(button) == HIGH){
    score = roundCount;
    
    //turns off light if button was pressed
    digitalWrite(light, LOW);
  }
  
  //if wrong button was pressed
  else if(digitalRead(PB_RED) == HIGH || digitalRead(PB_YELLOW) == HIGH || digitalRead(PB_BLUE) == HIGH || digitalRead(PB_GREEN) == HIGH){
    score = 0;
    
    digitalWrite(light, LOW);
  }
  return score;
}

//prints score on serial monitor
void result(int roundCount, int score){
  
  //prints after each prompt
  if(roundCount <= ROUNDS){
    Serial.print("Round ");
    Serial.print(roundCount);
    
    Serial.print(" --- Score: ");
    Serial.println(score);
  }
  
  //prints when game is terminated
  else if (roundCount == ROUNDS + 2){
    Serial.println("Button not pressed, gameover!");
  }
  
  //prints at the end of a game
  else if(roundCount == ROUNDS + 1){
    Serial.print("Game over! Score: ");
    Serial.println(score);
  }
}
