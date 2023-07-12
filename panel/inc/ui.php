<?php
function ui_start($title)
{
?>
<!doctype HTML>
<html>
  <head>
    <title>NzT - <?php echo($title); ?></title>
    <meta charset="UTF-8">
    <link rel="stylesheet" type="text/css" href="style/style.css">
      <script>
         function UserConfirm()
         {
            return confirm("Are you sure?");
         }
    </script>
  </head>
  <body>
<?php
}

function ui_echo_nav_tab($link, $text)
{
   echo('<a href="'.$link.'" class="'.(basename($_SERVER['PHP_SELF']) == $link ? 'current' : '').'">'.$text.'</a>');	
}

function ui_content_start()
{
?>
   <div class="nav">
<?php
   ui_echo_nav_tab('index.php',    'Statistics');
   ui_echo_nav_tab('bots.php',     'Bots');
   ui_echo_nav_tab('commands.php',  'Commands');
   ui_echo_nav_tab('settings.php', 'Settings');
?>
   </div>
   <div class="content">
<?php
}

function ui_content_end()
{
?>
   <div style="clear: both;"></div>
   </div>
   <div class="footer">
      NzT
      <?php
         if(isset($_SESSION['auth']))
         {
      ?>
      |
      <a href="logout.php">logout</a>
      <?php
         }
      ?>
   </div>
<?php
}

function ui_end()
{
?>
  </body>
</html>
<?php
}
?>