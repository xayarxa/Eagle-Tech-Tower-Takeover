#include "main.h"

#define LEFT_MTR_PORT 20
#define RIGHT_MTR_PORT 1
#define STACK_MTR_PORT 15
#define INTAKE_MTR_PORT 5
#define VISION_PORT 10

#define STACK_MTR_POWER 100
#define INTAKE_MTR_POWER 100


/*
x:0-310
y:0-210 (inverted)

cube width height: min vision 30, max vision 200
*/
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
	pros::Controller master (pros::E_CONTROLLER_MASTER);
	pros::Motor left_mtr(LEFT_MTR_PORT);
	pros::Motor right_mtr (RIGHT_MTR_PORT, true);
	pros::Motor stack_mtr(STACK_MTR_PORT);
	pros::Motor intake_mtr(INTAKE_MTR_PORT);
	pros::Vision vision_sensor(VISION_PORT);
	stack_mtr.set_gearing(pros::E_MOTOR_GEARSET_36);
}




/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}





/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}


void autonomous() {
	while(true)
	{
		pros::vision_object cube = vision_sensor.get_by_size(0);
		power = 0;
		turn = 0;
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
			if(present != 0) unsigned int present = pros::millis();
		}
		if(pros::millis() == )
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

		while(master.get_digital(DIGITAL_R1))
		{
			intake_mtr.move(INTAKE_MTR_POWER);
			pros::delay(5);
		}

		while(master.get_digital(DIGITAL_R2))
		{
			intake_mtr.move(-INTAKE_MTR_POWER);
			pros::delay(5);
		}
		pros::lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);

		int left = power + turn;
		int right = power - turn;
		left_mtr.move(left);
		right_mtr.move(right);
		pros::delay(20);
	}
}
