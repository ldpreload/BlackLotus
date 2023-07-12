<?php

session_start();

if(!isset($_SESSION['auth']))
{
   header('location: login.php');
   exit();
}

require_once('config.php');
require_once('const.php');
require_once('ui.php');
require_once('utils.php');
require_once('db.php');


?>