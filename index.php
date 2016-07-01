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
<html lang="cs" class="no-js responsive-layout">
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
            
            showScriptLinks(array('assets/modernizr/modernizr-custom.min.js', 'assets/colorbox/colorbox.min.js', 'assets/swipebox/swipebox.min.js'), 'head', true);
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
                <ul>
                    <li><a href="http://domain.com">externí odkaz</a> v textu</li>
                    <li><a href="mailto:aaa@aaa.com">e-mail</a> na osobu nebo firmu</li>
                    <li><a href="tel:0420123456789">telefon</a> s možností vytočení</li>
                    <li><a href="aa.pdf">soubor ke stažení</a> [PDF; 1,0 MB]</li>
                </ul>
            </article>
            <aside class="main-content__gallery row">
                <div class="col-sm-3"><a href="content/picture/sample.png" rel="main-content__gallery"><img class="img-responsive img-thumbnail" src="content/picture/sample.png" alt="sample image"></a></div>
                <div class="col-sm-3"><a href="content/picture/sample.png" rel="main-content__gallery"><img class="img-responsive img-thumbnail" src="content/picture/sample.png" alt="sample image"></a></div>
                <div class="col-sm-3"><a href="content/picture/sample.png" rel="main-content__gallery"><img class="img-responsive img-thumbnail" src="content/picture/sample.png" alt="sample image"></a></div>
                <div class="col-sm-3"><a href="content/picture/sample.png" rel="main-content__gallery"><img class="img-responsive img-thumbnail" src="content/picture/sample.png" alt="sample image"></a></div>
            </aside>
        </main>
        <footer></footer>
        <?php
            showScriptLinks(array('assets/page-all.js'), 'footer', null);
            showAalyticsTracking('');
        ?>
    </body>
</html>