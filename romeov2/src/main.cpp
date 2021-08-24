#include <Arduino.h>

#include <Wire.h>

#define LED_PIN 13

char msgs[5][15] = {
  "Right Key OK ",
  "Up Key OK    ",
  "Down Key OK  ",
  "Left Key OK  ",
  "Select Key OK" };
char start_msg[15] = {
  "Start loop "};
int  adc_key_val[5] ={
  30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;


// Convert ADC value to key number
int get_key(unsigned int input)
{
  int k;
  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {
      return k;
    }
  }
  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed
  return k;
}


// void setup() {
//   pinMode(LED_PIN, OUTPUT);  //we'll use the debug LED to output a heartbeat
//   Serial.begin(9600);

//   /* Print that we made it here */
//   Serial.println(start_msg);
// }

// void loop()
// {
//   adc_key_in = analogRead(0);    // read the value from the sensor
//   digitalWrite(LED_PIN, LOW);
//   /* get the key */
//   key = get_key(adc_key_in);    // convert into key press
//   if (key != oldkey) {   // if keypress is detected
//     delay(50);      // wait for debounce time
//     adc_key_in = analogRead(0);    // read the value from the sensor
//     key = get_key(adc_key_in);    // convert into key press
//     if (key != oldkey) {
//       oldkey = key;
//       if (key >=0){
//         Serial.println(adc_key_in, DEC);
//         Serial.println(msgs[key]);
//       }
//     }
//   }
//   digitalWrite(LED_PIN, HIGH);
// }



// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


// #include <Wire.h>

// // function that executes whenever data is received from master
// // this function is registered as an event, see setup()
// void receiveEvent(int howMany)
// {
//   digitalWrite(LED_PIN, HIGH);
//   while(1 < Wire.available()) // loop through all but the last
//   {
//     char c = Wire.read(); // receive byte as a character
//     Serial.print(c);         // print the character
//   }
//   int x = Wire.read();    // receive byte as an integer
//   Serial.println(x);         // print the integer
//   digitalWrite(LED_PIN, LOW);
// }


// void setup()
// {
//   Wire.begin(0x60);                // join i2c bus with address #4
//   Wire.onReceive(receiveEvent); // register event
//   Serial.begin(9600);           // start serial for output
// }

// void loop()
// {
//   delay(100);
// }


//Standard PWM DC control
int E1 = 5;     //M1 Speed Control
int E2 = 6;     //M2 Speed Control
int M1 = 4;    //M1 Direction Control
int M2 = 7;    //M1 Direction Control

///For previous Romeo, please use these pins.
//int E1 = 6;     //M1 Speed Control
//int E2 = 9;     //M2 Speed Control
//int M1 = 7;    //M1 Direction Control
//int M2 = 8;    //M1 Direction Control


void stop(void)                    //Stop
{
  Serial.println("stop");
  digitalWrite(E1,LOW);
  digitalWrite(E2,LOW);
}
void advance(char a,char b)          //Move forward
{
  Serial.println("advance");
  analogWrite (E1,a);      //PWM Speed Control
  digitalWrite(M1,HIGH);
  analogWrite (E2,b);
  digitalWrite(M2,HIGH);
}
void back_off (char a,char b)          //Move backward
{
  Serial.println("back_off");
  analogWrite (E1,a);
  digitalWrite(M1,LOW);
  analogWrite (E2,b);
  digitalWrite(M2,LOW);
}
void turn_L (char a,char b)             //Turn Left
{
  Serial.println("turn_L");
  analogWrite (E1,a);
  digitalWrite(M1,LOW);
  analogWrite (E2,b);
  digitalWrite(M2,HIGH);
}
void turn_R (char a,char b)             //Turn Right
{
  Serial.println("turn_R");
  analogWrite (E1,a);
  digitalWrite(M1,HIGH);
  analogWrite (E2,b);
  digitalWrite(M2,LOW);
}
void setup(void)
{
  pinMode(13, OUTPUT);
  int i;
  for(i=4;i<=7;i++)
    pinMode(i, OUTPUT);
  Serial.begin(19200);      //Set Baud Rate
  Serial.println("Run keyboard control");

  Serial1.begin(115200);
  Serial.println("Serial 1 started");
}
void loop(void)
{
  // if(Serial.available()){
  //   char val = Serial.read();
  //   if(val != -1)
  //   {
  //     switch(val)
  //     {
  //     case 'w'://Move Forward
  //       advance (255,255);   //move forward in max speed
  //       break;
  //     case 's'://Move Backward
  //       back_off (255,255);   //move back in max speed
  //       break;
  //     case 'a'://Turn Left
  //       turn_L (100,100);
  //       break;
  //     case 'd'://Turn Right
  //       turn_R (100,100);
  //       break;
  //     case 'z':
  //       Serial.println("Hello");
  //       break;
  //     case 'x':
  //       stop();
  //       break;
  //     }
  //   }
  //   else stop();
  // }

  if (Serial1.available())  {
    Serial.write(Serial1.read());//send what has been received
    Serial.println();   //print line feed character
  }

  adc_key_in = analogRead(0);    // read the value from the sensor
  digitalWrite(13, HIGH);
  /* get the key */
  key = get_key(adc_key_in);    // convert into key press
  if (key != oldkey) {   // if keypress is detected
    delay(50);      // wait for debounce time
    adc_key_in = analogRead(0);    // read the value from the sensor
    key = get_key(adc_key_in);    // convert into key press
    if (key != oldkey) {
      oldkey = key;
      if (key >=0){
        // Serial.println(adc_key_in, DEC);
        Serial.println(msgs[key]);
        Serial.println(key);

        switch (key) {
          case 0: // Right
                    turn_R (100,100);
                    break;
          case 2: // Down
                  back_off (255,255);   //move forward in max speed
                    break;
          case 1: // up0000
                 advance (255,255);   //move forward in max speed
                    break;
           case 3: // left
                 turn_L (100,100);   //move forward in max speed
                    break;
            case 4:  // select
              stop();
              break;
        }
      }
    }
  }
  digitalWrite(13, LOW);
}
