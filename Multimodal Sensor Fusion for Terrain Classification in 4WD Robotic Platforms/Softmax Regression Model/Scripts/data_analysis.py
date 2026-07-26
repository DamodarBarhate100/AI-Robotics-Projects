import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os

def generate_hackathon_visualizations():
    output_dir = "Images/visualizations"
    os.makedirs(output_dir, exist_ok=True)
    print(f"Directory '{output_dir}' is ready.")

    csv_path = "Softmax Regression Model/Datasets/cleaned_rover_Data.csv"
    try:
        df = pd.read_csv(csv_path)
        print(f"Loaded {len(df)} rows from {csv_path}")
    except FileNotFoundError:
        print(f"Error: Could not find {csv_path}. Please check the path.")
        return

    sns.set_theme(style="darkgrid", context="talk")
    
    target_col = "Label" if "Label" in df.columns else "Surface_Type"


    plt.figure(figsize=(10, 6))
    sns.boxplot(x=target_col, y="accel_z_std", data=df, palette="viridis")
    plt.title("Physical Vibration Signatures by Terrain")
    plt.ylabel("Z-Axis Standard Deviation (Variance)")
    plt.xlabel("Terrain Class")
    plt.tight_layout()
    plt.savefig(f"{output_dir}/01_Vibration_Profile.png", dpi=300)
    plt.close()
    print("Generated: 01_Vibration_Profile.png")


    plt.figure(figsize=(10, 6))
    sns.violinplot(x=target_col, y="current_mean", data=df, palette="magma")
    plt.title("Motor Load (Friction) by Terrain")
    plt.ylabel("Average Current Draw (Amps/Units)")
    plt.xlabel("Terrain Class")
    plt.tight_layout()
    plt.savefig(f"{output_dir}/02_Motor_Current_Profile.png", dpi=300)
    plt.close()
    print("Generated: 02_Motor_Current_Profile.png")


    plt.figure(figsize=(10, 6))
    sns.stripplot(x=target_col, y="pitch_angle_mean", data=df, size=8, alpha=0.7, palette="coolwarm", jitter=True)
    plt.title("Spatial Awareness: Ramp Detection")
    plt.ylabel("Pitch Angle (Degrees)")
    plt.xlabel("Terrain Class")
    plt.tight_layout()
    plt.savefig(f"{output_dir}/03_Pitch_Angle_Profile.png", dpi=300)
    plt.close()
    print("Generated: 03_Pitch_Angle_Profile.png")

 
    plt.figure(figsize=(12, 8))
    sns.scatterplot(x="current_mean", y="accel_z_std", hue=target_col, style=target_col, data=df, s=150, palette="Set1")
    plt.title("Sensor Fusion: Friction vs. Vibration")
    plt.xlabel("Motor Current (Friction)")
    plt.ylabel("Z-Axis Vibration (Impacts)")
    plt.legend(title="Terrain", bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.tight_layout()
    plt.savefig(f"{output_dir}/04_Sensor_Fusion_Scatter.png", dpi=300)
    plt.close()
    print("Generated: 04_Sensor_Fusion_Scatter.png")

    print("\nAll visualizations have been successfully saved to the 'Images/visualizations' folder!")

if __name__ == "__main__":
    generate_hackathon_visualizations()