#include "../include/main.h"

//TO
/*
	right arm

*/

#define LEFT_MTR_PORT 20
#define RIGHT_MTR_PORT 17
#define STACK_MTR_PORT 13
#define INTAKE_MTR1_PORT 15
#define INTAKE_MTR2_PORT 16
#define INTAKE_ARM1_PORT 11
#define INTAKE_ARM2_PORT 12
#define VISION_PORT 10
#define TEST_MOTOR_PORT 8

#define STACK_MTR_POWER 10
#define INTAKE_MTR_POWER 60
#define STACK_MOTOR_ROTATION_FORWARD 1971.6 // this magic number is the encoder turn unit to turn 70 degrees (it's just an assumption. 164.3 * 12 lol)
#define BACKLASH 164.3
#define CORRECTED_STACK_MOTOR_ROTATION_FORWARD STACK_MOTOR_ROTATION_FORWARD-BACKLASH
#define ENCODER_TURN_PER_DEGREE 16.2222

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor left_mtr(LEFT_MTR_PORT);
pros::Motor right_mtr(RIGHT_MTR_PORT, true);
pros::Motor stack_mtr(STACK_MTR_PORT);
pros::Motor intake_mtr1(INTAKE_MTR1_PORT);
pros::Motor intake_mtr2(INTAKE_MTR2_PORT,true);
pros::Motor arm_mtr1(INTAKE_ARM1_PORT);
pros::Motor arm_mtr2(INTAKE_ARM2_PORT);
pros::Motor test_mtr(TEST_MOTOR_PORT);
pros::Vision vision_sensor(VISION_PORT);
int power, turn, present, left, right;
short stack_state = -1;

// one full turn of a motor is 900 or -900 units(?)
// perimeter of a wheel is 4pi inch
// distance from the start to the end of the last cube is 51.6 inch ==> 3700 encoder turn units
// to turn 90 degrees with a fixed to a pivot point, turn the left/right motor 1460 encoder turn units

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

void stack_lift(void* state)
	{
		switch((int)state)
		{
				case 1:
					stack_mtr.move_absolute(CORRECTED_STACK_MOTOR_ROTATION_FORWARD, 30);
					stack_state = 1;
					break;

				case -1:
					stack_mtr.move_absolute(0, 30);
					stack_state = -1;
					break;
			}

		}

void go(int rotation, int velocity)
{
		left_mtr.move_relative(rotation, velocity);
		right_mtr.move_relative(rotation, velocity);
		pros::delay(5);
}

void turn_fixed(int degrees, int velocity)
{
	if(degrees > 0) left_mtr.move_relative(ENCODER_TURN_PER_DEGREE * degrees, velocity);
	else right_mtr.move_relative(ENCODER_TURN_PER_DEGREE * degrees, velocity);
}
// OUR FUNCTIONS

void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "meegle mech robot brain");
	stack_mtr.set_gearing(pros::E_MOTOR_GEARSET_36);
}

void disabled() {}
void competition_initialize() {}

void autonomous() {
	intake(1);
	go(3700, 80);
	intake(0);
	go(0, -60);
	turn_fixed(-90, 50);
	go(-3700, 60);
	stack_lift((void*)1);
	intake(-1);
	intake(0);
	stack_lift((void*)-1);
	}

void opcontrol() {
	pros::Task stack_lift_task(stack_lift,(void*)-1, "stacklift down");
	while (true) {
		int power = master.get_analog(ANALOG_LEFT_Y);
		int turn = master.get_analog(ANALOG_LEFT_X);
		int arm_power = master.get_analog(ANALOG_RIGHT_Y);
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

		if(master.get_digital_new_press(DIGITAL_L1))
		{
			if(stack_state == -1)
			{
				stack_lift_task.remove();
				pros::Task stack_lift_task (stack_lift,(void*)1, "stacklift up");
			}
			else {
				stack_lift_task.remove();
				pros::Task stack_lift_task (stack_lift,(void*)-1, "stacklift down");}
				}

		if(master.get_digital_new_press(DIGITAL_R1))
		{
			if(intake_mtr1.get_actual_velocity() > 10) intake(0);
			else intake(1);
		}

		if(master.get_digital_new_press(DIGITAL_R2))
		{
			if(intake_mtr1.get_actual_velocity() < -10) intake(0);
			else intake(-1);
		}

		int left = power + turn;
		int right = power - turn;
		left_mtr.move(left);
		right_mtr.move(right);
		arm_mtr1.move(arm_power);
		arm_mtr2.move(arm_power);
		}
		pros::delay(5);
	}
