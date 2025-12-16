<?php
/**
 * @file header.php
 * @brief Общий HTML-шаблон (шапка) для страниц приложения.
 *
 * Файл содержит:
 * - инициализацию PHP-сессии (если ещё не начата);
 * - HTML-разметку <head> и начало <body>;
 * - навигационное меню с кнопками авторизации/выхода.
 *
 * Используется через `include 'header.php';` на страницах приложения.
 */

// Запускаем сессию только один раз.
if (session_status() === PHP_SESSION_NONE) {
    session_start();
}
?>
<!DOCTYPE html>
<html lang="ru">
<head>
    <meta charset="UTF-8">
    <title>Адресная книга</title>

    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.3/dist/css/bootstrap.min.css" rel="stylesheet">
    <link rel="icon" href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAQAAADZc7J/AAABF0lEQVRIx+2UrY7CQBSFvyVsQkZsCNnHWMEDoHmGTWqx2Hocr1BH8BsUBoGjmBVrmDeA1V1DAuKu6A/T6bQZwBFuRdPT3q/ntDMX8lrwjm8FBFXxyIG+R7MiQhi5AMKJkFZjex+N1AMEaYwSkGRPNQCkJkpqXXwAwomJFSW37gmwo1ysXwHIo5StXwVIo2iH7g2oPwpAizvrCYC2df3L3Kvvu+43xvdG6N4eJnVwRAFUlu+k2BVlXdsOOgwA+LL0TXbeWfrMNVDSmprv4dWYnRc9NnRjLwSF3ZgDmijb2CEKUEzRJMSMzXYTkDB0fKMQIWpaN/ZEUs5xVpmYLwagU7rzx5ote3p88MlboZ9ZsmXFz3MePBDgH8Hl9DF8GD4QAAAAAElFTkSuQmCC">
</head>
<body>

<nav class="navbar navbar-expand-lg navbar-dark bg-dark mb-4">
    <div class="container">
        <a class="navbar-brand" href="users.php">Адресная книга</a>

        <?php if (!empty($_SESSION['user_id'])): ?>
            <div>
                <a class="btn btn-outline-light btn-sm" href="own_profile_logic.php">Мой профиль</a>
                <a class="btn btn-outline-warning btn-sm" href="logout.php">Выход</a>
            </div>
        <?php else: ?>
            <div>
                <a class="btn btn-outline-light btn-sm" href="login.php">Вход</a>
                <a class="btn btn-outline-info btn-sm" href="signup.php">Регистрация</a>
            </div>
        <?php endif; ?>
    </div>
</nav>

<div class="container">
