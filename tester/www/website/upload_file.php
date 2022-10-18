<?php

#$countFiles = count($_FILES['upload_files']['name']);
foreach ($_FILES['upload_files']['name'] as $key => $val)
{
	$tmp = $_FILES["upload_files"]["tmp_name"][$key];
        move_uploaded_file($tmp, getcwd().'/upload/'.$val);
}

?>
