<?php

/*
 * GOOGLE ANALYTICK TRACKING CODE
 */

function showAalyticsTracking($id = '')
{
    if (!empty($id))
    {
        echo "<script>";
        echo "(function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){(i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)})(window,document,'script','//www.google-analytics.com/analytics.js','ga');";
        echo "ga('create', '".$id."', 'auto');";
        echo "ga('send', 'pageview');";
        echo "</script>";
    }
}



/*
 * TEST JESTLI JSEM NA PRODUKČNÍM SERVERU
 * note: nutno zkontrolovat nastavení proměnné $projectDomain, aby odpovídala ostré doméně
 */

function isProduction($projectDomain) 
{
    return $_SERVER['HTTP_HOST'] == $projectDomain;
}


/*
 * SANITIZE HTML OUTPUT
 */

function sanitize_output($buffer)
{
    // BASIC SANITIZE BASIC STRING REPLACE (true) × REPLACE ON AROUND ELEMENTS (false)
    $basic = false;
    
    if ($basic)
    {
        $search = array(
            '/\>[^\S ]+/s',     // strip whitespaces after tags, except space
            '/[^\S ]+\</s',     // strip whitespaces before tags, except space
            '/(\s)+/s',         // shorten multiple whitespace sequences
            '/<!--(.*)-->/Uis',  // remove HTML comments
        );
        $replace = array(
            '>',
            '<',
            '\\1',
            '',
        );
        
        $buffer = preg_replace($search, $replace, $buffer);
    }
    else
    {
        $baseElements = Array('html', 'head', 'body', 'title', 'meta', 'link', 'script');
        $blockElements = Array('article', 'aside', 'details', 'figcaption', 'figure', 'footer', 'header', 'hgroup', 'main', 'menu', 'nav', 'section', 'summary', 'audio', 'canvas', 'progress', 'video');
        $contentElements = Array('html', 'head', 'body', 'title', 'meta', 'link', 'script', 'div', 'p', 'ul', 'ol', 'h[\d]', 'option', 'optgroup');
        
        foreach($baseElements as $to_replace) {
            $buffer = preg_replace('/\s+(<[\/]?'.$to_replace.')/', '\\1', $buffer);
        }
        foreach($blockElements as $to_replace) {
            $buffer = preg_replace('/\s+(<[\/]?'.$to_replace.')/', '\\1', $buffer);
        }
        foreach($contentElements as $to_replace) {
            $buffer = preg_replace('/\s+(<[\/]?'.$to_replace.')/', '\\1', $buffer);
        }
        
        $buffer = preg_replace('/<!--(.*)-->/Uis', '', $buffer);
    }
    
    return $buffer;
}



/*
 * SANITIZE JSON
 * note: read content of file, remove unescessary white characters, encode to UTF-8, write into tag
 */

function showJsonFromFile($file = '')
{
    $data = autoUTF(file_get_contents($file));
    $data = preg_replace('/[ ]{2,}|[\n\r\t]/', '', $data);
    $data = preg_replace('/([\'\"])[ ]{0,}:[ ]{0,}([\'\"\[])/', "\\1:\\2", $data);
    
    echo '<script type="application/ld+json">'.$data.'</script>'.PHP_EOL;
}



/*
 * BEAUTIFY HTML CODE
 */

function beautify_output($buffer)
{
    include ("vendor/dindent/Indenter.php");
    $indenter = new \Gajus\Dindent\Indenter();
    $buffer = $indenter->indent($buffer);
    $buffer = preg_replace('/(<h\d[^>]*>)[\s]+(.*?)[\s]+(<\/h\d>)/u', '\\1\\2\\3', $buffer);
    
    return $buffer;
}



/*
 * ENCODING TO UTF-8
 */

function autoUTF($s)
{
    // detect UTF-8
    if (preg_match('#[\x80-\x{1FF}\x{2000}-\x{3FFF}]#u', $s)) {
        return $s;
    }
    
    // detect WINDOWS-1250
    if (preg_match('#[\x7F-\x9F\xBC]#', $s)) {
        return iconv('WINDOWS-1250', 'UTF-8', $s);
    }
    
    // assume ISO-8859-2
    return iconv('ISO-8859-2', 'UTF-8', $s);
}