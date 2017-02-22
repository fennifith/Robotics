#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(Sensor, dgtl1,  encoder3,       sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  encoder4,       sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  touchSensor4,   sensorTouch)
#pragma config(Sensor, dgtl6,  touchSensor3,   sensorTouch)
#pragma config(Motor,  port1,           leftMotor,     tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           armMotor3,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           strongArm,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           strongArm2,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           klaw1,         tmotorVex269_MC29, openLoop)
#pragma config(Motor,  port6,           klaw2,         tmotorVex269_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          rightMotor,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
	// Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task autonomous() {
  SensorValue[encoder3] = 0;
	SensorValue[encoder4] = 0;

	//should be holding a star - grab it
	motor[klaw1] = 100;
	motor[klaw2] = 100;

	//move forwards to the fence while raising the arm
	for (int i = 0; i < 4000; i++) {
		motor[leftMotor] = -127;
		motor[rightMotor] = -127;

		motor[armMotor3] = SensorValue(encoder3) + 150;

		motor[strongArm] = (3.5 * SensorValue(encoder4)) - 380;
		motor[strongArm2] = (3.5 * SensorValue(encoder4)) - 380;

		sleep(1);
	}

	motor[leftMotor] = 0;
	motor[rightMotor] = 0;

	//drop the star
	motor[klaw1] = -127;
	motor[klaw2] = -127;

	//shake the arm
	motor[armMotor3] = -127;
	sleep(200);
	motor[armMotor3] = 127;
	sleep(200);
	motor[armMotor3] = -127;
	sleep(200);
	motor[armMotor3] = 127;
	sleep(200);
	motor[armMotor3] = -127;
	sleep(200);
	motor[armMotor3] = 127;
	sleep(200);
	motor[armMotor3] = 0;
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

float getSpeed();
int motor3Value;
int strongArmValue;

bool klaw;

task usercontrol() {
  SensorValue[encoder3] = 0;
	SensorValue[encoder4] = 0;

	while (true) {

		//drive motors
		motor[rightMotor] = (-vexRT[Ch4] - vexRT[Ch3] - vexRT[Ch2] - vexRT[Ch1] + motor[rightMotor]) / 3;
		motor[leftMotor] = (vexRT[Ch4] - vexRT[Ch3] - vexRT[Ch2] + vexRT[Ch1] + motor[rightMotor]) / 3;

		//upper arm motor
		if (vexRT[Btn5U]) {
			motor[armMotor3] = -getSpeed();
			motor3Value = 2 * SensorValue(encoder3);
		} else if (vexRT[Btn5D]) {
			motor[armMotor3] = getSpeed();
			motor3Value = 2 * SensorValue(encoder3);
		} else if (vexRT[Btn7U]) {
			motor[armMotor3] = SensorValue(encoder3) + 150;
			motor3Value = 2 * SensorValue(encoder3);
		} else {
			motor[armMotor3] = (2 * SensorValue(encoder3)) - motor3Value;
		}
		
		if (SensorValue(touchSensor3)) {
			motor3Value = 0;
			SensorValue[encoder3] = 0;
		}

		//lower arm motors
		if (vexRT[Btn6U]) {
			motor[strongArm] = -getSpeed();
			motor[strongArm2] = -getSpeed();
			strongArmValue = 3.5 * SensorValue(encoder4);
		} else if (vexRT[Btn6D]) {
			motor[strongArm] = getSpeed();
			motor[strongArm2] = getSpeed();
			strongArmValue = 3.5 * SensorValue(encoder4);
		} else if (vexRT[Btn7U]) {
			motor[strongArm] = (3.5 * SensorValue(encoder4)) - 360;
			motor[strongArm2] = (3.5 * SensorValue(encoder4)) - 360;
			strongArmValue = 3.5 * SensorValue(encoder4);
		} else {
			motor[strongArm] = (3.5 * SensorValue(encoder4)) - strongArmValue;
			motor[strongArm2] = (3.5 * SensorValue(encoder4)) - strongArmValue;
		}
		
		if (SensorValue(touchSensor4)) {
			strongArmValue = 0;
			SensorValue[encoder4] = 0;
		}

		//set claw open/closed
		if (vexRT[Btn7R]) {
			motor[klaw1] = getSpeed();
			motor[klaw2] = getSpeed();
			klaw = true;
		} else if (vexRT[Btn7L]) {
			motor[klaw1] = -getSpeed();
			motor[klaw2] = -getSpeed();
			klaw = false;
		} else {
			if (klaw) {
				//hold the claw closed
				motor[klaw1] = getSpeed();
				motor[klaw2] = getSpeed();
			} else {
				motor[klaw1] = -5;
				motor[klaw2] = -5;
			}
		}

		//display encoder values on LCD (debug only)
		displayLCDNumber(0, 0, SensorValue(encoder3));
		displayLCDNumber(0, 6, SensorValue(encoder4) * 3.5);
	}
}

float getSpeed() {
	if (vexRT[Btn8R]) return 50;
	else return 127;
}
