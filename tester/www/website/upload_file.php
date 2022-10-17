<?php

$countFiles = count($_FILES['upload_files']['name'])
for ($i = 0; i < $count; $i++)
{
	print_r($_REQUEST);
	$tmp = $_FILES['upload_files']['name'][$i];
	print("FILE: $tmp");
        file_uploaded_file($_FILES['upload_files']['tmp_name'][$i], 'upload'.$tmp);
}

?>
