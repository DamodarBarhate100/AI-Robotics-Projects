#include <Wire.h>
#include <math.h>
#include "terrain_model.h"
#include <SD.h>

// Adding the Trained Machine Learning Model
Eloquent::ML::Port::LogisticRegression softmax_model;

int toggle_switch = 27;
const int feature_size = 8;
int total_samples_collected = 0;
int total_prediction_Val = 0;
int data_collection_pin = 21;
int right_side_penalty = 45;

// The container for the 6 raw ingredients
struct read_vals {
  int16_t accel_x;
  int16_t accel_y;
  int16_t accel_z;
  int16_t gyro_z;      
  int16_t current_val; 
  int pwm_val;         
};

// Sliding Window Parameters 
const int WINDOW_SIZE = 66; // 66 loops at 15ms = 1 second of data
float accel_z_buffer[WINDOW_SIZE];
float gyro_z_buffer[WINDOW_SIZE];
float current_buffer[WINDOW_SIZE];
float pitch_buffer[WINDOW_SIZE];
int buffer_index = 0;
float features[feature_size] = {0};

const int FILTER_SIZE = 5;
int prediction_buffer[FILTER_SIZE] = {0, 0, 0, 0, 0};
int filter_idx = 0;
bool filter_filled = false;


// L298N - Motor Drivers
int EN_A = 2;
int IN_1 = 4;
int IN_2 = 16;
int EN_B = 32;
int IN_3 = 17;
int IN_4 = 33;

// I2C Bus 0 (INA Sensor)
const uint8_t SDA_INA = 25;
const uint8_t SCL_INA = 26;

// I2C Bus 1 (MPU Sensor)
const uint8_t SDA_MPU = 13;
const uint8_t SCL_MPU = 14;

// Sensor Addresses 
const int MPU_addr = 0x68;
const int INA_addr = 0x40;



byte status_mpu;
byte status_ina;

TwoWire I2C_MPU = TwoWire(1);

// Function Declarations
void move_forward();
void read_Register(byte device_addr, byte reg_addr, read_vals *ptr);
bool print_sensor_vals();
void predict_output();
void save_to_sd();
void stop_motors();
int get_stable_prediction(int new_prediction); 

