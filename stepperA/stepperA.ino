#include <Servo.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <HttpClient.h>

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
#define NUM 401
int pos ;
boolean servo_check;
int steps;
int chooser = 0;
int top_cl = 0;
int right_cl = 0;
int origin = 0;
int a = 0;
YunServer server;
String command;
char row[NUM] ;
int len = 0;
int numRows = 0;
Process p;

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

  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Bridge.begin();
  delay(1000);
  digitalWrite(13, LOW);

  server.begin();
  servo_check = false;
  myservo.write(35);
  delay(1000);

  p.runShellCommand("killall node");
  p.runShellCommandAsynchronously("node /mnt/sda1/arduino/node/server.js");
}

void loop() {
  // main loop
  HttpClient client;

  if (origin != 3) {
    findOrigin();
    if (origin == 3) {
      for (int i = 0; i < 120; i++) {
        moveLeft();
      }
      client.get("http://localhost:8080/ready");
      Step_OFF();
    }
  }
  else {
    if (p.available() > 0 && len < NUM) {
      char buffer = p.read();
      Serial.print(buffer);
      row[len] = buffer;
      len++;
    } else {
      if (len > 0) {        
        int i = 0;
        while (i < len) {
          if (row[i] == '1') {
            Step_OFF();
            penDown();
            delay(1000);
          } else {
            Step_OFF();
            penUp();
            delay(1000);
          }
          for (int j = i + 1; j < len && row[i] == row[j]; j++) {
            if (numRows % 2 == 0) {
              moveLeft();
            } else {
              moveRight();
            }
            i++;
          }
          i++;
        }
        len = 0;
        numRows++;
        Serial.println(numRows);
        moveDown();
        client.get("http://localhost:8080/end");
      }
    }
  }
}

void process(YunClient client) {
  String command = client.readStringUntil('/');
  command.trim();        //kill whitespace
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);

  Serial.println(command);
}

//MOVE TO ORIGIN------------------------------------------------
void findOrigin() {

  right_cl = digitalRead(RIGHT);
  top_cl = digitalRead(TOP);

  if (right_cl == LOW && top_cl == LOW && (origin == 1 || origin == 0)) {
    moveRight();
    origin = 1;
    right_cl = digitalRead(RIGHT);
  }

  if (right_cl == HIGH && top_cl == LOW) {
    Step_OFF();
    delay(delay_time);
    for (int i = 0; i < 40; i++) {
      moveLeft();
    }
    origin = 2;
  }

  if (right_cl == LOW && top_cl == LOW && origin == 2) {
    moveUp();
    top_cl = digitalRead(TOP);
  }

  if (right_cl == LOW && top_cl == HIGH) {
    Step_OFF();
    for (int i = 0; i < 100; i++) {
      moveDown();
    }
    origin = 3;
  }
}

//MOVE DOWN-----------------------------------------------------
void moveDown() {

  leftNegative(1);
  delay(delay_time);
  rightNegative(1);
  delay(delay_time);

}

//MOVE UP-----------------------------------------------------
void moveUp() {

  leftPositive(1);
  delay(delay_time);
  rightPositive(1);
  delay(delay_time);

}

//MOVE LEFT-----------------------------------------------------
void moveLeft() {

  leftPositive(1);
  delay(delay_time);
  rightNegative(1);
  delay(delay_time);

}

//MOVE RIGHT-----------------------------------------------------
void moveRight() {

  leftNegative(1);
  delay(delay_time);
  rightPositive(1);
  delay(delay_time);

}

