#include "JARVIS.h"
#include<Arduino.h>
#include<Adafruit_PWMServoDriver.h>
#include<Wire.h>


JARVIS::JARVIS()
    :pwm(0x40),
    head(pwm,0,1,"head", 0),
    front_LL_hip(pwm,1,2, "front_ll_hip",0),
    front_RL_hip(pwm,2,3, "front_RL_hip",0),
    front_LL(pwm,3,4, "front_LL", -90),
    front_RL(pwm,4,5, "front_RL", -90),
    back_LL_hip(pwm,5,6, "Back_LL_hip",0),
    back_RL_hip(pwm,6,7, "Back_RL_hip",0), 
    back_LL(pwm,7,8, "Back_LL", -90),
    back_RL(pwm,8,9, "Back_RL", -90)
    {
        all_servos[0] = &head;
        all_servos[1] = &front_LL_hip;
        all_servos[2] = &front_RL_hip;
        all_servos[3] = &front_LL;
        all_servos[4] = &front_RL;
        all_servos[5] = &back_LL_hip;
        all_servos[6] = &back_RL_hip;
        all_servos[7] = &back_LL;
        all_servos[8] = &back_RL;
    }

void JARVIS::initialize(){
    Wire.begin();
    pwm.begin();
    pwm.setPWMFreq(50);
    for(int i = 0; i < SERVO_COUNT; i++){
        all_servos[i]->initialize();
    }
    Serial.println("All servos initialized successfully.");
}

void JARVIS::neutral_position(){
    for(int i = 0; i < SERVO_COUNT; i++){
        all_servos[i]->move_to_neutral();
    }
}

void JARVIS::print_robot_status(){
    Serial.println("=================================================");
    Serial.println("JARVIS STATUS");
    Serial.println("=================================================");
    for(int i = 0; i < SERVO_COUNT; i++){
        all_servos[i]->print_configuration();
    }
}

void JARVIS::move_servo(int id, double degree){
    if(id < 1 || id > SERVO_COUNT){
        return;
    }
    all_servos[id-1]->move_servo(degree);
}

