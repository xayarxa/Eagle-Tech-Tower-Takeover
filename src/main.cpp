#include "../include/main.h"

#define LEFT_MTR_PORT 2
#define RIGHT_MTR_PORT 9
#define STACK_MTR_PORT 20
#define INTAKE_MTR1_PORT 1
#define INTAKE_MTR2_PORT 10
#define INTAKE_ARM1_PORT 3
#define INTAKE_ARM2_PORT 8
#define VISION_PORT 15
#define TEST_MOTOR_PORT 99

#define STACK_MTR_POWER 30 //30 actually
#define INTAKE_MTR_POWER 127
#define STACK_MOTOR_ROTATION_FORWARD 1850 // 1850 this magic number is the encoder turn unit to turn 70 degrees (it's just an assumption. 164.3 * 12)
#define BACKLASH 164.3
#define CORRECTED_STACK_MOTOR_ROTATION_FORWARD STACK_MOTOR_ROTATION_FORWARD-BACKLASH
#define ENCODER_TURN_PER_DEGREE 16.2222
#define AUTONOMOUS_RIGHT false

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor left_mtr(LEFT_MTR_PORT);
pros::Motor right_mtr(RIGHT_MTR_PORT, true);
pros::Motor stack_mtr(STACK_MTR_PORT);
pros::Motor intake_mtr1(INTAKE_MTR1_PORT);
pros::Motor intake_mtr2(INTAKE_MTR2_PORT,true);
pros::Motor arm_mtr1(INTAKE_ARM1_PORT,true);
pros::Motor arm_mtr2(INTAKE_ARM2_PORT);
pros::Motor test_mtr(TEST_MOTOR_PORT);
// pros::Vision vision_sensor(VISION_PORT);

int power, turn, present, left, right, arm_power;
short stack_state = -1;

// one full turn of a motor is 900 or -900 units(?)
// perimeter of a wheel is 4pi inch
// distance from the start to the end of the last cube is 51.6 inch ==> 3700 encoder turn units
// to turn 90 degrees with a pivot point, turn the left/right motor 1460 encoder turn units

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

				case -2:
					intake_mtr1.move(-40);
					intake_mtr2.move(-40);
					break;
		}
		pros::delay(2);
	}

void stack_lift(int state)
	{
		switch(state)
		{
				case 1:
					stack_mtr.move_absolute(CORRECTED_STACK_MOTOR_ROTATION_FORWARD, STACK_MTR_POWER);
					stack_state = 1;
					break;

				case -1:
					stack_mtr.move_absolute(0, STACK_MTR_POWER);
					stack_state = -1;
					break;

				default:
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
	arm_mtr1.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	arm_mtr2.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	pros::lcd::set_text(1, "meegle mech robot brain");
	stack_mtr.set_gearing(pros::E_MOTOR_GEARSET_36);
}

void disabled() {}
void competition_initialize() {}

void autonomous() {
	arm_mtr1.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	arm_mtr2.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	//expanding the stack mechanism expansion
	// left_mtr.move_voltage(127);
	// right_mtr.move_voltage(127);
	// pros::delay(150);
	// left_mtr.move_voltage(0);
	// right_mtr.move_voltage(0);
	// stack_lift((void*)1);
	// intake(-1);
	// pros::delay(2050);
	// intake(0);
	// stack_lift((void*)-1);
	//expanding the stack mechanism expansion
	go(500,127);
	pros::delay(500);
	go(-500,127);
	pros::delay(500);
	for(int i = 0; i<2; i++)
	{
	stack_mtr.move_absolute(STACK_MOTOR_ROTATION_FORWARD, 127);
	pros::delay(450);
	stack_mtr.move_absolute(0, 127);
	pros::delay(400);
	}

	arm_mtr1.move_relative(600, 127);
	arm_mtr2.move_relative(600, 127);
	pros::delay(150);
	go(3000, 120);
	pros::delay(350);
	stack_mtr.move_absolute(0, 127);
	intake(1);
	pros::delay(2000);
	go(-2200, -127);
	pros::delay(2100);
	turn_fixed(AUTONOMOUS_RIGHT ? 90 : -90, 127);
	arm_mtr1.move_relative(200, 127);
	arm_mtr2.move_relative(200, 127);
	pros::delay(1000);
	go(2000, 127);
	pros::delay(1500);
	stack_lift(1);
	intake(0);
	pros::delay(1700);
	intake(-2);
	pros::delay(200);
	go(-900,-40);
	pros::delay(2000);
	intake(0);
	}


void opcontrol()
{
	arm_mtr1.set_zero_position(arm_mtr1.get_position()-400);
	arm_mtr2.set_zero_position(arm_mtr2.get_position()-400);
	arm_mtr1.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	arm_mtr2.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	while (true)
	{
		int power = master.get_analog(ANALOG_LEFT_Y);
		int turn = master.get_analog(ANALOG_LEFT_X);
		int arm_power = master.get_analog(ANALOG_RIGHT_Y);
		int arm1_power = arm_power-10;
		if(arm1_power == -10) arm1_power = 0;
		//vision sensor start
		// pros::vision_object cube = vision_sensor.get_by_size(0); // yesil dude
		// pros::lcd::print(5, "%d", cube.x_middle_coord);

		//a function which makes the robot chase after the cube it sees

		// while(master.get_digital(DIGITAL_X))
		// {
		// 	pros::vision_object cube = vision_sensor.get_by_size(0);
		// 	power = 0;
		// 	turn = 0;
		// 	if(!(cube.x_middle_coord<180 && cube.x_middle_coord>130) && cube.x_middle_coord > 0) // if there is not an object and if there is, not in the middle
		// 	{
		// 		turn = (cube.x_middle_coord-155) / 5;
		// 		pros::lcd::print(6, "imma be turnin'"); //3.5 for competition
		// 	}
		// 	else pros::lcd::print(6,"aint turnin for now");
		// 	if((cube.height >= 30 || cube.width >=30) && (cube.height <= 200 || cube.width <=200) && cube.x_middle_coord > 0) power = 80; // if there is a cube(cube.x_middle_coord > 0) and if it is in the appropriate size, RUN INTO IT!!!!
		// 	left_mtr.move(power+turn);
		// 	right_mtr.move(power-turn);
		// 	intake(1);
		// 	pros::delay(5);
		// }

		if(master.get_digital_new_press(DIGITAL_L1))
		{
			if(stack_state == -1)
			{
				stack_lift(1);
			}
			else
			{
				stack_lift(-1);
				}
		}
		if(master.get_digital_new_press(DIGITAL_L2))
		{
			if(intake_mtr1.get_actual_velocity() < -10) intake(0);
			else
			{
				intake_mtr1.move(-40);
				intake_mtr2.move(-40);
			}
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
		if((arm_mtr2.get_position() < -20) && (arm_power < 0)) arm_power = 0; // don't allow arms to crash the ground
		arm_mtr1.move(arm_power*1.095);
		arm_mtr2.move(arm_power);
		}
		pros::delay(5);
	}
