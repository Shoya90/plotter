#include <Servo.h>
Servo myservo;

#define pin1 4
#define pin2 5
#define pin3 6
#define pin4 7

#define pin5 8
#define pin6 9
#define pin7 10
#define pin8 11

#define TOP 3
#define RIGHT 2

#define delay_time  5
int pos ; 
int steps;
int chooser = 0;
int top_cl = 0;
int right_cl = 0;
int origin = 0;


void setup() {
  myservo.attach(12);
  Serial.begin(9600);
  
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);

  pinMode(pin5, OUTPUT);
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);
  pinMode(pin8, OUTPUT);

  pinMode(TOP, INPUT);
  pinMode(RIGHT, INPUT);
}

void loop() {

    if(origin == 0){
      findOrigin();
    }

}

//MOVE TO ORIGIN------------------------------------------------
void findOrigin(){

    right_cl = digitalRead(RIGHT);
    top_cl = digitalRead(TOP);
    
    if(right_cl == LOW && top_cl == LOW){
      moveRight();
      right_cl = digitalRead(RIGHT);
    }
    
    if(right_cl == HIGH && top_cl == LOW){
      Step_OFF();
      delay(delay_time);
      for(int i=0; i < 20; i++){
        moveLeft();
      }
    }
    
    if(right_cl == LOW && top_cl == LOW){

      moveUp();
      top_cl = digitalRead(TOP);

    }

    if(right_cl == LOW && top_cl == HIGH){
        Step_OFF();
        for(int i=0; i < 20; i++){
          moveDown();
        }
        origin = 1;
    }
}

//MOVE DOWN-----------------------------------------------------
void moveDown(){
 
    leftNegative(1);
    delay(delay_time);
    rightNegative(1);
    delay(delay_time);
  
}

//MOVE UP-----------------------------------------------------
void moveUp(){

    leftPositive(1);
    delay(delay_time);
    rightPositive(1);
    delay(delay_time);

}

//MOVE LEFT-----------------------------------------------------
void moveLeft(){

    leftPositive(1);
    delay(delay_time);
    rightNegative(1);
    delay(delay_time);
  
}

//MOVE RIGHT-----------------------------------------------------
void moveRight(){
  
    leftNegative(1);
    delay(delay_time);
    rightPositive(1);
    delay(delay_time);  
    
}

//SERVO CONTROL-------------------------------------------------
void penUp(){
  for(pos = 90; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(5);                       // waits 15ms for the servo to reach the position 
  } 
}
void penDown(){
  for(pos = 180; pos>=90; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(5);                       // waits 15ms for the servo to reach the position 
  }
}


//MOVE -----------------------------------------------------

//  void Bezier(p1,p2){
//    C = p1 + .39*abs(p2-p1)*n1
//    D = p1 - .39*abs(p2-p1)*n2
//    
//    for(t=0 ; t < 1 ; t+=.01){
//      px = p1*pow(1-t,3) + 3*C*t*pow(1-t,2) + 3*D*pow(t,2)*(1-t) + p2*pow(t,3);
//      py = 
//    }
//    
//  }

void moveTo(int x,int y){

  int b[2];
  b[0] = 0;
  int y_counter;
  
  if(abs(x)>=abs(y)){
    if(x>0){
      for(long a=1 ; a<=x ; a++){
        rightPositive(1);
        b[1] = (int)(a*abs(y)/x);
        y_counter = b[1]-b[0];
        while(y_counter>0){
          if(y<0) leftNegative(1);
          else leftPositive(1);
          y_counter--;
        }
        b[0] = b[1];
      }
    }
    else{
      x = abs(x);      
      for(long a=1 ; a<=x ; a++){
        rightNegative(1);
        b[1] = (int)(a*abs(y)/x);
        y_counter = b[1]-b[0];
        while(y_counter>0){ 
          if(y>0) leftPositive(1);
          else leftNegative(1);
          y_counter--;
        }
        b[0] = b[1];
      }
    }
  }
  else{
    if(y>0){
      for(long a=1 ; a<=y ; a++){
        b[1] = (int)(a*abs(x)/y);
        y_counter = b[1]-b[0];
        while(y_counter>0){ 
          if(x<0) rightNegative(1);
          else rightPositive(1);
          y_counter--;
        }
        leftPositive(1);
        b[0] = b[1];
      }
    }
    else{
      y = abs(y);
      for(long a=1 ; a<=y ; a++){
        b[1] = (int)(a*abs(x)/y);
        y_counter = b[1]-b[0];
        while(y_counter>0){ 
          if(x>0) rightPositive(1);
          else rightNegative(1);
          y_counter--;
        }
        leftNegative(1);
        b[0] = b[1];
      }
    }
  }
  
}




