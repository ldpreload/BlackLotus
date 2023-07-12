<?php
    require_once('inc/common.php');

    $_SESSION = array();

    $session = session_get_cookie_params();

    setcookie(session_name(), '', time() - 4200, $session["path"], $session["domain"], $session["secure"], $session["httponly"]);

    session_destroy();
    header('Location: login.php');
?>