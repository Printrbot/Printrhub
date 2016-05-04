<?php

$f = file_get_contents("menus.json");
$j = json_decode($f, true);
$o = json_encode($j);

print(addSlashes($o));
