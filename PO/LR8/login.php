<?php
/**
 * @file login.php
 * @brief Страница авторизации пользователя.
 *
 * Отображает HTML-форму для входа и выводит сообщения об ошибках валидации.
 *
 * @details
 * Логика обработки формы находится в `login_logic.php`.
 * Ожидаемые переменные, которые подготавливает логика:
 * - `$errors` — ассоциативный массив ошибок (ключи: email, password, login и т.п.);
 * - `$old`    — значения полей для повторного вывода (например, email).
 */

/** @var array $errors Массив ошибок, подготавливается в login_logic.php */
/** @var array $old Значения полей формы для повторного вывода */

require 'login_logic.php';
?>
<?php include 'header.php'; ?>

<h2 class="mb-4">Вход в аккаунт</h2>

<?php if (!empty($errors['login'])): ?>
    <div class="alert alert-danger"><?= $errors['login'] ?></div>
<?php endif; ?>

<form method="post" class="card p-4 shadow-sm" style="max-width: 450px;">
    <div class="mb-3">
        <label>Email</label>
        <input type="email" name="email" value="<?= htmlspecialchars($old['email']) ?>" class="form-control">
        <?php if (!empty($errors['email'])): ?>
            <div class="text-danger small"><?= $errors['email'] ?></div>
        <?php endif; ?>
    </div>

    <div class="mb-3">
        <label>Пароль</label>
        <input type="password" name="password" class="form-control">
        <?php if (!empty($errors['password'])): ?>
            <div class="text-danger small"><?= $errors['password'] ?></div>
        <?php endif; ?>
    </div>

    <button class="btn btn-primary">Войти</button>
</form>

<?php include 'footer.php'; ?>
