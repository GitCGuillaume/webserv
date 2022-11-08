<?php
print_r($_GET);
var_dump($_SERVER);
if (isset($_GET["test1"]))
    echo "test1: ".$_GET["test1"]." ";
if (isset($_GET["test2"]))
    echo "test2: ".$_GET["test2"]." ";
if (isset($_GET["test3"]))
    echo "test3: ".$_GET["test3"];
?>