<?php
    
    header('Content-type: text/html; charset=UTF-8', true);
    setlocale(LC_ALL, 'czech');
    
    require_once 'init.inc';
    require_once 'functions.inc';
    
    (isProduction($siteDomain)) ? ob_start('sanitizeOutput') : ob_start('beautifyOutput');
    
    $pageName = 'Úvodní stránka';
    $pageDescription = 'Popis stránky';

?><!doctype html>
<html lang="cs" class="no-js">
    <head>
        <meta charset="utf-8">
        <meta name="robots" content="index, follow">
        <meta name="viewport" content="width=device-width, initial-scale=1.0, minimum-scale=1.0">
        <meta name="skype_toolbar" content="skype_toolbar_parser_compatible">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <title><?php echo (isset($pageName) || !empty($pageName)) ? $pageName . ' | ' : ''; echo html_entity_decode($siteName); ?></title>
        <meta name="description" content="<?php echo html_entity_decode($siteDescription); ?>">
        <meta name="author" content="<?php echo html_entity_decode($siteAuthor); ?>">
        
        <meta property="og:type" content="website">
        <meta property="og:url" content="<?php echo (isset($_SERVER['HTTPS']) ? "https" : "http") . "://$_SERVER[HTTP_HOST]$_SERVER[REQUEST_URI]"; ?>">
        <meta property="og:site_name" content="<?php echo html_entity_decode($siteName); ?>">
        <meta property="og:title" content="<?php echo html_entity_decode($pageName); ?>">
        <meta property="og:description" content="<?php echo html_entity_decode($pageDescription); ?>">
        
        <meta name="theme-color" content="<?php echo $pageThemeColor; ?>">
        <?php // Add to homescreen for Chrome on Android ?>
        <meta name="mobile-web-app-capable" content="yes">
        <link rel="icon" sizes="192x192" href="<?php echo $pageIconPath; ?>/ico-192.png">
        <?php // Add to homescreen for Safari on iOS ?>
        <meta name="apple-mobile-web-app-capable" content="yes">
        <meta name="apple-mobile-web-app-status-bar-style" content="black">
        <meta name="apple-mobile-web-app-title" content="<?php echo html_entity_decode($siteName); ?>">
        <link rel="apple-touch-icon-precomposed" href="<?php echo $pageIconPath; ?>/ico-120.png">
        <?php // Tile icon for Win8 (144x144 + tile color) ?>
        <meta name="msapplication-TileImage" content="<?php echo $pageIconPath; ?>/ico-144.png">
        <meta name="msapplication-TileColor" content="<?php echo $pageThemeColor; ?>">
        <link rel="shortcut icon" href="<?php echo $pageIconPath; ?>/ico-48.png">

        <link rel="stylesheet" href="<?php echo isProduction($GLOBALS['siteDomain']) ? '/build/css/static.min.css' : $skinPath.'/index.css'; ?>">
        <link rel="stylesheet" href="build/css/jquery.fancybox.min.css">

        <?php
            echo getScriptLinks([
                'assets/modernizr-custom.min.js',
                'vendor/jquery-3.2.1/jquery-3.2.1.min.js',
                'vendor/tether-1.3.3/dist/js/tether.min.js',
                'vendor/bootstrap-4.0.0-alpha.6/dist/js/bootstrap.min.js',
                'vendor/fancybox-3.0/dist/jquery.fancybox.min.js',
                'vendor/lazysizes-4.0.0-rc2/lazysizes.min.js',
                'vendor/mediaCheck-0.4.6/mediaCheck.min.js',
                'assets/page-all.js',
            ], 'build/js/head.js');
            
            $jsonHeadData = 'build/js/json-head-data.json';
            if (fileIsIncludable($jsonHeadData)) {
                echo '    <script type="application/ld+json">' . file_get_contents($jsonHeadData) . '</script>';
            }
        ?>
    </head>
    <body class="is-responsive">
        <header></header>
        <main class="container">
            <article>
                <h1>Heading 1</h1>
                <ul>
                    <li><a href="https://www.cebre.us" target="_blank">externí odkaz</a> v textu</li>
                    <li><a href="mailto:aaa@aaa.com">e-mail</a> na osobu nebo firmu</li>
                    <li><a href="tel:0420123456789">telefon</a> s možností vytočení</li>
                    <li><a href="aa.pdf">soubor ke stažení</a> [PDF; 1,0 MB]</li>
                </ul>
            </article>
            <aside class="main-content__gallery row">
                <div class="col-sm-3"><a href="assets/detail.jpg" data-fancybox="gallery"><img class="img-responsive img-thumbnail" src="assets/preview.jpg" alt="sample image"></a></div>
                <div class="col-sm-3"><a href="assets/detail.jpg" data-fancybox="gallery"><img class="img-responsive img-thumbnail" src="assets/preview.jpg" alt="sample image"></a></div>
                <div class="col-sm-3"><a href="assets/detail.jpg" data-fancybox="gallery"><img class="img-responsive img-thumbnail" src="assets/preview.jpg" alt="sample image"></a></div>
                <div class="col-sm-3"><a href="assets/detail.jpg" data-fancybox="gallery"><img class="img-responsive img-thumbnail" src="assets/preview.jpg" alt="sample image"></a></div>
            </aside>
        </main>
        <footer></footer>
        <?php echo getGA(''); ?>
    </body>
</html>