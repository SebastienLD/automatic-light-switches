#include <Servo.h>
#include <IRremote.h>
#include <TimeLib.h>

Servo leftLightServo;
Servo rightLightServo;

#define REMOTE_TOP_LEFT_BUTTON 69
#define REMOTE_TOP_MIDDLE_BUTTON 70
#define REMOTE_TOP_RIGHT_BUTTON 71

int left_pos = 0;  
int right_pos = 0;
time_t last_change_time_left = now();
time_t last_change_time_right = now();

bool left_light_on = 0;
bool right_light_on = 0;

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup(){
  setTime(22, 34, 0, 11, 1, 2023); // hard coding time of flash
  leftLightServo.attach(10);
  rightLightServo.attach(9);
  Serial.begin(9600);
  irrecv.enableIRIn();
}

bool has_been_long_enough_left() {
  int MS_BETWEEN_CHANGES = 3000;
  if (last_change_time_left - now() < MS_BETWEEN_CHANGES) {
    return false;
  }
  return true;
}

bool has_been_long_enough_right() {
  int MS_BETWEEN_CHANGES = 3000;
  if (last_change_time_right - now() < MS_BETWEEN_CHANGES) {
    return false;
  }
  return true;
}

void turn_off_right() {  
  if (!has_been_long_enough_right()) return;
  last_change_time_right = now();
  right_light_on = 0;
  for (right_pos = 20; right_pos <= 110; right_pos += 1) { // goes from 0 degrees to DEGREES_TO_TURN degrees
    rightLightServo.write(right_pos);              // tell servo to go to position in variable 'pos'
    delay(1);                       // waits 15ms for the servo to reach the position
  }
}

void turn_on_right() {
  if (!has_been_long_enough_right()) return;
  last_change_time_right = now();
  right_light_on = 1;
  for (right_pos = 110; right_pos >= 20; right_pos -= 1) { // goes from 0 degrees to DEGREES_TO_TURN degrees
    rightLightServo.write(right_pos);              // tell servo to go to position in variable 'pos'
    delay(1);                       // waits 15ms for the servo to reach the position
  }
}

void turn_on_left() {
  if (!has_been_long_enough_left()) return;
  last_change_time_left = now();
  left_light_on = 1;
  for (left_pos = 20; left_pos <= 110; left_pos += 1) { // goes from 0 degrees to DEGREES_TO_TURN degrees
    leftLightServo.write(left_pos);              // tell servo to go to position in variable 'pos'
    delay(1);                       // waits 15ms for the servo to reach the position
  }
}

void turn_off_left() {
  if (!has_been_long_enough_left()) return;
  last_change_time_left = now();
  left_light_on = 0;
    for (left_pos = 110; left_pos >= 20; left_pos -= 1) { // goes from 0 degrees to DEGREES_TO_TURN degrees
      leftLightServo.write(left_pos);              // tell servo to go to position in variable 'pos'
      delay(1);                       // waits 15ms for the servo to reach the position
    }
}

void turn_both_on() {
  turn_on_left();
  turn_on_right();
}

void turn_both_off() {
  turn_off_left();
  turn_off_right();
}

void handle_right() {
  if (right_light_on == 0) {
    turn_on_right();
  } else {
    turn_off_right();
  }
}

void handle_left() {
  if (left_light_on == 0) {
    turn_on_left();
  } else {
    turn_off_left();
  }
}

void handle_both() {
  if (left_light_on == 0 && right_light_on == 0) {
    turn_both_on();
  } else {
    turn_both_off();
  }
}

void loop() {
  // commenting out automatic turn on / off
  // time_t curr_time = now();
  // if (hour(curr_time) == 23 && minute(curr_time) == 30 && second(curr_time) == 0) {
  //   turn_both_off();
  // }
  // else if (hour(curr_time) == 7 && minute(curr_time) == 30 && second(curr_time) == 0) {
  //   turn_both_on();
  // }
  
  if (IrReceiver.decode()) {
    IrReceiver.resume();
    int command = IrReceiver.decodedIRData.command;
    switch (command) {
      case REMOTE_TOP_LEFT_BUTTON: {
        handle_both();
        break;
      }
      case REMOTE_TOP_MIDDLE_BUTTON: {
        handle_left();
        break;
      }
      case REMOTE_TOP_RIGHT_BUTTON: {
        handle_right();
        break;
      }
      default: {
        Serial.println("Didn't recognize that button!");
      }
    }
  }
}