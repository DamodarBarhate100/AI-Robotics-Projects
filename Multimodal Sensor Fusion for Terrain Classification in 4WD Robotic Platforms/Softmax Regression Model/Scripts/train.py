import pandas as pd
from sklearn.metrics import accuracy_score, recall_score, precision_score, confusion_matrix
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler 
import joblib
from micromlgen import port


df = pd.read_csv("Softmax Regression Model/Datasets/Final_rover_Data.csv")

X = df.drop('Label', axis=1)
Y = df['Label']

X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.25, random_state=42, shuffle=True, stratify=Y)

scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train) 
X_test_scaled = scaler.transform(X_test)       

softmax_reg = LogisticRegression(solver="lbfgs", max_iter=500)
softmax_reg.fit(X_train_scaled, Y_train)

print("\n Predicting Outputs:")
y_pred = softmax_reg.predict(X_test_scaled)
print(y_pred)

print("\n Evaluation Metrics:")
print(f"Accuracy Score: {accuracy_score(y_pred=y_pred, y_true=Y_test):.4f}")
print(f"Precision: {precision_score(Y_test, y_pred, average='weighted'):.4f}")
print(f"Recall: {recall_score(Y_test, y_pred, average='weighted'):.4f}")
print("\nConfusion Matrix:\n", confusion_matrix(Y_test, y_pred))

joblib.dump(softmax_reg, 'Softmax Regression Model/Model/terrain_model.pkl')
print("Model saved as terrain_model.pkl")

model = joblib.load('Softmax Regression Model/Model/terrain_model.pkl')

cpp_code = port(model)


with open("Softmax Regression Model/Model/terrain_model.h", "w") as file:

    file.write(cpp_code)

print("Header File Saved")


print("\nDone!")


print("\n--- COPY THESE ARRAYS TO YOUR ESP32 C++ CODE ---")

means_str = ", ".join(map(str, scaler.mean_))
scales_str = ", ".join(map(str, scaler.scale_))

print(f"float scaler_means[{len(X.columns)}] = {{{means_str}}};")
print(f"float scaler_scales[{len(X.columns)}] = {{{scales_str}}};")