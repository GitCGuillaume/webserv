<pre>
<?php
print_r($_POST);

//var_dump($_SERVER);
var_dump($_FILES);
foreach ($_FILES['upload_files']['name'] as $key => $val)
{
        echo "aaaa\n";
	$tmp = $_FILES["upload_files"]["tmp_name"][$key];
        move_uploaded_file($tmp, getcwd().'/../upload/'.$val);
}

?>
</pre>