//HAIF CYCLEs-------------------------------------------------

void leftCircle(float degree){
  int count = (int) degree*4.055;
  
  while(count>0){
    Step_D();
    delay(delay_time);
    Step_DC();
    delay(delay_time);
  
    Step_C();
    delay(delay_time);
    Step_CB();
    delay(delay_time);
  
    Step_B();
    delay(delay_time);
    Step_BA();
    delay(delay_time);
  
    Step_A();
    delay(delay_time);
    Step_AD();
    delay(delay_time);
    count--;
  }


}
void forwardLine(int count){
  while(count>0){
    Step_AD_LINE();
    delay(delay_time);
    Step_A_LINE();
    delay(delay_time);
    
    Step_BA_LINE();
    delay(delay_time);
    Step_B_LINE();
    delay(delay_time);
    
    Step_CB_LINE();
    delay(delay_time);
    Step_C_LINE();
    delay(delay_time);
    
    Step_DC_LINE();
    delay(delay_time);
    Step_D_LINE();
    delay(delay_time);
    
    count--;
  }

}
void rightNegative(int count){
  while(count>0){
    Step_D_L();
    delay(delay_time);
    Step_DC_L();
    delay(delay_time);
  
    Step_C_L();
    delay(delay_time);
    Step_CB_L();
    delay(delay_time);
  
    Step_B_L();
    delay(delay_time);
    Step_BA_L();
    delay(delay_time);
  
    Step_A_L();
    delay(delay_time);
    Step_AD_L();
    delay(delay_time);
    count--;
  }


}
void leftPositive(int count){
  while(count>0){
    Step_D_R();
    delay(delay_time);
    Step_DC_R();
    delay(delay_time);
  
    Step_C_R();
    delay(delay_time);
    Step_CB_R();
    delay(delay_time);
  
    Step_B_R();
    delay(delay_time);
    Step_BA_R();
    delay(delay_time);
  
    Step_A_R();
    delay(delay_time);
    Step_AD_R();
    delay(delay_time);    
    count--;
  }


}

void rightCircle(float degree){
  int count = (int) degree*4.055;
  
  while(count>0){
    Step_AD();
    delay(delay_time);
    Step_A();
    delay(delay_time);
    
    Step_BA();
    delay(delay_time);
    Step_B();
    delay(delay_time);
    
  
    Step_CB();
    delay(delay_time);
    Step_C();
    delay(delay_time);
    
    Step_DC();
    delay(delay_time);
    Step_D();
    delay(delay_time);
    
    count--;
  }
}
void backwardLine(int count){
  while(count>0){
    Step_D_LINE();
    delay(delay_time);
    Step_DC_LINE();
    delay(delay_time);
  
    Step_C_LINE();
    delay(delay_time);
    Step_CB_LINE();
    delay(delay_time);
  
    Step_B_LINE();
    delay(delay_time);
    Step_BA_LINE();
    delay(delay_time);
  
    Step_A_LINE();
    delay(delay_time);
    Step_AD_LINE();
    delay(delay_time);
    count--;
  }


}
void rightPositive(int count){
  while(count>0){
    Step_AD_L();
    delay(delay_time);
    Step_A_L();
    delay(delay_time);
    
    Step_BA_L();
    delay(delay_time);
    Step_B_L();
    delay(delay_time);
    
  
    Step_CB_L();
    delay(delay_time);
    Step_C_L();
    delay(delay_time);
    
    Step_DC_L();
    delay(delay_time);
    Step_D_L();
    delay(delay_time);
    
    count--;
  }


}
void leftNegative(int count){
  while(count > 0){
    Step_AD_R();
    delay(delay_time);
    Step_A_R();
    delay(delay_time);
    
    Step_BA_R();
    delay(delay_time);
    Step_B_R();
    delay(delay_time);
    
  
    Step_CB_R();
    delay(delay_time);
    Step_C_R();
    delay(delay_time);
    
    Step_DC_R();
    delay(delay_time);
    Step_D_R();
    delay(delay_time);
    
    count--;
  }


}

