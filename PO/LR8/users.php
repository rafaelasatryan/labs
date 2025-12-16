<?php
/**
 * @file users.php
 * @brief Страница со списком пользователей и формой фильтрации.
 *
 * Скрипт:
 * - проверяет авторизацию пользователя (наличие `$_SESSION['user_id']`);
 * - подключает базу данных и функцию поиска пользователей;
 * - формирует массив фильтров из GET-параметров;
 * - запрашивает список пользователей и выводит его в HTML-таблице.
 *
 * @see search_user.php Файл с функцией поиска пользователей.
 */

session_start();
if (!isset($_SESSION['user_id'])) {
    header("Location: login.php");
    exit;
}

require_once 'db_connect.php';
require_once 'search_user.php';

/**
 * @var array $filters Массив фильтров поиска (значения берутся из $_GET).
 */
$filters = [
    'last_name'  => $_GET['last_name']  ?? '',
    'first_name' => $_GET['first_name'] ?? '',
    'city'       => $_GET['city']       ?? '',
    'phone'      => $_GET['phone']      ?? '',
];

/**
 * @var array $users Результат поиска пользователей.
 */
$users = searchUsers($pdo, $filters);

include 'header.php';
?>

<h2 class="mb-4">Список пользователей</h2>

<div class="card p-3 mb-4">
    <form method="get" class="row g-3">
        <div class="col-md-3">
            <input type="text" name="last_name" class="form-control" placeholder="Фамилия" value="<?= htmlspecialchars($filters['last_name']) ?>">
        </div>

        <div class="col-md-3">
            <input type="text" name="first_name" class="form-control" placeholder="Имя" value="<?= htmlspecialchars($filters['first_name']) ?>">
        </div>

        <div class="col-md-3">
            <input type="text" name="city" class="form-control" placeholder="Город" value="<?= htmlspecialchars($filters['city']) ?>">
        </div>

        <div class="col-md-3">
            <input type="text" name="phone" class="form-control" placeholder="Телефон" value="<?= htmlspecialchars($filters['phone']) ?>">
        </div>

        <div class="col-12">
            <button class="btn btn-primary">Поиск</button>
            <a href="users.php" class="btn btn-secondary">Сброс</a>
        </div>
    </form>
</div>

<table class="table table-striped table-hover">
    <thead>
        <tr>
            <th>Фамилия</th>
            <th>Имя</th>
            <th>Телефон</th>
            <th>Город</th>
            <th></th>
        </tr>
    </thead>

    <tbody>
    <?php foreach ($users as $u): ?>
        <tr>
            <td><?= htmlspecialchars($u['last_name']) ?></td>
            <td><?= htmlspecialchars($u['first_name']) ?></td>
            <td><?= htmlspecialchars($u['phone'] ?: '—') ?></td>
            <td><?= htmlspecialchars($u['city'] ?: '—') ?></td>
            <td>
                <a href="user_view_logic.php?id=<?= $u['id'] ?>" class="btn btn-sm btn-outline-info">Просмотр</a>
            </td>
        </tr>
    <?php endforeach; ?>
    </tbody>
</table>

<?php include 'footer.php'; ?>
