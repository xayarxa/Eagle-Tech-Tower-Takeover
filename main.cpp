
#include "../include/main.h"

//mavi sağ

#define LEFT_MTR_PORT 2
#define RIGHT_MTR_PORT 9
#define STACK_MTR_PORT 19
#define STACK_MTR2_PORT 18
#define INTAKE_MTR1_PORT 4
#define INTAKE_MTR2_PORT 10
#define INTAKE_ARM1_PORT 3
#define INTAKE_ARM2_PORT 8
#define VISION_PORT 15
#define TEST_MOTOR_PORT 99

#define STACK_MTR_VELOCITY 35 //35 actually
#define INTAKE_MTR_POWER 127
#define STACK_MOTOR_ROTATION_FORWARD 1530 // this magic number is the encoder turn unit to turn 70 degrees (it's just an assumption)
#define BACKLASH 164.3
#define CORRECTED_STACK_MOTOR_ROTATION_FORWARD STACK_MOTOR_ROTATION_FORWARD-BACKLASH
#define ENCODER_TURN_PER_DEGREE 16.2222
#define FILE_PATH "/usd/autonomous_recorder3.txt"

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor left_mtr(LEFT_MTR_PORT);
pros::Motor right_mtr(RIGHT_MTR_PORT, true);
pros::Motor stack_mtr(STACK_MTR_PORT);
pros::Motor stack_mtr2(STACK_MTR2_PORT, true);
pros::Motor intake_mtr1(INTAKE_MTR1_PORT);
pros::Motor intake_mtr2(INTAKE_MTR2_PORT,true);
pros::Motor arm_mtr1(INTAKE_ARM1_PORT,true);
pros::Motor arm_mtr2(INTAKE_ARM2_PORT);
pros::Motor test_mtr(TEST_MOTOR_PORT);
// pros::Vision vision_sensor(VISION_PORT);
int power, turn, left, right, arm_power;
short stack_state = -1;
FILE* autonomous_recorder;
bool file_flag = false;
bool arm_pressed_left = false;
bool arm_pressed_right = false;

float left_mtr_auto, right_mtr_auto, stack_mtr_auto, intake_mtr_auto, arm_mtr_auto = 0; //old technique
float left_mtr_position, right_mtr_position;

int stack_buttons, IS_PRESSED_DIGITAL_L1, IS_PRESSED_DIGITAL_L2, IS_PRESSED_DIGITAL_R1, IS_PRESSED_DIGITAL_R2;


// one full turn of a motor is 900 or -900 units(?)
// perimeter of a wheel is 4pi inch
// distance from the start to the end of the last cube is 51.6 inch ==> 3700 encoder turn units
// to turn 90 degrees with a pivot point, turn the left/right motor 1460 encoder turn units

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
					intake_mtr1.move(-60);
					intake_mtr2.move(-60);
					break;
		}
	}

void stack_lift(int state, int velocity = STACK_MTR_VELOCITY)
	{
		switch(state)
		{
				case 1:
					stack_mtr.move_absolute(CORRECTED_STACK_MOTOR_ROTATION_FORWARD, velocity);
					stack_mtr2.move_absolute(CORRECTED_STACK_MOTOR_ROTATION_FORWARD, velocity);
					stack_state = 1;
					break;

				case -1:
					stack_mtr.move_absolute(15, velocity); //15 because of some concerns such as:
					stack_mtr2.move_absolute(15, velocity); //if the stack mechanism is stuck and can't go to position 0, it would force and waste energy to go to position 0
					stack_state = -1;
					break;

				case 0:
					stack_mtr.move_absolute((CORRECTED_STACK_MOTOR_ROTATION_FORWARD)/3, velocity);
					stack_mtr2.move_absolute((CORRECTED_STACK_MOTOR_ROTATION_FORWARD)/3, velocity);
					stack_state = 1;
					break;

				default:
					break;
			}

		}

void go(int rotation, int velocity = 127)
{
		left_mtr.move_relative(rotation, velocity);
		right_mtr.move_relative(rotation, velocity);
}

