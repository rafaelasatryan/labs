import numpy as np
from scipy.optimize import minimize_scalar

# Исходные данные
t = np.array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10], dtype=float)
U = np.array([100, 75, 55, 40, 30, 20, 15, 10, 10, 5, 5], dtype=float)

U0 = 100.0

# Функция суммы квадратов ошибок
def sse(alpha):
    U_hat = U0 * np.exp(-alpha * t)
    return np.sum((U - U_hat) ** 2)

# Минимизируем сумму квадратов ошибок
result = minimize_scalar(sse, bounds=(0, 2), method='bounded')
alpha = result.x

# Прогнозные значения
U_hat = U0 * np.exp(-alpha * t)

# Ошибки
errors = U - U_hat
abs_errors = np.abs(errors)
sq_errors = errors ** 2

max_error = np.max(abs_errors)
rmse = np.sqrt(np.mean(sq_errors))

# Вывод
print("ЭКСПОНЕНЦИАЛЬНАЯ МОДЕЛЬ")
print(f"U = 100 * exp(-{alpha:.6f} * t)")
print()

print("Таблица результатов:")
print("t\tU\tU^\t\tОшибка\t\t|Ошибка|")
for i in range(len(t)):
    print(f"{int(t[i])}\t{U[i]:.0f}\t{U_hat[i]:.6f}\t{errors[i]:.6f}\t{abs_errors[i]:.6f}")

print()
print(f"alpha = {alpha:.6f}")
print(f"Сумма квадратов ошибок S = {np.sum(sq_errors):.6f}")
print(f"Максимальная ошибка: {max_error:.6f}")
print(f"Среднеквадратичная ошибка: {rmse:.6f}")