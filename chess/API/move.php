<?php

require_once $_SERVER['DOCUMENT_ROOT'] . "/api/API.php";

$api = new API();
echo $api->Move();