void turn_fixed(int degrees, int velocity = 127)
{
	if(degrees > 0)
	{
	left_mtr.move_relative(ENCODER_TURN_PER_DEGREE * degrees, velocity);
	right_mtr.move_relative(ENCODER_TURN_PER_DEGREE * -degrees, velocity);
	}
	else
	{
	right_mtr.move_relative(ENCODER_TURN_PER_DEGREE * -degrees, velocity);
	left_mtr.move_relative(ENCODER_TURN_PER_DEGREE * degrees, velocity);
	}
}

void autonomous_stack()
{
	stack_lift(1);
	pros::delay(1900); // mavi sol / kırmızı sağda 1900
	intake(1);
	pros::delay(400); // mavi sol / kırmızı sağda 350
	intake(-2);
	pros::delay(700); // mavi sol / kırmızı sağda 400
	go(-900,-30); // mavi sol / kırmızı sağda -900, -30
	pros::delay(2000); // mavi sol / kırmızı sağda 2000
	stack_lift(-1);
	intake(0);
}

void open_stack()
{
	stack_lift(1, 127);
	pros::delay(180);
	intake(-1);
	stack_lift(-1);
	pros::delay(200);
	stack_lift(1, 127);
	pros::delay(350);
	pros::delay(180);
	stack_lift(-1,100);
	pros::delay(200);
	intake(0);
	pros::delay(350);
	arm_mtr1.move_relative(600, 127);//500du
	arm_mtr2.move_relative(600, 127);//500du
	pros::delay(200);
}

void initialize() {
	pros::lcd::initialize();
	arm_mtr1.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	arm_mtr2.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	stack_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	stack_mtr2.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	stack_mtr.set_gearing(pros::E_MOTOR_GEARSET_18);
	stack_mtr2.set_gearing(pros::E_MOTOR_GEARSET_18);
}

void disabled() {}
void competition_initialize() {}

