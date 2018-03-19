#include "Arduino.h"
#include "myStepper.h"

myStepper::myStepper(int steps_per_rev, int pulse_pin, int dir_pin) {

  this->direction = 0;
  this->steps_per_rev = steps_per_rev;
  this->pulse_pin = pulse_pin;
  this->dir_pin = dir_pin;
  this->max_steps = 300;

  pinMode(this->pulse_pin, OUTPUT);
  pinMode(this->dir_pin, OUTPUT);

  this->step_delay = 60L * 1000L * 1000L / this->steps_per_rev / 180;  // default speed of 3 revs per second

  digitalWrite(this->dir_pin, LOW);
}


void myStepper::setSpeed(long whatSpeed) {
  this->step_delay = 60L * 1000L * 1000L / this->steps_per_rev / whatSpeed;
}


void myStepper::step(int steps_to_move) {
  int steps_left = abs(steps_to_move);  // how many steps to take

  // determine direction based on whether steps_to_mode is + or -:
  if (steps_to_move > 0) { this->direction = 1; }
  if (steps_to_move < 0) { this->direction = 0; }




  // decrement the number of steps, moving one step each time:
  while (steps_left > 0) {
    unsigned long now = micros();
    // move only if the appropriate delay has passed:
    if (now - this->last_step_time >= this->step_delay) {
      // get the timeStamp of when you stepped:
      this->last_step_time = now + 1000;
      // depending on direction take a step:
      if (this->direction == 1) {
        if(this->steps_from_start + 1 >= this->max_steps) { break; } // cancel the movement

        digitalWrite(this->dir_pin, HIGH);
        pulse();
        this->steps_from_start++;
      }
    else {
        if(this->steps_from_start - 1 < 0) { break; } // cancel the movement

        digitalWrite(this->dir_pin, LOW);
        pulse();
        this->steps_from_start--;
    }
    // decrement the steps left:
    steps_left--;
    }
  }
}

void myStepper::pulse() {
  digitalWrite(this->pulse_pin, HIGH);
  delayMicroseconds(500);
  digitalWrite(this->pulse_pin, LOW);
  delayMicroseconds(500);
}

void myStepper::returnToStart() {
  step(-1*this->steps_from_start);
  this->steps_from_start = 0;

}