//-------------------------------------------------------------



//-------------------------------------------------------------

//L-->avanti, R-->indietro
void Step_A(){
  digitalWrite(pin1, HIGH);
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
//L-->avanti, R-->avanti
void Step_A_LINE(){
  digitalWrite(pin1, HIGH);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, HIGH);//
}


void Step_A_L(){
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
}
void Step_A_R(){
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin6, LOW);//
  digitalWrite(pin7, LOW);//
  digitalWrite(pin8, LOW);//
}
void Step_AD(){
  digitalWrite(pin1, HIGH);
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, HIGH);
  digitalWrite(pin8, HIGH);//
}
void Step_AD_LINE(){
  digitalWrite(pin1, HIGH);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin4, HIGH);
  digitalWrite(pin8, HIGH);//
}
void Step_AD_L(){
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, HIGH);
}
void Step_AD_R(){
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin6, LOW);//
  digitalWrite(pin7, LOW);//
  digitalWrite(pin8, HIGH);//
}
void Step_B(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, HIGH);
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_B_LINE(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, HIGH);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_B_L(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
}
void Step_B_R(){
  digitalWrite(pin5, LOW);//
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin7, LOW);//
  digitalWrite(pin8, LOW);//
}
void Step_BA(){
  digitalWrite(pin1, HIGH);
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin2, HIGH);
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_BA_LINE(){
  digitalWrite(pin1, HIGH);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, HIGH);
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_BA_L(){
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
}
void Step_BA_R(){
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin7, LOW);//
  digitalWrite(pin8, LOW);//
}
void Step_C(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, HIGH);
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_C_LINE(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin3, HIGH);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_C_L(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, LOW);
}
void Step_C_R(){
  digitalWrite(pin5, LOW);//
  digitalWrite(pin6, LOW);//
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin8, LOW);//
}
void Step_CB(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, HIGH);
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin3, HIGH);
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_CB_LINE(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin2, HIGH);
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin3, HIGH);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_CB_L(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, LOW);
}
void Step_CB_R(){
  digitalWrite(pin5, LOW);//
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin8, LOW);//
}
void Step_D(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, HIGH);
  digitalWrite(pin8, HIGH);//
}
void Step_D_LINE(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, HIGH);
  digitalWrite(pin8, LOW);//
}
void Step_D_L(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, HIGH);
}
void Step_D_R(){
  digitalWrite(pin5, LOW);//
  digitalWrite(pin6, LOW);//
  digitalWrite(pin7, LOW);//
  digitalWrite(pin8, HIGH);//
}
void Step_DC(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, HIGH);
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin4, HIGH);
  digitalWrite(pin8, HIGH);//
}
void Step_DC_LINE(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, HIGH);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, HIGH);
  digitalWrite(pin8, HIGH);//
}
void Step_DC_L(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, HIGH);
}
void Step_DC_R(){
  digitalWrite(pin5, LOW);//
  digitalWrite(pin6, LOW);//
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin8, HIGH);//
}
void Step_OFF(){
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}







































