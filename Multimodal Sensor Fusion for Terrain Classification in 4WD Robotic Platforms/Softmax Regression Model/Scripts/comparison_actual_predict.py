import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.metrics import confusion_matrix

def plot_confusion_heatmap(csv_path):
    df = pd.read_csv(csv_path)
    df_pred = pd.read_csv('Predictions/predictions.csv')
    labels = ["Grass", "Gravel", "Rough", "Smooth", "Smooth Inclined"]
    
    y_true = df['Label'] 
    y_pred = df_pred['Predicted_Label']
    
    # Calculate Matrix
    cm = confusion_matrix(y_true, y_pred=y_pred) 
    
    plt.figure(figsize=(10, 8))
    sns.heatmap(cm, annot=True, fmt='d', cmap='Blues', 
                xticklabels=labels, yticklabels=labels)
    
    plt.title("Terrain Classification Confusion Matrix")
    plt.ylabel("Actual Terrain")
    plt.xlabel("Predicted Terrain")
    
    plt.tight_layout()
    plt.savefig("Images/visualizations/05_Confusion_Matrix.png", dpi=300)
    print("Confusion Matrix saved to 'Images/visualizations/05_Confusion_Matrix.png'")
    plt.show()

plot_confusion_heatmap("Predictions/True.csv")