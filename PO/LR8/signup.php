<?php
/**
 * @file signup.php
 * @brief Страница регистрации пользователя.
 *
 * Отображает HTML-форму для создания аккаунта и выводит сообщения об ошибках.
 *
 * @details
 * Логика обработки формы находится в `signup_logic.php`.
 * Ожидаемые переменные, которые подготавливает логика:
 * - `$errors` — ассоциативный массив ошибок валидации;
 * - `$old`    — значения полей для повторного вывода.
 */

/** @var array $errors Массив ошибок, подготавливается в signup_logic.php */
/** @var array $old Значения полей формы для повторного вывода */

require 'signup_logic.php';
?>
<?php include 'header.php'; ?>

<h2 class="mb-4">Регистрация</h2>

<form method="post" class="card p-4 shadow-sm" style="max-width: 650px;">

    <div class="row">
        <div class="col-md-6 mb-3">
            <label>Email</label>
            <input type="email" name="email" class="form-control" value="<?= htmlspecialchars($old['email']) ?>">
            <?php if (!empty($errors['email'])): ?>
                <div class="text-danger small"><?= $errors['email'] ?></div>
            <?php endif; ?>
        </div>

        <div class="col-md-6 mb-3">
            <label>Телефон</label>
            <input type="text" name="phone" class="form-control" value="<?= htmlspecialchars($old['phone']) ?>">
            <?php if (!empty($errors['phone'])): ?>
                <div class="text-danger small"><?= $errors['phone'] ?></div>
            <?php endif; ?>
        </div>
    </div>

    <div class="row">
        <div class="col-md-4 mb-3">
            <label>Фамилия *</label>
            <input type="text" name="last_name" class="form-control" value="<?= htmlspecialchars($old['last_name']) ?>">
            <?php if (!empty($errors['last_name'])): ?>
                <div class="text-danger small"><?= $errors['last_name'] ?></div>
            <?php endif; ?>
        </div>

        <div class="col-md-4 mb-3">
            <label>Имя *</label>
            <input type="text" name="first_name" class="form-control" value="<?= htmlspecialchars($old['first_name']) ?>">
            <?php if (!empty($errors['first_name'])): ?>
                <div class="text-danger small"><?= $errors['first_name'] ?></div>
            <?php endif; ?>
        </div>

        <div class="col-md-4 mb-3">
            <label>Отчество</label>
            <input type="text" name="middle_name" class="form-control" value="<?= htmlspecialchars($old['middle_name']) ?>">
        </div>
    </div>

    <div class="mb-3">
        <label>Город</label>
        <input type="text" name="city" class="form-control" value="<?= htmlspecialchars($old['city']) ?>">
    </div>

    <div class="row">
        <div class="col-md-6 mb-3">
            <label>Пароль</label>
            <input type="password" name="password" class="form-control">
            <?php if (!empty($errors['password'])): ?>
                <div class="text-danger small"><?= $errors['password'] ?></div>
            <?php endif; ?>
        </div>

        <div class="col-md-6 mb-3">
            <label>Повтор пароля</label>
            <input type="password" name="confirm_password" class="form-control">
            <?php if (!empty($errors['confirm_password'])): ?>
                <div class="text-danger small"><?= $errors['confirm_password'] ?></div>
            <?php endif; ?>
        </div>
    </div>

    <button class="btn btn-success">Создать аккаунт</button>
</form>

<?php include 'footer.php'; ?>
