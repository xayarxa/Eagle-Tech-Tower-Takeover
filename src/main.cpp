#include "../include/main.h"

#define LEFT_MTR_PORT 20
#define RIGHT_MTR_PORT 1
#define STACK_MTR_PORT 15
#define INTAKE_MTR1_PORT 5
#define INTAKE_MTR2_PORT 6
#define VISION_PORT 10

#define STACK_MTR_POWER 100
#define INTAKE_MTR_POWER 100
#define LIFT_MOTOR_ROTATION 180

pros::Controller master (pros::E_CONTROLLER_MASTER);
pros::Motor left_mtr(LEFT_MTR_PORT);
pros::Motor right_mtr (RIGHT_MTR_PORT, true);
pros::Motor stack_mtr(STACK_MTR_PORT);
pros::Motor intake_mtr1(INTAKE_MTR1_PORT);
pros::Motor intake_mtr2(INTAKE_MTR2_PORT);
pros::Vision vision_sensor(VISION_PORT);

int power,turn,present,left,right;

// OUR FUNCTIONS
		void intake(short state)
		{
			switch(state)
			{
					case -1:
						intake_mtr1.move(-INTAKE_MTR_POWER);
						intake_mtr2.move(-INTAKE_MTR_POWER);
						break;

					case 0:
						intake_mtr1.move(0);
						intake_mtr2.move(0);
						break;

					case 1:
						intake_mtr1.move(INTAKE_MTR_POWER);
						intake_mtr2.move(INTAKE_MTR_POWER);
						break;

			}
			pros::delay(2);
		}

	void lift(bool reverse)
		{
      while(stack_mtr.get_position()<LIFT_MOTOR_ROTATION)
      {
        stack_mtr.move(reverse ? STACK_MTR_POWER:-STACK_MTR_POWER);
        pros::delay(3);
      }
		}
// OUR FUNCTIONS


void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "meegle mech robot brain");
	pros::lcd::register_btn1_cb(on_center_button);
	stack_mtr.set_gearing(pros::E_MOTOR_GEARSET_36);
}

void disabled() {}
void competition_initialize() {}



	/*
	intake(1);              //DONE
	go(forward, 2.5sec);
	intake(0);             //DONE
	go(back,2.5sec);
	turn(-90);
	go(forward,0.5sec);
	lift(false);
	intake(-1);								//DONE
	intake(0);                 //DONE
	lift(true);
	*/


void autonomous() {
	while(true)
	{
		pros::vision_object cube = vision_sensor.get_by_size(0);
		int power = 0;
		int turn = 0;

		if(!(cube.x_middle_coord<180 && cube.x_middle_coord>130) && cube.x_middle_coord > 0) // if there is not an object and if there is, not in the middle
		{
			turn = (cube.x_middle_coord-155) / 5;
			unsigned int present = 0;
			pros::lcd::print(6, "imma be turnin'");
		}

		else pros::lcd::print(6,"aint turnin for now");

		if((cube.height >= 30 || cube.width >=30) && (cube.height <= 200 || cube.width <=200) && cube.x_middle_coord > 0)
		{
		    power = 127; // if there is a cube(cube.x_middle_coord > 0) and if it is in the appropriate size, RUN INTO IT!!!!
		}

		else
		{
			if(present < 100) unsigned int present = pros::millis();
			if(pros::millis() > present + 2000) break;
		}

			left_mtr.move(power+turn);
			right_mtr.move(power-turn);
			pros::delay(5);
		}
	}



void opcontrol() {
	while (true) {
		int power = master.get_analog(ANALOG_LEFT_Y);
		int turn = master.get_analog(ANALOG_LEFT_X);
		//vision sensor start
		pros::vision_object cube = vision_sensor.get_by_size(0); // yesil dude
		pros::lcd::print(5, "%d", cube.x_middle_coord);

			//a function which makes the robot direct to the cube it sees
		while(master.get_digital(DIGITAL_X))
		{
			pros::vision_object cube = vision_sensor.get_by_size(0);
			power = 0;
			turn = 0;
			if(!(cube.x_middle_coord<180 && cube.x_middle_coord>130) && cube.x_middle_coord > 0) // if there is not an object and if there is, not in the middle
			{
				turn = (cube.x_middle_coord-155) / 5;
				pros::lcd::print(6, "imma be turnin'"); //3.5 for competition
			}
			else pros::lcd::print(6,"aint turnin for now");
			if((cube.height >= 30 || cube.width >=30) && (cube.height <= 200 || cube.width <=200) && cube.x_middle_coord > 0) power = 127; // if there is a cube(cube.x_middle_coord > 0) and if it is in the appropriate size, RUN INTO IT!!!!
			left_mtr.move(power+turn);
			right_mtr.move(power-turn);
			pros::delay(5);
		}

		while(master.get_digital(DIGITAL_L1))
		{
			stack_mtr.move(STACK_MTR_POWER);
			pros::delay(5);
		}

		while(master.get_digital(DIGITAL_L2))
	  {
  		stack_mtr.move(-STACK_MTR_POWER);
			pros::delay(5);
		}

		if(master.get_digital_new_press(DIGITAL_R1))
		{
			if(intake_mtr1.get_actual_velocity() > 50) intake(0);
			else intake(1);
		}

		if(master.get_digital_new_press(DIGITAL_R2))
		{
			if(intake_mtr1.get_actual_velocity() < -50) intake(0);
			else intake(-1);
		}

	  while(stack_mtr.get_position()<LIFT_MOTOR_ROTATION)
	   {
			 int lpwr=master.get_analog(ANALOG_RIGHT_Y);
	     stack_mtr.move(lpwr);
	   }

		int left = power + turn;
		int right = power - turn;
		left_mtr.move(left);
		right_mtr.move(right);
		pros::delay(20);
		}
	}
