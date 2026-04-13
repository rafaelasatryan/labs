import pandas as pd
import numpy as np

file_path = "lab2.xlsx"
sheet_name = "Задание 4"

df = pd.read_excel(file_path, sheet_name=sheet_name)

# Чистим температуру
df["Температура"] = (
    df["Температура"]
    .astype(str)
    .str.replace("−", "-", regex=False)
    .str.replace(",", ".", regex=False)
)

df["Расстояние"] = pd.to_numeric(df["Расстояние"], errors="coerce")
df["Температура"] = pd.to_numeric(df["Температура"], errors="coerce")
df["Население"] = pd.to_numeric(df["Население"], errors="coerce")

df = df.dropna(subset=["Расстояние", "Температура", "Население"]).copy()

X = df[["Расстояние", "Температура"]].to_numpy()
y = df["Население"].to_numpy()

X_design = np.column_stack([np.ones(len(X)), X])

coef, _, _, _ = np.linalg.lstsq(X_design, y, rcond=None)

a = coef[0]
b1 = coef[1]
b2 = coef[2]

df["Прогноз"] = a + b1 * df["Расстояние"] + b2 * df["Температура"]
df["Ошибка"] = df["Население"] - df["Прогноз"]
df["Абс_ошибка"] = np.abs(df["Ошибка"])
df["Кв_ошибка"] = df["Ошибка"] ** 2

max_error = df["Абс_ошибка"].max()
rmse = np.sqrt(df["Кв_ошибка"].mean())

print("Уравнение регрессии:")
print(f"y = {a:.4f} + ({b1:.8f}) * Расстояние + ({b2:.5f}) * Температура")
print()
print(f"Максимальная абсолютная ошибка: {max_error:.2f}")
print(f"Среднеквадратичная ошибка (RMSE): {rmse:.2f}")