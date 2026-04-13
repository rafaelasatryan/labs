import numpy as np

# Исходные данные
Z = np.array([26, 33, 24, 29, 42, 24, 52, 56, 26, 45, 27, 54, 34, 48, 45], dtype=float)
X = np.array([37, 33, 15, 36, 26, 24, 15, 33, 44, 34, 63, 8, 44, 43, 31], dtype=float)
Y = np.array([39, 40, 35, 48, 53, 42, 54, 54, 50, 53, 46, 50, 43, 55, 51], dtype=float)

# Формируем матрицу признаков: [1, X, Y]
A = np.column_stack((np.ones(len(X)), X, Y))

# Находим коэффициенты МНК
coeffs, residuals, rank, s = np.linalg.lstsq(A, Z, rcond=None)
a0, a1, a2 = coeffs

# Прогнозные значения
Z_hat = A @ coeffs

# Ошибки
errors = Z - Z_hat
abs_errors = np.abs(errors)
sq_errors = errors ** 2

max_error = np.max(abs_errors)
rmse = np.sqrt(np.mean(sq_errors))

# Вывод
print("ЛИНЕЙНАЯ МОДЕЛЬ")
print(f"Z = {a0:.6f} + ({a1:.6f})*X + ({a2:.6f})*Y")
print()

print("Таблица результатов:")
print("№\tZ\tZ^ \t\tОшибка\t\t|Ошибка|")
for i in range(len(Z)):
    print(f"{i+1}\t{Z[i]:.0f}\t{Z_hat[i]:.6f}\t{errors[i]:.6f}\t{abs_errors[i]:.6f}")

print()
print(f"Максимальная ошибка: {max_error:.6f}")
print(f"Среднеквадратичная ошибка: {rmse:.6f}")