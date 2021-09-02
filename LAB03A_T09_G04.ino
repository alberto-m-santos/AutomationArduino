/////////////////////////////////////////////////
// Sistemas e Automação                        //  
// (C) FEUP up201708979@edu.fe.up.pt  //
/////////////////////////////////////////////////

#include "sa.h"
#include <TimerOne.h>


// Define 8 global timers
const int total_timers  = 8;
timer_t timer[total_timers];

// To be updated by read_inputs at start of cycle - do not touch
byte sens_in;
byte sens_out;

// To be updated by write_outputs at end of cycle - do not touch
byte sign_red;
byte sign_green;
byte barr_in;
byte barr_out;


// Input PINs =====> Rectify with your own pins if needed
const int pin_sens_in   = 8;  
const int pin_sens_out  = 9;

// OUTPUT PINs =====> Rectify with your own pins if needed
const int pin_green    = 5;
const int pin_red      = 4;
const int pin_barr_in  = 2;
const int pin_barr_out = 3;


// Global Variables, define additional global vars below
byte state0;
byte state1;
byte state2;
byte state3;
int  carro;
int cap = 4;        // CAPACIDADE DO PARQUE


// Run Once After Reset or PowerUp (to setup hardware, etc)
void setup() 
{                
  // Serial Port Speed
  Serial.begin(115200);    

  // Init message
  Serial.println("(C)FEUP S&A - start init");

  
  // Define Input and Output Pins
  pinMode(pin_green, OUTPUT);     
  pinMode(pin_red, OUTPUT);     
  pinMode(pin_barr_in, OUTPUT);     
  pinMode(pin_barr_out, OUTPUT);     
  pinMode(pin_sens_in, INPUT_PULLUP); 
  pinMode(pin_sens_out, INPUT_PULLUP); 
  
  digitalWrite(pin_green, HIGH);
  digitalWrite(pin_red, HIGH);
  digitalWrite(pin_barr_in, HIGH);
  digitalWrite(pin_barr_out, HIGH);

  delay(500);    // Leave POST for half a second

  // Setup Timers
  timer[0].p = 20; // 2 seconds
  start_timer(timer[0]);
  timer[1].p = 50; // 5 seconds
  start_timer(timer[1]);


  digitalWrite(pin_green, LOW);
  digitalWrite(pin_red, LOW);
  digitalWrite(pin_barr_in, LOW);
  digitalWrite(pin_barr_out, LOW);


  Serial.println("Init Done...");

  state0 = 1;
  state2 = 1;
  carro  = 0;

} // End of Setup


// Update timers at start of cycle - do not touch
void refresh_timers(void)
{
 byte i;
 for(i = 0; i < total_timers; i++)
   refresh_timer(timer[i]);
} // End of refresh_timers()


// Read from pins to positive “Image_Variables” - do not touch
void read_inputs(void)
{
  if (digitalRead(pin_sens_in )==LOW) sens_in  = 1; else sens_in  = 0;
  if (digitalRead(pin_sens_out)==LOW) sens_out = 1; else sens_out = 0;
} // End of read_inputs()



// Write to pins from “Image_Variables” - do not touch
void write_outputs(void)
{
  digitalWrite(pin_green, sign_green);
  digitalWrite(pin_red, sign_red);
  digitalWrite(pin_barr_in, barr_in);
  digitalWrite(pin_barr_out, barr_out);
} // End of write_inputs



// Global Variables for Shield and S&A stuff - do not touch
long previousMicros = 0; 

// Arduino Loop - do not touch
void loop() 
{
  unsigned long currentMicros = micros();
  if(currentMicros - previousMicros > 10000) {   // 10 ms

    previousMicros = currentMicros;

    refresh_timers();
    read_inputs();
  
    loop_10ms();
    
    write_outputs();
  }
} // End of Arduino loop



///////////////////////////////////////////////////
///////                                     ///////   
///////   Write Your Code below this line   ///////
///////                                     ///////   
///////////////////////////////////////////////////


// Loop where you should write your state machine code, etc
void loop_10ms(void)  
{  
  // Calculate Outputs
  sign_red   = (carro  == cap);   // CAP- CAPACIDADE DO PARQUE
  sign_green = (carro  <  cap);
  barr_in    = (state1 == 1);
  barr_out   = (state3 == 1);

//Alinea A
  // Debug – print states and variables, change as you wish!
  Serial.println( " St0:"    + ((String) state0) +" St1:"   + ((String) state1)    + " St2:"    + ((String) state2) +  " St3:"    + ((String) state3) + 
                  " Sens:" + ((String) sens_in  ) + ((String) sens_out ) +
                  " Sign:" + ((String) sign_green) + ((String) sign_red) +
                  " Barr:" + ((String) barr_in)   + ((String) barr_out));
   Serial.println(carro);
if((state0)&&(sens_in)&&carro<cap){     // ESTADOS DE ENTRADA- STAND BY,          CARRO> CAP, NAO PERMITE MAIS QUE A LOTAÇAO DO PARQUE
  state0=0;
  state1=1;
  carro+=1; // COUNT UP
}
if((state1)&&(!sens_in)){               //ESTADOS DE ENTRADA
  state1=0;
  state0=1;
}
if((state2)&&(sens_out)&&carro>0){      // ESTADO DE SAIDA- STAND BY,      CARRO>0, NAO PERMITE "CARROS" NEGATIVOS
  state2=0;
  state3=1;
  carro-=1; // COUNT DOWN
}
if((state3)&&(!sens_out)){            // ESTADO DE SAIDA
  state3=0;
  state2=1;
}
  // Debug – print states and variables, change as you wish!
  Serial.println( " St0:"    + ((String) state0) +" St1:"   + ((String) state1)    + " St2:"    + ((String) state2) +  " St3:"    + ((String) state3) + 
                  " Sens:" + ((String) sens_in  ) + ((String) sens_out ) +
                  " Sign:" + ((String) sign_green) + ((String) sign_red) +
                  " Barr:" + ((String) barr_in)   + ((String) barr_out));
   Serial.println(carro);
} // End of loop_10ms


