import pandas as pd

df = pd.read_csv("Softmax Regression Model/Datasets/raw_rover_Data.csv",header=None)

df_grass = df.iloc[0:20].copy()
df_grass['Surface_Type'] = "Grass"

df_rough = df.iloc[20:29].copy()
df_rough['Surface_Type'] = "Rough"


df_gravel = df.iloc[37:56].copy()
df_gravel['Surface_Type'] = "Gravel"

df_smooth_inclined = df.iloc[56:63].copy()
df_smooth_inclined['Surface_Type'] = "Smooth Inclined"

df_smooth = df.iloc[64:].copy()
df_smooth['Surface_Type'] = "Smooth"


clean_data = pd.concat([df_grass, df_rough, df_gravel, df_smooth_inclined, df_smooth], ignore_index=True)

clean_data = clean_data[clean_data.iloc[:, 0] != 0.0]  # drop if the accel_z_mean is 0

clean_data = clean_data.sample(frac=1).reset_index(drop=True)

# droping null values
clean_data.dropna(inplace=True)
clean_data.columns = ["accel_z_mean" , "accel_z_std", "gyro_z_mean" ,"gyro_z_std" ,
                      "pitch_angle_mean" , "current_mean", "max_current", "pwm_val","Label"]

clean_data.to_csv("Softmax Regression Model/Datasets/cleaned_rover_Data.csv", index=False)

print("Dataset Shape:", clean_data.shape)
print("Data Preview:\n", clean_data.head())