void setup() {
  Serial.begin(115200);
  // Checking for SD Card
  if (!SD.begin(5)) { 
    Serial.println("Card Mount Failed");
    // return;
  }

  // Initializing Both the sensors using I2C protocol
  Wire.begin(SDA_INA, SCL_INA, 100000);
  I2C_MPU.begin(SDA_MPU, SCL_MPU, 100000);
  I2C_MPU.beginTransmission(MPU_addr);
  I2C_MPU.write(0x6B); // powers the sensor
  I2C_MPU.write(0x00);  // wakes up the sensor
  status_mpu = I2C_MPU.endTransmission();
    if(status_mpu == 0){
    Serial.println("MPU-6050 Sensor Initialized");
  }
  else{
    Serial.println("MPU-6050 Sensor Not Found");
  }

  // Current Sensor INA219
  Wire.beginTransmission(INA_addr);
  status_ina = Wire.endTransmission();
  if(status_ina == 0){
    Serial.println("INA219 Sensor Initialized");
  }
  else{
    Serial.println("INA219 Sensor Not Found");
  }

  //  Intializing Motor Pins 
  pinMode(EN_A, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(EN_B, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  // Toggle Switch
  pinMode(toggle_switch, INPUT_PULLUP);
  pinMode(data_collection_pin, OUTPUT);

}

unsigned long previous_millis = 0;
const unsigned long interval = 15;
int current_speed = 80;


void loop() {
  unsigned long current_millis = millis();
  if(current_millis - previous_millis >= interval){
    previous_millis = current_millis;
    bool is_collecting = (digitalRead(toggle_switch) == LOW);
    if((is_collecting && total_samples_collected<=10) || (!is_collecting && total_prediction_Val <= 20)){
      move_forward();
      if (print_sensor_vals() == true) { 
        
        if(digitalRead(toggle_switch) == LOW){
          Serial.println("Data Collection Phase Starting ....");
          digitalWrite(data_collection_pin, HIGH);
          save_to_sd();
        }
        else if(digitalRead(toggle_switch) == HIGH){
          Serial.println("Predicting Phase Starting ....");
          digitalWrite(data_collection_pin, LOW);
          predict_output();
        }
      }
    }
    else{
      stop_motors();
      digitalWrite(data_collection_pin, LOW);
    }
  }
}


void move_forward(){
    // Forward direction
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_4, LOW);
  if(current_speed < 220){
    analogWrite(EN_A, current_speed - right_side_penalty);
    analogWrite(EN_B, current_speed);
    current_speed++;
  }
}

bool print_sensor_vals(){
  read_vals vals = {0,0,0,0,0,0};
  float_t accel_x, accel_y, accel_z, current_v, gyro_z, pwm_val, pitch;

  // For Motion - Control Sensor
  read_Register(MPU_addr, 0x3B, &vals);
  // converting the values to meter square per second
  accel_x = (vals.accel_x/16384.0) * 9.81;
  accel_y = (vals.accel_y/16384.0) * 9.81;
  accel_z = (vals.accel_z/16384.0) * 9.81;
  gyro_z = vals.gyro_z / 131.0;

  // For Current Sensor
  read_Register(INA_addr, 0x01, &vals);  // shunt voltage 0x01  --  where I = v_shunt/ r_shunt =  v_shunt/0.1 
  current_v = vals.current_val * 0.0001;  // converting to AMps
  
  pwm_val = vals.pwm_val;
  pitch = atan2(accel_x, sqrt(pow(accel_y,2)+ pow(accel_z,2) + 0.001)) * (180/PI);

  accel_z_buffer[buffer_index] = accel_z;
  gyro_z_buffer[buffer_index] = gyro_z;
  current_buffer[buffer_index] = current_v;
  pitch_buffer[buffer_index] = pitch;  // tells the tilt in forward or backward direction
  buffer_index++;

  if (buffer_index >= WINDOW_SIZE) {
    float_t accel_z_sum = 0 , gyro_z_sum = 0, max_current = 0, current_buffer_sum = 0, pitch_buffer_sum = 0;
    for(int i =0; i<WINDOW_SIZE; i++){
      accel_z_sum += accel_z_buffer[i];
      gyro_z_sum += gyro_z_buffer[i];
      current_buffer_sum += current_buffer[i];
      pitch_buffer_sum += pitch_buffer[i];
      if(current_buffer[i]>max_current){
        max_current = current_buffer[i];
      }
    }

    float accel_z_mean = accel_z_sum/WINDOW_SIZE;
    float gyro_z_mean = gyro_z_sum/WINDOW_SIZE;
    float pitch_angle_mean = pitch_buffer_sum/WINDOW_SIZE;
    float current_mean = current_buffer_sum/WINDOW_SIZE;

    // Now for standard deviations
    float accel_z_std_sum= 0, gyro_z_std_sum = 0;

    for(int i=0; i<WINDOW_SIZE; i++){
      accel_z_std_sum += pow(accel_z_buffer[i] - accel_z_mean, 2);
      gyro_z_std_sum += pow(gyro_z_buffer[i] - gyro_z_mean, 2);
    }

    float accel_z_std = sqrt((accel_z_std_sum/WINDOW_SIZE)+ 0.001);  
    float gyro_z_std = sqrt((gyro_z_std_sum/WINDOW_SIZE)+ 0.001);  

    features[0] = accel_z_mean;
    features[1] = accel_z_std;
    features[2] = gyro_z_mean;
    features[3] = gyro_z_std;
    features[4] = pitch_angle_mean;
    features[5] = current_mean;
    features[6] = max_current;
    features[7] = pwm_val;
    buffer_index = 0;
    return true;
  }
  return false;
}

void read_Register(byte device_addr, byte reg_addr ,  read_vals *ptr){

  TwoWire &bus  = (device_addr == 0x68) ? I2C_MPU : Wire;
  
  //  For INA sensor 
  bus.beginTransmission(device_addr);
  bus.write(reg_addr);
  bus.endTransmission(false);
  if(device_addr == INA_addr){
    bus.requestFrom((uint8_t)device_addr, (uint8_t)2, (uint8_t)1);
    if(bus.available() >=2  && reg_addr == 0x01){
      ptr->current_val = bus.read() << 8 | bus.read();  
    }
  }  
  else if(device_addr == MPU_addr){
    bus.requestFrom((uint8_t)device_addr, (uint8_t)14, (uint8_t)1);
    if(bus.available() >= 14){
        ptr->accel_x = bus.read() << 8 | bus.read();
        ptr->accel_y = bus.read() << 8 | bus.read();
        ptr->accel_z = bus.read() << 8 | bus.read();
        // had to read to clear the buffer
        int16_t temp = bus.read() << 8 | bus.read();
        int16_t gyro_x = bus.read() << 8 | bus.read();
        int16_t gyro_y = bus.read() << 8 | bus.read();

        ptr->gyro_z = bus.read() << 8 | bus.read();
      }    
    }
    ptr->pwm_val = current_speed;
}

void save_to_sd(){
  char data_row[128];
  snprintf(data_row, sizeof(data_row), "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.2f\n",
    features[0],
    features[1],
    features[2],
    features[3],
    features[4],
    features[5],
    features[6],
    features[7]
    );
    File myFile = SD.open("/rover_Data.csv", FILE_APPEND);
    if (myFile) {
        Serial.println("Starting to append the data into the file");
        total_samples_collected++;
        myFile.print(data_row); 
        myFile.close(); 
    } 
    else {
        Serial.println("ERROR: SD Card write failed. Check connection!");
    }
}

void predict_output(){
  const char* terrainLabels[] = {"Grass", "Gravel", "Rough", "Smooth", "Smooth Inclined"};;
  
  // Scaling the features first
  const float MEAN_[8] = {-9.882731708468564, 2.959839039282236, 3.415522122738882, 12.285473302366416, 
  -5.720796618328928, 1.6345811891807578, 2.0837636624260156, 210.7473309608541};
  const float STD_[8] = {0.34501976650919025, 1.9643359498658335, 10.806722062567259, 11.189390677602395,
   2.1215192413605046, 0.32169298689401393, 0.2698251895047187, 23.336549045349035};
  
  float scaled_feature[feature_size] = {0};
  for(int i=0; i<feature_size; i++){
      scaled_feature[i] = (features[i] - MEAN_[i])/STD_[i];
  }

  unsigned long start_time = micros();
  int raw_prediction_id = softmax_model.predict(scaled_feature);
  
  int stable_prediction_id = get_stable_prediction(raw_prediction_id);
  
  unsigned long end_time = micros();
  
  float inference_ms = (end_time - start_time) / 1000.0;
  
  Serial.print("Terrain Detected: ");
  Serial.println(terrainLabels[stable_prediction_id]); 
  Serial.print("Inference Time (ms): ");
  Serial.println(inference_ms);

  // Structure: F1, F2, F3, F4, F5, F6, F7, F8, Latency, Predicted_Label
  char predict_row[128];
  snprintf(predict_row, sizeof(predict_row), "%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f,%.2f,%.2f,%s\n",
    features[0],
    features[1],
    features[2],
    features[3],
    features[4],
    features[5],
    features[6],
    features[7],
    inference_ms,
    terrainLabels[stable_prediction_id] 
  );

  File predict_file = SD.open("/predictions.csv", FILE_APPEND);
  if (predict_file) {
      total_prediction_Val++;
      predict_file.print(predict_row); 
      predict_file.close(); 
  } 
  else {
      Serial.println("ERROR: SD Card write failed. Check connection!");
  }
}

void stop_motors(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(EN_A, 0);
  analogWrite(EN_B, 0);
}

int get_stable_prediction(int new_prediction) {
  prediction_buffer[filter_idx] = new_prediction;
  filter_idx = (filter_idx + 1) % FILTER_SIZE;

  if (filter_idx == 0) filter_filled = true;
  if (!filter_filled) return new_prediction;

  int max_count = 0;
  int stable_pred = prediction_buffer[0];
  
  for (int i = 0; i < FILTER_SIZE; i++) {
    int count = 0;
    for (int j = 0; j < FILTER_SIZE; j++) {
      if (prediction_buffer[j] == prediction_buffer[i]) {
        count++;
      }
    }
    if (count > max_count) {
      max_count = count;
      stable_pred = prediction_buffer[i];
    }
  }
  return stable_pred;
}