<?php

/*
 * AM I ON PRODUCTION SERVER?
 * note: nutno zkontrolovat nastavení proměnné $projectDomain, aby odpovídala ostré doméně
 * @param: string
 * @return: boolean
 */

function isProduction($projectDomain)
{
    return $_SERVER['HTTP_HOST'] == $projectDomain;
}


/*
 * SANITIZE HTML OUTPUT
 * @param: string
 * @return: string
 */

function sanitizeOutput($buffer)
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
        $baseElements     = Array('html', 'head', 'body', 'title', 'meta', 'link', 'script', 'noscript', 'style');
        $semanticElements = Array('article', 'aside', 'details', 'figcaption', 'figure', 'footer', 'header', 'hgroup', 'main', 'menu', 'nav', 'section', 'summary', 'audio', 'canvas', 'progress', 'video');
        $contentElements  = Array('div', 'table', 'thead', 'tbody', 'tfoot', 'colgroup', 'caption', 'tr', 'th', 'td', 'form', 'label', 'legend', 'textarea', 'button', 'option', 'optgroup', 'blockquote', 'h[\d]', 'p', 'svg', 'object', 'embed', 'ol', 'ul', 'li');
        $inlineElements   = Array('a', 'span', 'strong', 'em', 'b', 'i');
        
        $buffer = preg_replace('/<!--(.*)-->/Uis', '', $buffer);
        
        foreach($baseElements as $to_replace) {
            $buffer = preg_replace('/\s+(<[\/]?'.$to_replace.')/Uis', '\\1', $buffer);
        }
        foreach($semanticElements as $to_replace) {
            $buffer = preg_replace('/\s+(<[\/]?'.$to_replace.')/', '\\1', $buffer);
        }
        foreach($contentElements as $to_replace) {
            // zacatek tagu
            $buffer = preg_replace('/(<'.$to_replace.' *[^\/]*?>)\s+/', '\\1', $buffer);
            // konectagu
            $buffer = preg_replace('/\s+(<[\/]?'.$to_replace.')/', '\\1', $buffer);
            
        }
        foreach($inlineElements as $to_replace) {
            // one space before tag
            $buffer = preg_replace('/\s+(<'.$to_replace.')/i', ' \\1', $buffer);
            // sibling tags
            $buffer = preg_replace('/([A-Z0-9]>)\s+(<[\/]?'.$to_replace.')/i', '\\1 \\2', $buffer);
        }
        
        $buffer = preg_replace('/\s*(<br>|<hr>)\s*/', '\\1', $buffer);
        
    }
    
    return $buffer;
}


/*
 * BEAUTIFY HTML CODE
 * @param: string
 * @return: string
 */

function beautifyOutput($buffer)
{
    include_once("vendor/dindent/Indenter.php");
    $indenter = new \Gajus\Dindent\Indenter();
    $buffer = $indenter->indent($buffer);
    $buffer = preg_replace('/(<h\d[^>]*>)[\s]+(.*?)[\s]+(<\/h\d>)/u', '\\1\\2\\3', $buffer);
    
    return $buffer;
}


/*
 * ENCODING TO UTF-8
 * @param: string
 * @return: string
 */

function autoUTF($data)
{
    // detect UTF-8
    if (preg_match('#[\x80-\x{1FF}\x{2000}-\x{3FFF}]#u', $data)) {
        return $data;
    }
    
    // detect WINDOWS-1250
    if (preg_match('#[\x7F-\x9F\xBC]#', $data)) {
        return iconv('WINDOWS-1250', 'UTF-8', $data);
    }
    
    // assume ISO-8859-2
    return iconv('ISO-8859-2', 'UTF-8', $data);
}


/**
 * Normalize Lines
 *
 * Normalize and convert newline characters to UNIX (by default) or
 * Windows and collapse lines containing two or more adjacent empty lines
 * into one empty line.
 *
 * @param  string  $str     The string of lines to normalize.
 * @param  boolean $windows Set as `true` to convert line endings to
 *                          Windows. The default value is `false`, and  
 *                          UNIX line endings will be used.
 *
 * @return string The normalized lines.
 */

function normalizeLines($str, $windows = true)
{
    $str = str_replace("\r\n", "\n", $str);
    $str = str_replace("\r", "\n", $str);
    $str = preg_replace("/\n{2,}/", "\n\n", $str);
   
    if ($windows === true) {
      $str = str_replace("\n", "\r\n", $str);
    }
     
    return $str;
}


/**
 * BASIC TEXT CORRECTIONS
 * @param: string $input
 * @return: string $output
 */

