<?php
require_once('inc/common.php');
require_once('inc/geoip.php');

$db = db();

ui_start('Statistics');
ui_content_start();

function format_count($count)
{
   global $total;
   if($total == 0 && $count == 0)
      $total = 1;
   return $count.' ('.round(($count / $total) * 100, 2).'%)';  
}

$query = $db->query('SELECT COUNT(*) FROM bots');
$totalBots = $query->fetchColumn();

$total = $totalBots;

if($total == 0)
   echo('<div class="error">Database is empty</div>');
else
{
?>
<style>
.content
{
   text-align: center;
   font-size: 0;
}

.table
{
   width: 500px;
   font-size: 12px;
}

.box
{
   font-size: 12px;
}

.left
{
   text-align: left;
   display: inline-block;
   vertical-align: top;
}
</style>
<div class="left" style="margin-right: 10px;">
<?php
   $query = $db->prepare('SELECT COUNT(*) FROM bots WHERE last_seen > ?');
   $query->bindValue(1, time() - $CONF_TIMEOUT_OFFLINE, PDO::PARAM_INT);
   $query->execute();
   $online = (int) $query->fetchColumn();
   $offline = $total - $online;

   $query = $db->prepare('SELECT COUNT(*) FROM bots WHERE last_seen < ?');
   $query->bindValue(1, time() - $CONF_TIMEOUT_DEAD, PDO::PARAM_INT);
   $query->execute();
   $dead = $query->fetchColumn();

   $query = $db->prepare('SELECT COUNT(*) FROM bots WHERE last_seen > ?');
   $query->bindValue(1, time() - 60 * 60 * 24, PDO::PARAM_INT);
   $query->execute();
   $online24h = (int) $query->fetchColumn();
?>
  <div class="box margin-bottom">
    <div>Amount</div>
    <table class="table">
      <tr><td>Total:</td><td><?php echo($total); ?></td></tr>
      <tr><td>Online:</td><td><?php echo(format_count($online)); ?></td></tr>
      <tr><td>Offline:</td><td><?php echo(format_count($offline)); ?></td></tr>
      <tr><td>Bots seen since 24h:</td><td><?php echo(format_count($online24h)); ?></td></tr>
      <tr><td>Dead:</td><td><?php echo(format_count($dead)); ?></td></tr>  
    </table>
  </div>
<?php
   $query = $db->query('SELECT COUNT(*) FROM bots WHERE arch = 1');
   $x64 = $query->fetchColumn();

   $os = array();
   $query = $db->query('SELECT os FROM bots');
   $rows = $query->fetchAll();
   foreach($rows as $row)
   {
      $osName = get_os($row['os']);
      if(isset($os[$osName]))
         ++$os[$osName];
      else
         $os[$osName] = 1;
   }
   arsort($os);
?>
  <div class="box margin-bottom">
    <div>Computer Info</div>
    <table class="table margin-bottom">
<?php
   foreach($os as $key => $value)
      echo('<tr><td>'.$key.':</td><td>'.format_count($value).'</td></tr>');
?>
      <tr class="line"><td class="line">x64:</td><td><?php echo(format_count($x64)); ?></td></tr>
      <tr><td>x86:</td><td><?php echo(format_count($total - $x64)); ?></td></tr>
    </table>
  </div>
  <div class="box">
    <div>Countries</div>
    <table class="table">
<?php
   $total = $totalBots;
   $query = $db->query('SELECT DISTINCT country, COUNT(*) as num FROM bots GROUP BY country ORDER BY num DESC');
   $rows = $query->fetchAll();
   $geoip = new GeoIP();
   foreach($rows as $row)
   {
      echo('<tr><td>'.$row['country'].' <em>('.$geoip->GEOIP_COUNTRY_NAMES[$geoip->GEOIP_COUNTRY_CODE_TO_NUMBER[$row['country']]].')</em>:</td><td>'.format_count($row['num']).'</td></tr>');
   }
?>
    </table>
  </div>
</div>
<?php
}
ui_content_end();
ui_end();
?>