//SERVO CONTROL-------------------------------------------------
void penDown() {
  for (pos = 35; pos <= 90; pos +=5) // goes from 90 degrees to 180 degrees
  { // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}
void penUp() {
  for (pos = 90; pos >= 35; pos -=5) // goes from 180 degrees to 90 degrees
  {
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
}


//MOVE -----------------------------------------------------
void moveTo(int x, int y) {

  int b[2];
  b[0] = 0;
  int y_counter;

  if (abs(x) >= abs(y)) {
    if (x > 0) {
      for (long a = 1 ; a <= x ; a++) {
        rightPositive(1);
        b[1] = (int)(a * abs(y) / x);
        y_counter = b[1] - b[0];
        while (y_counter > 0) {
          if (y < 0) leftNegative(1);
          else leftPositive(1);
          y_counter--;
        }
        b[0] = b[1];
      }
    }
    else {
      x = abs(x);
      for (long a = 1 ; a <= x ; a++) {
        rightNegative(1);
        b[1] = (int)(a * abs(y) / x);
        y_counter = b[1] - b[0];
        while (y_counter > 0) {
          if (y > 0) leftPositive(1);
          else leftNegative(1);
          y_counter--;
        }
        b[0] = b[1];
      }
    }
  }
  else {
    if (y > 0) {
      for (long a = 1 ; a <= y ; a++) {
        b[1] = (int)(a * abs(x) / y);
        y_counter = b[1] - b[0];
        while (y_counter > 0) {
          if (x < 0) rightNegative(1);
          else rightPositive(1);
          y_counter--;
        }
        leftPositive(1);
        b[0] = b[1];
      }
    }
    else {
      y = abs(y);
      for (long a = 1 ; a <= y ; a++) {
        b[1] = (int)(a * abs(x) / y);
        y_counter = b[1] - b[0];
        while (y_counter > 0) {
          if (x > 0) rightPositive(1);
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

void leftCircle(float degree) {
  int count = (int) degree * 4.055;

  while (count > 0) {
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
void forwardLine(int count) {
  while (count > 0) {
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
void rightNegative(int count) {
  while (count > 0) {
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
void leftPositive(int count) {
  while (count > 0) {
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

void rightCircle(float degree) {
  int count = (int) degree * 4.055;

  while (count > 0) {
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
void backwardLine(int count) {
  while (count > 0) {
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
void rightPositive(int count) {
  while (count > 0) {
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
void leftNegative(int count) {
  while (count > 0) {
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
void Step_A() {
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
void Step_A_LINE() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, HIGH);//
}


void Step_A_L() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
}
void Step_A_R() {
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin6, LOW);//
  digitalWrite(pin7, LOW);//
  digitalWrite(pin8, LOW);//
}
void Step_AD() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, HIGH);
  digitalWrite(pin8, HIGH);//
}
void Step_AD_LINE() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin4, HIGH);
  digitalWrite(pin8, HIGH);//
}
void Step_AD_L() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, HIGH);
}
void Step_AD_R() {
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin6, LOW);//
  digitalWrite(pin7, LOW);//
  digitalWrite(pin8, HIGH);//
}
void Step_B() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, HIGH);
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_B_LINE() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, HIGH);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_B_L() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
}
void Step_B_R() {
  digitalWrite(pin5, LOW);//
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin7, LOW);//
  digitalWrite(pin8, LOW);//
}
void Step_BA() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin2, HIGH);
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_BA_LINE() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, HIGH);
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_BA_L() {
  digitalWrite(pin1, HIGH);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW);
}
void Step_BA_R() {
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin7, LOW);//
  digitalWrite(pin8, LOW);//
}
void Step_C() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, HIGH);
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_C_LINE() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin3, HIGH);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_C_L() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, LOW);
}
void Step_C_R() {
  digitalWrite(pin5, LOW);//
  digitalWrite(pin6, LOW);//
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin8, LOW);//
}
void Step_CB() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, HIGH);
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin3, HIGH);
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_CB_LINE() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin2, HIGH);
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin3, HIGH);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}
void Step_CB_L() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, HIGH);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, LOW);
}
void Step_CB_R() {
  digitalWrite(pin5, LOW);//
  digitalWrite(pin6, HIGH);//
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin8, LOW);//
}
void Step_D() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, HIGH);
  digitalWrite(pin8, HIGH);//
}
void Step_D_LINE() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, HIGH);
  digitalWrite(pin8, LOW);//
}
void Step_D_L() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, HIGH);
}
void Step_D_R() {
  digitalWrite(pin5, LOW);//
  digitalWrite(pin6, LOW);//
  digitalWrite(pin7, LOW);//
  digitalWrite(pin8, HIGH);//
}
void Step_DC() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, HIGH);
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin4, HIGH);
  digitalWrite(pin8, HIGH);//
}
void Step_DC_LINE() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, HIGH);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, HIGH);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, HIGH);
  digitalWrite(pin8, HIGH);//
}
void Step_DC_L() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, HIGH);
  digitalWrite(pin4, HIGH);
}
void Step_DC_R() {
  digitalWrite(pin5, LOW);//
  digitalWrite(pin6, LOW);//
  digitalWrite(pin7, HIGH);//
  digitalWrite(pin8, HIGH);//
}
void Step_OFF() {
  digitalWrite(pin1, LOW);
  digitalWrite(pin5, LOW);//
  digitalWrite(pin2, LOW);
  digitalWrite(pin6, LOW);//
  digitalWrite(pin3, LOW);
  digitalWrite(pin7, LOW);//
  digitalWrite(pin4, LOW);
  digitalWrite(pin8, LOW);//
}







































