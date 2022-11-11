<!doctype html>
<html>
    <head>
        <title>CGI PHP POST</title>
        <meta charset="utf-8">
        <link href="42.png" rel="icon" type="image/png">
        <link href="42.png" rel="shortcut icon" type="image/png">
        <link rel="stylesheet" href="../../stylesheet.css">
        <meta name="author" content="gchopin">
        <meta name="viewport" content="width=device-width, initial-scale=1">
    </head>
    <article>
        <h1>POST</h1>
        <p class="answer">
        <?php
            if (isset($_POST["test1"]))
                echo "Name: ".$_POST["test1"]." ";
            if (isset($_POST["test2"]))
                echo "Surname: ".$_POST["test2"]." ";
        ?>
        </p>
    </article>
</html>