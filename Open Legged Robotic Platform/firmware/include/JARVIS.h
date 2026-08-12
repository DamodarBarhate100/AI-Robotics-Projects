#ifndef JARVIS_H
#define JARVIS_H
#include "Servo_Calibration.h" 

class JARVIS{
    private:
    Adafruit_PWMServoDriver pwm;

    Servo_Calibration *all_servos[9];
    Servo_Calibration head;
    Servo_Calibration front_LL_hip;
    Servo_Calibration front_RL_hip;
    Servo_Calibration front_LL;
    Servo_Calibration front_RL;
    Servo_Calibration back_RL_hip;
    Servo_Calibration back_LL_hip;
    Servo_Calibration back_LL;
    Servo_Calibration back_RL;
    static constexpr uint8_t SERVO_COUNT = 9;

    public:
        JARVIS();
        void initialize();
        void neutral_position();
        void print_robot_status();
        void move_servo(int id, double angle);

};

#endif