<?php
file_put_contents("tokens.txt",  $_GET['token'] . "\n", FILE_APPEND);
exit();
?>
