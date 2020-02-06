// #include "../include/main.h"
// #include "../src/definitions.h"
//
// void autonomous() {
// 	while(true)
// 	{
// 		pros::vision_object cube = vision_sensor.get_by_size(0);
// 		int power = 0;
// 		int turn = 0;
//
// 		if(!(cube.x_middle_coord<180 && cube.x_middle_coord>130) && cube.x_middle_coord > 0) // if there is not an object and if there is, not in the middle
// 		{
// 			turn = (cube.x_middle_coord-155) / 5;
// 			unsigned int present = 0;
// 			pros::lcd::print(6, "imma be turnin'");
// 		}
//
// 		else pros::lcd::print(6,"aint turnin for now");
//
// 		if((cube.height >= 30 || cube.width >=30) && (cube.height <= 200 || cube.width <=200) && cube.x_middle_coord > 0)
// 		{
// 		    power = 127; // if there is a cube(cube.x_middle_coord > 0) and if it is in the appropriate size, RUN INTO IT!!!!
// 		}
//
// 		else
// 		{
// 			if(present < 100) unsigned int present = pros::millis();
// 			if(pros::millis() > present + 2000) break;
// 		}
//
// 		left_mtr.move(power+turn);
// 		right_mtr.move(power-turn);
// 		pros::delay(5);
// 	}
// }
