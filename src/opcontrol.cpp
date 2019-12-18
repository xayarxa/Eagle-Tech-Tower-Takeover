#include "../include/main.h"
#include "../src/definitions.h"
//
// void opcontrol() {
// 	while (true) {
// 		int power = master.get_analog(ANALOG_LEFT_Y);
// 		int turn = master.get_analog(ANALOG_LEFT_X);
// 		//vision sensor start
// 		pros::vision_object cube = vision_sensor.get_by_size(0); // yesil dude
// 		pros::lcd::print(5, "%d", cube.x_middle_coord);
//
// 		//a function which makes the robot direct to the cube it sees
// 		while(master.get_digital(DIGITAL_X))
// 		{
// 				pros::vision_object cube = vision_sensor.get_by_size(0);
// 				power = 0;
// 				turn = 0;
// 			if(!(cube.x_middle_coord<180 && cube.x_middle_coord>130) && cube.x_middle_coord > 0) // if there is not an object and if there is, not in the middle
// 			{
// 				turn = (cube.x_middle_coord-155) / 5;
// 				pros::lcd::print(6, "imma be turnin'"); //3.5 for competition
// 			}
// 			else pros::lcd::print(6,"aint turnin for now");
// 			if((cube.height >= 30 || cube.width >=30) && (cube.height <= 200 || cube.width <=200) && cube.x_middle_coord > 0) power = 127; // if there is a cube(cube.x_middle_coord > 0) and if it is in the appropriate size, RUN INTO IT!!!!
// 			left_mtr.move(power+turn);
// 			right_mtr.move(power-turn);
// 			pros::delay(5);
// 		}
//
// 		while(master.get_digital(DIGITAL_L1))
// 	  {
// 			stack_mtr.move(STACK_MTR_POWER);
// 			pros::delay(5);
// 		}
//
// 		while(master.get_digital(DIGITAL_L2))
// 	  {
//   		stack_mtr.move(-STACK_MTR_POWER);
// 			pros::delay(5);
// 		}
//
// 		if(master.get_digital_new_press(DIGITAL_R1))
// 		{
// 			if(intake_mtr1.get_actual_velocity() > 50) intake(0);
// 			else intake(1);
// 		}
//
// 		if(master.get_digital_new_press(DIGITAL_R2))
// 		{
// 			if(intake_mtr1.get_actual_velocity() < -50) intake(0);
// 			else intake(-1);
// 		}
//
//     while(stack_mtr.get_position()<LIFT_MOTOR_ROTATION)
//     {
//       int lpwr=master.get_analog(ANALOG_RIGHT_Y);
//       stack_mtr.move(lpwr);
//     }
//
// 		int left = power + turn;
// 		int right = power - turn;
// 		left_mtr.move(left);
// 		right_mtr.move(right);
// 		pros::delay(20);
// 	}
// }
