<?php
require_once('inc/config.php');
require_once('inc/utils.php');
require_once('inc/ui.php');

session_start();
ui_start('Login');

if(isset($_SESSION['auth']))
{
   header('location: index.php');
   exit();
}
?>
<div class="box margin-bottom center" style="width: 250px;">
   <div>Login</div>
   <?php
   if (isset($_POST['login']))
   {
      $username = $_POST['username'];
      $password = $_POST['password'];
   
      if ($username == $CONF_PANEL_USER && hash_pass($password) == $CONF_PANEL_PASS)
      {
         $_SESSION['auth'] = true;
         $_SESSION['time'] = (string)microtime(true);
         header('location: index.php');
         exit();
      }
      else
         echo('<div class="error">Invalid username or password!</div>');
   }
   ?>
      <form method="POST">
         <table>
            <tr><td>Username:</td>
            <td><input type="text" class="input" name="username"></td></tr>
            <tr><td>Password:</td>
            <td><input input type="password" class="input" name="password"></td></tr>
            <td>
               <input type="submit" class="btn" value="Login" name="login">  
            </td> 
         </table>
   </form>
</div>
<?php
ui_end();
?>