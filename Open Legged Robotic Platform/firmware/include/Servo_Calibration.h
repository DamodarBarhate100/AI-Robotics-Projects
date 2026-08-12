#ifndef SERVO_CALIBRATION_H
#define SERVO_CALIBRATION_H
#include<Arduino.h>
#include <Adafruit_PWMServoDriver.h>

struct servo_config{
    int servo_id;
    const char *servo_name;
    uint8_t  pca_channel;
    double servo_offset;
    double neutral_angle;
    double current_angle;


};

class Servo_Calibration{
    private:
        servo_config config;
        int max_angle;
        int min_angle;
        Adafruit_PWMServoDriver& pwm;
    public:
        Servo_Calibration( Adafruit_PWMServoDriver& driver, uint8_t channel, int servo_id, const char* servo_name, double neutral_angle, float offset=0);
        // Initialization
        void initialize();
        void move_to_neutral();
        
        // Movement
        void safety_check(double degree);
        void move_servo(double degree);

        // Persistance
        void save_offset(float offset_val);
        float load_offset();

        // Utility
        void move_to_center();
        void print_configuration();
        void print_logical_angle();

};

#endif