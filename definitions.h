/*
x:0-310
y:0-210 (inverted)

cube width height: min vision 30, max vision 200
*/

/*
#pragma once
#ifndef DEFINITIONS_H
#define DEFINITIONS_H
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

#endif
*/