void autonomous()
{

	arm_mtr1.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	arm_mtr2.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	stack_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	stack_mtr2.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	//this is old af
	// go(500,127);
	// pros::delay(500);
	// go(-500,127);
	// pros::delay(500);
	// for(int i = 0; i<2; i++)
	// {
	// stack_lift(1, 127);
	// pros::delay(450);
	// stack_lift(-1, 127);
	// pros::delay(400);
	// }
	// arm_mtr1.move_relative(600, 127);
	// arm_mtr2.move_relative(600, 127);
	// pros::delay(150);
	//this is old af
	//
	// open_stack();
	// go(2800, 72); 			// küçüklerde 3000, 72 				büyüklerde 3500,127
	// pros::delay(500); 	// küçüklerde 500							büyüklerde 500
	// stack_lift(-1, 127);// ikisinde de var
	// intake(1);					// ikisinde de var
	// pros::delay(2800); 	// küçüklerde 2800 						büyüklerde  3400
	// go(-1000, -100); 		// küçüklerde -1000, -100 		büyüklerde -2250, -90
	// pros::delay(1000); 	// küçüklerde 1000 						büyüklerde  1000
	// turn_fixed(97, 87); // mavi küçükte(sol) 96, 87		büyüklerde -65,		 87
	// // küçükler için
	// pros::delay(1700);	// küçüklerde 1700						büyüklerde 1200
	// go(1855, 75); 			// küçüklerde 1900, 75 				büyüklerde 2400, 75
	// pros::delay(200);
	// // intake(-2);					// küçüklerde
	// // pros::delay(475); 	// küçüklerde 475
	// // küçükler için
	// pros::delay(500); 	// küçüklerde 1400
	//
	// intake(-2);
	// pros::delay(450); //????
	// intake(0);					// ikisinde de var
	// pros::delay(725);
	// // stack bırakma
	// go(-100,50);
	// stack_lift(1);
	// arm_mtr1.move_relative(-300, 127);
	// arm_mtr2.move_relative(-300, 127);
	// pros::delay(1900); // küçüklerde 190+0
	// intake(1);				// ikisinde de var
	// arm_mtr1.move_relative(150, 60);
	// arm_mtr2.move_relative(150, 60);
	// pros::delay(650); // küçüklerde 400
	// intake(-2);				// ikisinde de var
	// pros::delay(350); // küçüklerde 700
	// go(-900,-30);			// küçüklerde -900, -30
	// pros::delay(1000);// küçüklerde 2000			büyüklerde 2000
	// stack_lift(-1);		// ikisinde de var
	// arm_mtr1.move_relative(150, 60);
	// arm_mtr2.move_relative(150, 60);
	// intake_mtr1.move(0);				// ikisinde de var
	// intake_mtr2.move(0);
	// pros::delay(300);
	// left_mtr.move_relative(-800, 80); // 800 sağa oynarken
	// right_mtr.move_relative(1000, 80); // -1000 sağa oynarken
	// pros::delay(2070);
	// go(2100,80);
	// pros::delay(1000);
	// intake(1);
	// pros::delay(850);
	// intake(0);
	// stack_lift(0);
	// pros::delay(200);
	// go(-400, 80);
	// pros::delay(200);
	// arm_mtr1.move_relative(4500, 127);
	// arm_mtr2.move_relative(4300, 127);
	// pros::delay(2500);
	// intake(-1);
	// pros::delay(1000);
	// intake(0);
	// arm_mtr1.move_relative(-4500, 127);
	// arm_mtr2.move_relative(-4300, 127);
	// pros::delay(2500);
	// left_mtr.move_relative(115,70);
	// right_mtr.move_relative(-140,70);
	// pros::delay(600);
	// go(1000,50);
	// intake(1);
	// stack_lift(-1);
	// pros::delay(1500);
	// intake(0);
	// go(-2300,75);
	// pros::delay(3000);
	// left_mtr.move_relative(1282,50);
	// right_mtr.move_relative(-1282,50);
	// stack_lift(0);
	// pros::delay(2400);
	// arm_mtr1.move_relative(4500, 127);
	// arm_mtr2.move_relative(4300, 127);
	// pros::delay(2500);
	// go(320,70);
	// pros::delay(1000);
	// intake(-2);
	// pros::delay(1000);
	// intake(0);
	// pros::delay(200);
	// go(-1000,70);
	// arm_mtr1.move_relative(-4500, 127);
	// arm_mtr2.move_relative(-4300, 127);

	//1165, -1088
	// stack bırakma
//4509solmot 1424sag       3698sol 2416sag    -800sol   1000sag
//arm1 4478 			arm2 4277
//125sol -145sag
	open_stack();
	autonomous_recorder = fopen(FILE_PATH, "r");
	do
	{
		fscanf(autonomous_recorder, "%f %,f %f %f %f/", &left_mtr_auto, &right_mtr_auto, &stack_mtr_auto, &intake_mtr_auto, &arm_mtr_auto);
		// std::cout << left_mtr_auto << " " << right_mtr_auto << " " << stack_mtr_auto << " " << intake_mtr_auto << " " << arm_mtr_auto << std::endl;
		left_mtr.move_velocity(left_mtr_auto);
		right_mtr.move_velocity(right_mtr_auto);
		stack_mtr.move_velocity(stack_mtr_auto);
		stack_mtr2.move_velocity(stack_mtr_auto);
		intake_mtr1.move_velocity(intake_mtr_auto);
		intake_mtr2.move_velocity(intake_mtr_auto);
		arm_mtr1.move_velocity(arm_mtr_auto);
		arm_mtr2.move_velocity(arm_mtr_auto);

		// // fscanf(autonomous_recorder,"%d %d %d %d %d %d %d/", &power, &turn, &arm_power, &IS_PRESSED_DIGITAL_L1, &IS_PRESSED_DIGITAL_L2, &IS_PRESSED_DIGITAL_R1, &IS_PRESSED_DIGITAL_R2);
		// fscanf(autonomous_recorder,"%d %d %d %d %d %d %d %d %d %d/", &power, &turn, &arm_power, &stack_buttons, &IS_PRESSED_DIGITAL_L1, &IS_PRESSED_DIGITAL_L2, &IS_PRESSED_DIGITAL_R1, &IS_PRESSED_DIGITAL_R2, &left_mtr_position, &right_mtr_position);
		// int left = power + turn;
		// int right = power - turn;
		//
		// if(IS_PRESSED_DIGITAL_L1 == 1)
		// {
		// 	if(stack_state == -1)
		// 	{
		// 		stack_lift(1);
		// 	}
		// 	else
		// 	{
		// 		stack_lift(-1);
		// 	}
		// 	IS_PRESSED_DIGITAL_L1 = 0;
		// }
		//
		// if(IS_PRESSED_DIGITAL_L2 == 1)
		// {
		// 	if(intake_mtr1.get_actual_velocity() < -10) intake(0);
		// 	else
		// 	{
		// 		intake(-2);
		// 	}
		// 	IS_PRESSED_DIGITAL_L2 = 0;
		// }
		//
		// if(IS_PRESSED_DIGITAL_R1 == 1)
		// {
		// 	if(intake_mtr2.get_actual_velocity() > 5) intake(0);
		// 	else intake(1);
		// 	IS_PRESSED_DIGITAL_R1 = 0;
		// }
		//
		// if(IS_PRESSED_DIGITAL_R2 == 1)
		// {
		// 	if(intake_mtr2.get_actual_velocity() < -5) intake(0);
		// 	else intake(-1);
		// 	IS_PRESSED_DIGITAL_R2 = 0;
		// }

		// if((abs(left_mtr_position - left_mtr.get_position()) > 100) || (abs(right_mtr_position - right_mtr.get_position()) > 100))
		// {
		// 	if(abs(left_mtr_position - left_mtr.get_position()) > 100)
		// 	{
		//
		// 		left_mtr.move_absolute(left_mtr_position, left_mtr_auto);
		// 		pros::delay(50);
		// 	}
		//
		// 	if(abs(right_mtr_position - right_mtr.get_position()) > 100)
		// 	{
		//
		// 		right_mtr.move_absolute(right_mtr_position, right_mtr_auto);
		// 		pros::delay(50);
		// 	}
		// }
		// else
		// {
			// arm_mtr1.move_velocity(arm_power);
			// arm_mtr2.move_velocity(arm_power);
			// left_mtr.move(left);
			// right_mtr.move(right);
			pros::delay(50);
		// }
	}while(!(feof(autonomous_recorder) || ferror(autonomous_recorder)));
}


