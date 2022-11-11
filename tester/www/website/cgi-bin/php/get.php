<!doctype html>
<html>
    <head>
        <title>CGI PHP GET</title>
        <meta charset="utf-8">
        <link href="42.png" rel="icon" type="image/png">
        <link href="42.png" rel="shortcut icon" type="image/png">
        <link rel="stylesheet" href="../../stylesheet.css">
        <meta name="author" content="gchopin">
        <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>
    <article>
        <h1>GET</h1>
        <p class="answer">
        <?php
            if (isset($_GET["test1"]))
                echo "Name: ".$_GET["test1"]." ";
            if (isset($_GET["test2"]))
                echo "Surname: ".$_GET["test2"]." ";
        ?>
        </p>
    </article>
</html>