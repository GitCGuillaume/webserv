<?php
echo '<pre>';
echo '<p>_SERVER:</p>';
var_dump($_SERVER);
echo '<p>_FILES</p>';
var_dump($_FILES);
foreach ($_FILES['upload_files']['name'] as $key => $val)
{
	$tmp = $_FILES["upload_files"]["tmp_name"][$key];
        move_uploaded_file($tmp, getcwd().'/../../upload/'.$val);
}
echo '</pre>';
?>
</pre>