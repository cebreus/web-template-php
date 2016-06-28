<?php
    
    header("Content-type: text/html; charset=UTF-8", true);
    setlocale(LC_ALL, 'czech');
    
    require_once('init.inc');
    require_once('functions.inc');
    
    // REMOVE WHITE CHARS IN WHOLE OUTPUT FILE
    if (isProduction($projectDomain))
    {
        ob_start("sanitize_output");
    }
    // BEAUTIFY HTML CODE IN WHOLE OUTPUT FILE
    else
    {
        ob_start("beautify_output");
    }

?><!doctype html>
<html lang="cs" class="no-js">
    <head>
        <meta charset="utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <meta name="skype_toolbar" content="skype_toolbar_parser_compatible">
        <title></title>
        <meta name="description" content="">
        <meta name="keywords" content="">
        <meta name="author" content="">
        <meta name="robots" content="index, follow">
        <link rel="stylesheet" type="text/css" href="//fonts.googleapis.com/css?family=Roboto:500,400,400italic&ampsubset=latin,latin-ext">
        <?php
            if (!isProduction($projectDomain)) {
                showHeadCssLinks(genCssFiles(array('show-grid')));
            }
            
            showHeadCssLinks(genCssFiles(array('index-base','index-mask','index-utils')));
            
            echo '        <script src="//ajax.googleapis.com/ajax/libs/jquery/2.2.0/jquery.min.js"></script>'.PHP_EOL;
            
            showScriptLinks(array('assets/modernizr/modernizr-custom.min.js'), 'head', true);
        ?>
        <meta name="theme-color" content="">
        <?php
            showJsonFromFile('assets/json-head-data.json');
        ?>
    </head>
    <body>
        <header></header>
        <main class="container">
            <article>
                <h1>Heading 1</h1>
            </article>
        </main>
        <footer></footer>
        <?php
            showAalyticsTracking('');
        ?>
    </body>
</html>