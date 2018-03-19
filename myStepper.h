#ifndef myStepper_h
#define myStepper_h

class myStepper {
public:
  // constructors:
  myStepper(int steps_per_rev, int pulse_pin, int dir_pin);


  // speed setter method:
  void setSpeed(long whatSpeed);

  // mover method:
  void step(int steps_to_move);

  void returnToStart();

private:

  void pulse();

  int direction;            // Direction of rotation
  unsigned long step_delay; // delay between steps, in ms, based on speed
  int steps_per_rev;      // total number of steps this motor can take
  int steps_from_start;  // counting steps from beginning
  int max_steps;        // maximum allowed steps from the start

  // motor pin numbers:
  int pulse_pin;
  int dir_pin;

  unsigned long last_step_time; // time stamp in us of when the last step was taken
};

#endif
