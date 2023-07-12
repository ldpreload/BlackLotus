<?php
require_once('inc/geoip.php');
function escape_php_string($str)
{
   $str = str_replace("\\", "\\\\", $str);
   $str = str_replace("\"", "\\\"", $str);
   $str = str_replace("\'", "\\\'", $str);
   $str = str_replace("\n", "\\n", $str); 
   $str = str_replace("\t", "\\t", $str);   
   $str = str_replace("\r", "\\r", $str); 
   $str = str_replace("$", "\\$", $str);
   return $str;
}

function hash_pass($pass)
{
   return hash('sha512', $pass);
}

function RC4($pt, $key)
{
	$s = array();
	for ($i=0; $i<256; $i++)
   {
		$s[$i] = $i;
	}
	$j = 0;
	$x;
	for ($i=0; $i<256; $i++)
   {
		$j = ($j + $s[$i] + ord($key[$i % strlen($key)])) % 256;
		$x = $s[$i];
		$s[$i] = $s[$j];
		$s[$j] = $x;
	}
	$i = 0;
	$j = 0;
	$ct = '';
	$y;
	for ($y=0; $y<strlen($pt); $y++)
   {
		$i = ($i + 1) % 256;
		$j = ($j + $s[$i]) % 256;
		$x = $s[$i];
		$s[$i] = $s[$j];
		$s[$j] = $x;
		$ct .= $pt[$y] ^ chr($s[($s[$i] + $s[$j]) % 256]);
	}
	return $ct;
}

function set_headers_txt()
{
   header('X-Content-Type-Options: nosniff'); //stop chrome from downloading the file
   header('Content-Type: text/plain');
}

function echo_file_upload_error()
{
   echo('<div class="error">No file uploaded</div>');
}

function gen_qmarks($arr)
{
   return str_repeat('?, ', count($arr) - 1).'?';
}

function get_pag_vars($total, &$pages, &$page, &$offset)
{
   global $CONST_PAGE_LIMIT;
   $pages = ceil($total / $CONST_PAGE_LIMIT);
   $page = 1;
   if(isset($_GET['page']))
   {
      $page = (int) $_GET['page'];
      if($page > $pages)
         $page = $pages;
      else if($page < 1)
         $page = 1;
   }
   $offset = ($page - 1) * $CONST_PAGE_LIMIT;
}

function get_os($os)
{
   if($os == 0)
      return 'Windows 2000';
   else if ($os == 1)
      return 'Windows XP';
   else if ($os == 2)
      return 'Windows Vista';
   else if ($os == 3)
      return 'Windows 7';
   else if ($os == 4)
      return 'Windows 8';
   else if ($os == 5)
      return 'Windows 8.1';
   else if ($os == 6)
      return 'Windows 10';
   else if ($os == 7)
      return 'Windows 11';
   else
     return 'Unknown';
}

function get_ip()
{
	if (isset($_SERVER["HTTP_X_REAL_IP"]))
	{
		return $_SERVER["HTTP_X_REAL_IP"];
	}
	else if (isset($_SERVER["HTTP_X_FORWARDED_FOR"]))
	{
		return $_SERVER ["HTTP_X_FORWARDED_FOR"];
	}

	return $_SERVER ['REMOTE_ADDR'];
}

function get_country($ip)
{
   $gi      = geoip_open('inc/geoip.dat', GEOIP_STANDARD);
   $country = geoip_country_code_by_addr($gi, $ip);
   geoip_close($gi);
   if (empty($country))
      return '??';
   return $country;
}

function format_time($time)
{
   return date('d/m/Y H:i:s', $time);  
}

function time_since($time)
{
   $time = time() - $time;
   $time = ($time < 1) ? 1 : $time;
   $tokens = array (
      31536000 => 'year',
      2592000  => 'month',
      604800   => 'week',
      86400    => 'day',
      3600     => 'hour',
      60       => 'minute',
      1        => 'second'
   );

   foreach($tokens as $unit => $text)
   {
      if($time < $unit) continue;
      $numberOfUnits = floor($time / $unit);
      return $numberOfUnits.' '.$text.(($numberOfUnits > 1) ? 's' : '').' ago';
   }
}

function is_online($time)
{
   global $CONF_TIMEOUT_OFFLINE;
   return (time() - $time) < $CONF_TIMEOUT_OFFLINE ;
}

function echo_hidden_fields()
{
   $args = func_get_args();
   foreach($_GET as $name => $value)
   {
      if(!in_array($name, $args))
         echo('<input type="hidden" name="'.$name.'" value="'.$value.'">');
   }
}

function echo_pag_form($page, $pages)
{
   $firstDisabled = $page == 1 ? 'disabled' : '';
   echo('<form method="GET" class="margin-top"><a class="btn '.$firstDisabled.'" href="'.add_get_param('page', 1).'">First</a>');
   echo(' <a class="btn '.$firstDisabled.'" href="'.add_get_param('page', $page - 1).'">Previous</a>');
   echo_hidden_fields('page');
   echo(' <input type="text" name="page" placeholder="'.$page.' / '.$pages.'" style="width: 70px; text-align: center;" 
      class="'.($pages == 1 ? 'disabled' : '').' input">');
   $lastDisabled = $page == $pages ? 'disabled' : '';
   echo(' <a class="btn '.$lastDisabled.'" href="'.add_get_param('page', $page + 1).'">Next</a>');
   echo(' <a class="btn '.$lastDisabled.'" href="'.add_get_param('page', $pages).'">Last</a></form>');    
}

function add_get_param($name, $value)
{
   $params = $_GET;
   unset($params[$name]);
   $params[$name] = $value;
   return basename($_SERVER['PHP_SELF']).'?'.http_build_query($params);
}

function action_sec_check()
{
   if($_SERVER['REQUEST_METHOD'] == 'POST')
      $userTime = $_POST['time'];
   else
      $userTime = $_GET['time'];
   if($userTime != $_SESSION['time'])
      exit();
}

function error404()
{
	header('HTTP/1.1 404 Not Found', TRUE, 404);
	echo <<<HTML
	<!DOCTYPE HTML PUBLIC "-//IETF//DTD HTML 2.0//EN">
	<HTML><HEAD>
	<TITLE>404 Not Found</TITLE>
	</HEAD><BODY>
	<H1>Not Found</H1>
	The requested URL $_SERVER[REQUEST_URI] was not found on this server.
	<HR>
	<I>$_SERVER[HTTP_HOST]</I>
	</BODY></HTML>
HTML;
	echo str_repeat ("\r\n", 50);
	exit();
}

function http_redirect($url)
{
    header("Location: $url");
    exit();
}

function http_no_cache()
{
    header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");
    header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
    header("Pragma: no-cache");
    header("Cache-Control: no-cache, must-revalidate");
}


?>