function basicTextCorrections($input)
{
    $prepositionsNonbreak = Array('K', 'k', 'S', 's', 'V', 'v', 'Z', 'z', 'O', 'o', 'U', 'u', 'A', 'a', 'I', 'i', 'sv.');
    
    foreach($prepositionsNonbreak as $to_replace)
    {
        $output = preg_replace("/([ ,.;:!?>])(".$to_replace.")[ ]+/", "\\1\\2&nbsp;", $input);
    }
    
    // radove cislovky
    $output = preg_replace("/( \d{1,}.) (\D)/", "\\1&nbsp;\\2", $output);
    
    // rimsle cislice
    $output = preg_replace("/(\D) (^M{0,4}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$.)/", "\\1&nbsp;\\2", $output);
    
    // pomlcky
    $output = preg_replace("/ (–|—)/", "&nbsp;\\1", $output);

    // speciality
    $output = preg_replace("/(mimi) (fortunae)/i", "\\1&nbsp;\\2", $output);
    
    return $output;
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


/**
 * FILESIZE FORMATER
 * @param string $path to file
 */

function filesizeFormatted($path)
{
    $size = filesize($path);
    $units = array( 'B', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB');
    $power = $size > 0 ? floor(log($size, 1024)) : 0;
    return number_format($size / pow(1024, $power), 2, ',', ',') . ' ' . $units[$power];
}


/**
 * WRITE FILES
 * @param string $outputPath
 * @param string $outputFileName
 * @param string $content
 */

function writeFile($outputPath, $outputFileName, $content, $filemtime = '')
{
    $outputFile = $outputPath.$outputFileName;
    $inputFileTime = empty($filemtime) ? '' : $filemtime;
    $outputFileTime = is_file($outputFile) ?  filemtime($outputFile) : '';
    
    if (!empty($outputPath) && !is_dir($outputPath)) {
        mkdir($outputPath, 0777, true);
    }
    
    if (strlen($content) <= 0)
    {
        //echo getChip('Nelze generovat soubor  '.$outputPath.'/<strong>'.$outputFileName.'</strong>.<br>Prázná vstupní data!', 'error');
        echo '<script nonce="SomeRandomNonce">console.error("Prázná vstupní data pro soubor https://'.$_SERVER['HTTP_HOST'].'/'.$outputPath.$outputFileName.'.")</script>';
        
        if (is_file($outputFile))
        {
            //echo '<script nonce="SomeRandomNonce">console.info("Soubor https://'.$_SERVER['HTTP_HOST'].'/'.$outputPath.$outputFileName.' je aktuální. Vytvořen: '.date('j.n.Y H:i:s', filemtime($outputFile)).'; velikost: '.filesizeFormatted($outputFile).'")</script>';
        }
    }
    else
    {
        if (!is_file($outputFile) || $outputFileTime < $inputFileTime)
        {
            $fp = fopen($outputFile, "w+"); // read + write, rewrite all content of file
            fputs($fp, $content);
            fclose($fp);
            echo '<script nonce="SomeRandomNonce">console.info("Soubor https://'.$_SERVER['HTTP_HOST'].'/'.$outputPath.$outputFileName.' je vytvořen.")</script>';
            
        }
        elseif(!is_file($outputFile))
        {
            echo '<script nonce="SomeRandomNonce">console.error("Soubor https://'.$_SERVER['HTTP_HOST'].'/'.$outputPath.$outputFileName.' nelze vytvořit!")</script>';
        }
        elseif (!is_writable($outputFile))
        {
            echo '<script nonce="SomeRandomNonce">console.error("Soubor https://'.$_SERVER['HTTP_HOST'].'/'.$outputPath.$outputFileName.' nelze zapisovat!")</script>';
        }
        
        elseif (filesize($outputFile) <= 0)
        {
            echo '<script nonce="SomeRandomNonce">console.error("Soubor https://'.$_SERVER['HTTP_HOST'].'/'.$outputPath.$outputFileName.' je prázdný!")</script>';
        }
        else
        {
            //echo '<script nonce="SomeRandomNonce">console.info("Soubor https://'.$_SERVER['HTTP_HOST'].'/'.$outputPath.$outputFileName.' je aktuální. Vytvořen: '.date('j.n.Y H:i:s', filemtime($outputFile)).'; velikost: '.filesizeFormatted($outputFile).'")</script>';
        }
    }
}


/**
 * Copy files
 * @param string $outputPath
 * @param string $outputFileName
 * @param string $content
 */

function copyFile($inputFile, $outputFile, $filemtime = '')
{
    $inputFileTime = is_file($inputFile) ?  filemtime($inputFile) : '';
    $outputFileTime = is_file($outputFile) ?  filemtime($outputFile) : '';
    
    if (!is_file($outputFile) || $outputFileTime < $inputFileTime)
    {
        copy($inputFile, $outputFile);
        //echo '"Soubor https://'.$_SERVER['HTTP_HOST'].'/'.$inputFile.' zkopírován do \n       https://'.$_SERVER['HTTP_HOST'].'/'.$outputFile.'")</script>';
        echo '<div class="mdl-color-text--green">Soubor <strong>'.$inputFile.'</strong> zkopírován do <strong>'.$outputFile.'</strong></div>';
        
    }
}


/**
 * Return date of last moddifies file
 * @param string $inputPath
 */

function getLastModFileTime($inputPaths)
{
    $lastMod = 0;
    $lastModFile = '';
    foreach ($inputPaths as $inputPath)
    {
        if (is_file($inputPath) && filectime($inputPath) > $lastMod) {
            $lastMod = filectime($inputPath);
            $lastModFile = $inputPath;
        }
    }
    return ["file" => $lastModFile, "date" => $lastMod ];
}


/**
 * Recursively delete files
 * @param string $dir
 */

function deleteFile($filePath)
{
    if (is_array($filePath))
    {
        foreach ($filePath as $item) {
            file_exists($item) ? unlink($item) : null;
        }
    }
    elseif (file_exists($filePath))
    {
        unlink($filePath);
    }
}


/**
 * Recursively delete folder
 * @param string $dir
 */

function deleteDirectory($dir)
{
    if (!file_exists($dir)) { return true; }
    if (!is_dir($dir) || is_link($dir)) {
        return unlink($dir);
    }
    foreach (scandir($dir) as $item) {
        if ($item == '.' || $item == '..') { continue; }
        if (!deleteDirectory($dir . "/" . $item, false)) {
            chmod($dir . "/" . $item, 0777);
            if (!deleteDirectory($dir . "/" . $item, false)) return false;
        };
    }
    return rmdir($dir);
}