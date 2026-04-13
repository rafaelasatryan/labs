import numpy as np
import matplotlib.pyplot as plt

# Исходные данные
X = np.array([22, 25, 30, 40, 30, 39, 30, 39, 26, 23], dtype=float)
Y = np.array([2200, 1600, 700, 400, 1100, 800, 700, 1100, 1100, 1800], dtype=float)

# -------------------------------
# 1. Квадратичная регрессия
# Y = a*X^2 + b*X + c
# -------------------------------
coeffs = np.polyfit(X, Y, 2)
a, b, c = coeffs

print("Коэффициенты квадратичной регрессии:")
print(f"a = {a:.6f}")
print(f"b = {b:.6f}")
print(f"c = {c:.6f}")

# Расчетные значения в исходных точках
Y_calc = a * X**2 + b * X + c

# -------------------------------
# 2. Ошибки
# -------------------------------
errors = Y - Y_calc
abs_errors = np.abs(errors)
sq_errors = errors**2

max_error = np.max(abs_errors)
rmse = np.sqrt(np.mean(sq_errors))
sse = np.sum(sq_errors)

print("\nУравнение регрессии:")
print(f"Y = {a:.6f}*X^2 + ({b:.6f})*X + ({c:.6f})")

print("\nОшибки:")
print(f"Максимальная ошибка = {max_error:.6f}")
print(f"Среднеквадратичная ошибка = {rmse:.6f}")
print(f"Сумма квадратов ошибок = {sse:.6f}")

# -------------------------------
# 3. Таблица результатов
# -------------------------------
print("\nТаблица:")
print("   X        Y       Yрасч      Ошибка      Ошибка^2     |Ошибка|")
for i in range(len(X)):
    print(f"{X[i]:5.0f}   {Y[i]:7.3f}   {Y_calc[i]:9.3f}   {errors[i]:9.3f}   {sq_errors[i]:11.3f}   {abs_errors[i]:9.3f}")

# -------------------------------
# 4. График
# -------------------------------
X_plot = np.linspace(X.min(), X.max(), 300)
Y_plot = a * X_plot**2 + b * X_plot + c

plt.figure(figsize=(9, 5))
plt.scatter(X, Y, label='Исходные данные')
plt.plot(X_plot, Y_plot, label='Квадратичная регрессия')

plt.title('Квадратичная регрессия зависимости Y от X')
plt.xlabel('X (%)')
plt.ylabel('Y (мл)')
plt.grid(True)
plt.legend()
plt.show()