void opcontrol()
{
	arm_mtr1.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	arm_mtr2.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	stack_mtr.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	stack_mtr2.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
	while (true)
	{
		int power = master.get_analog(ANALOG_LEFT_Y);
		int turn = master.get_analog(ANALOG_LEFT_X);
		int arm_power = master.get_analog(ANALOG_RIGHT_Y );
		int stack_power = master.get_analog(ANALOG_RIGHT_X) / 3.8;
		int left = power + turn;
		int right = power - turn;
		left_mtr.move(left);
		right_mtr.move(right);

		// if(master.get_digital(DIGITAL_LEFT)) arm_pressed_left = true;
		//
		// if(master.get_digital(DIGITAL_RIGHT)) arm_pressed_right = true;
		//
		// if(arm_pressed_left)	arm_mtr1.move(100);
		// else arm_mtr1.move(0);
		//
		// if(arm_pressed_right)	arm_mtr2.move(100);
		// else arm_mtr2.move(0);

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
			IS_PRESSED_DIGITAL_L1 = 1;
		}

		while(master.get_digital(DIGITAL_LEFT) || master.get_digital(DIGITAL_RIGHT))
		{
			if(master.get_digital(DIGITAL_LEFT) ? arm_pressed_left = true : arm_pressed_right = true)

			if (arm_pressed_left)
			{
				arm_mtr1.move_velocity(100);
			}

			if (arm_pressed_right)
			{
				arm_mtr2.move_velocity(100);
			}
			arm_pressed_left = false;
			arm_pressed_right = false;
		}
		if(!(arm_power > -10 && arm_power < 10))
		{
			arm_mtr1.move(0);
			arm_mtr2.move(0);
		}

		if(master.get_digital_new_press(DIGITAL_L2))
		{
			if(intake_mtr1.get_actual_velocity() < -10) intake(0);
			else
			{
				intake(-2);
			}
			IS_PRESSED_DIGITAL_L2 = 1;
		}

		if(master.get_digital_new_press(DIGITAL_R1))
		{
			if(intake_mtr2.get_actual_velocity() > 5) intake(0);
			else intake(1);
			IS_PRESSED_DIGITAL_R1 = 1;
		}

		if(master.get_digital_new_press(DIGITAL_R2))
		{
			if(intake_mtr2.get_actual_velocity() < -5) intake(0);
			else intake(-1);
			IS_PRESSED_DIGITAL_R2 = 1;
		}

		if(master.get_digital_new_press(DIGITAL_X) )
		{
			open_stack();
		}


		//if((arm_mtr2.get_position() < -20) && (arm_power < 0)) arm_power = 0; // don't allow arms to crash the ground

		arm_mtr1.move(arm_power);
		arm_mtr2.move(arm_power);

		// if((!(stack_mtr.get_position() < -35 && stack_power < 0) || !(stack_mtr.get_position() > CORRECTED_STACK_MOTOR_ROTATION_FORWARD+50 && stack_power > 0))
		// && (master.get_digital(DIGITAL_RIGHT) || master.get_digital(DIGITAL_LEFT) ))
		// {
		// 	stack_mtr.move_relative(master.get_digital(DIGITAL_RIGHT)?110:-110, 50);
		// 	stack_mtr2.move_relative(master.get_digital(DIGITAL_RIGHT)?110:-110, 50);
		// 	pros::delay(150);
		// 	stack_buttons = master.get_digital(DIGITAL_RIGHT)?1:-1;
		// }

		// if(abs(stack_mtr.get_target_velocity()) != STACK_MTR_VELOCITY )
		// {
		// 	stack_mtr.move_velocity(stack_power);
		// 	stack_mtr2.move_velocity(stack_power);
		// }

		if(master.get_digital_new_press(DIGITAL_B))
		{
			autonomous_recorder = fopen(FILE_PATH, "w");
			open_stack();
			std::cout << autonomous_recorder;
			file_flag = true;
		}

		// 1. left motor
		// 2. right motor
		// 3. stack motor(s)
		// 4. intake motor(s)
		// 5. arm motor(s)
		if(file_flag)	fprintf(autonomous_recorder,"%f %f %f %f %f/", left_mtr.get_actual_velocity(), right_mtr.get_actual_velocity(),stack_mtr.get_actual_velocity(), intake_mtr1.get_actual_velocity(), arm_mtr1.get_actual_velocity());

		// if(file_flag)
		// {
		// fprintf(autonomous_recorder,"%d %d %d %d %d %d %d %d/", power, turn, arm_power, stack_buttons, IS_PRESSED_DIGITAL_L1, IS_PRESSED_DIGITAL_L2,IS_PRESSED_DIGITAL_R1, IS_PRESSED_DIGITAL_R2);
		// IS_PRESSED_DIGITAL_L1 = 0;
		// IS_PRESSED_DIGITAL_L2 = 0;
		// IS_PRESSED_DIGITAL_R1 = 0;
		// IS_PRESSED_DIGITAL_R2 = 0;
		// stack_buttons = 0;
		// }

		// if(file_flag)
		// {
		// fprintf(autonomous_recorder,"%d %d %d %d %d %d %d %d %d %d/", power, turn, arm_power, stack_buttons, IS_PRESSED_DIGITAL_L1, IS_PRESSED_DIGITAL_L2,IS_PRESSED_DIGITAL_R1, IS_PRESSED_DIGITAL_R2, left_mtr.get_position(), right_mtr.get_position());
		// IS_PRESSED_DIGITAL_L1 = 0;
		// IS_PRESSED_DIGITAL_L2 = 0;
		// IS_PRESSED_DIGITAL_R1 = 0;
		// IS_PRESSED_DIGITAL_R2 = 0;
		// stack_buttons = 0;
		// }

		if(master.get_digital_new_press(DIGITAL_Y))
		{
			fclose(autonomous_recorder);
			file_flag = false;
		}

		if(master.get_digital_new_press(DIGITAL_A))
		{
			stack_lift(0);
		}

		if(master.get_digital_new_press(DIGITAL_UP)) autonomous_stack();
		// if(master.get_digital_new_press(DIGITAL_RIGHT))
		// {
		//
		// }
		// if(master.get_digital_new_press(DIGITAL_LEFT))
		// {
		//
		// }
		pros::lcd::print(0, "left  motor encoder: %f\n", left_mtr.get_position());
		pros::lcd::print(1, "right motor encoder: %f\n", right_mtr.get_position());
		pros::lcd::print(2, "arm1  motor encoder: %f\n", arm_mtr1.get_position());
		pros::lcd::print(3, "arm2 motor encoder: %f\n", arm_mtr2.get_position());
		// std::cout << "left motor position: " << left_mtr.get_position() << std::endl;
		// std::cout << "right motor position: " << right_mtr.get_position() << std::endl;

		pros::delay(50);
	}
}
