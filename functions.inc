<?php

/**
 * Table of contents
 * 
 *  - isProduction()
 *  - sanitizeOutput()
 *  - sanitizeJson()
 *  - beautifyOutput()
 *  - autoUTF()
 *  - normalizeLines()
 *  - getBasicTextCorrections()
 *  - getLog()
 *  - folderExist()
 *  - fileIsIncludable()
 *  - deleteDirectory()
 *  - deleteFile()
 *  - xcopy()
 *  - filesizeFormatted()
 *  - compareModFilesTime()
 *  - getLastModFileTime()
 *  - getScriptLinks()
 *  - writeFile()
 *  - getGA()
 */


/*
 * AM I ON PRODUCTION SERVER?
 * note: need to check the $siteDomain variable to match production
 * @param: string $siteDomain
 * @return: boolean
 */

function isProduction($siteDomain)
{
    return $_SERVER['HTTP_HOST'] == $siteDomain;
}


/*
 * SANITIZE HTML
 * @param string $buffer
 * @var boolean $basic `true` dump string replace
 *                     `false` specific line wrap around elements
 * @return string
 */

function sanitizeOutput($buffer, $basic = false)
{
    if ($basic === true)
    {
        $search = array(
            '/\>[^\S ]+/s',     // strip whitespaces after tags, except space
            '/[^\S ]+\</s',     // strip whitespaces before tags, except space
            '/(\s)+/s',         // shorten multiple whitespace sequences
            '/<!--(.*)-->/Uis', // remove HTML comments
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
        $baseElements         = ['body', 'head', 'html', 'link', 'meta', 'noscript', 'script', 'style', 'title'];
        $semanticElementsMain = ['article', 'aside', 'footer', 'header', 'main'];
        $semanticElementsSide = ['audio', 'canvas', 'details', 'figcaption', 'figure', 'hgroup', 'menu', 'nav', 'progress', 'section', 'summary', 'video'];
        $contentElementsMain  = ['form', 'fieldset', 'table', 'thead', 'tbody', 'tfoot',];
        $contentElementsSide  = ['div', 'h[\d]', 'p', 'li', 'ol', 'ul', 'colgroup', 'caption', 'tr', 'th', 'td', 'button', 'label', 'legend', 'legend', 'optgroup', 'option', 'textarea', 'blockquote', 'object'];
        $inlineElements       = ['a', 'b', 'em', 'embed', 'i', 'img', 'small', 'span', 'strong'];
        
        $buffer = preg_replace('/<!--(.*)-->/Uis', '', $buffer);
        $buffer = preg_replace('/[\n\r\t ]+/', ' ', $buffer);
        
        foreach($baseElements as $toReplace) {
            // tag begin
            $buffer = preg_replace('/\s{0,}(<[\/]?'.$toReplace.')/i', '\\1', $buffer);
        }
        
        foreach($inlineElements as $toReplace) {
            $buffer = preg_replace('/(<\/?'.$toReplace.'[^>]*>)\s{0,}(<[\/]?'.$toReplace.')/i', '\\1\\2', $buffer);
        }
        
        foreach($contentElementsSide as $toReplace) {
            // tag begin
            $buffer = preg_replace('/(<\/?'.$toReplace.'[^>]*>)\s+/i', '\\1', $buffer);
            // tag end
            $buffer = preg_replace('/\s+(<\/'.$toReplace.')/i', '\\1', $buffer);
        }
        
        foreach($semanticElementsMain as $toReplace) {
            // tag begin
            $buffer = preg_replace('/(<'.$toReplace.' *[^\/]*?>)\s{0,}/i', PHP_EOL.PHP_EOL.'\\1'.PHP_EOL, $buffer);
            // tag end
            $buffer = preg_replace('/\s{0,}(<\/'.$toReplace.'>)/i', PHP_EOL.'\\1'.PHP_EOL.PHP_EOL, $buffer);
        }
        
        $buffer = preg_replace('/\s{3,}/', PHP_EOL.PHP_EOL, $buffer);
        $buffer = preg_replace('/\s*(<br>|<hr>)\s*/', '\\1', $buffer);
    }
    
    return $buffer;
}


/*
 * SANITIZE JSON
 * @param string $content
 * @return string
 */

function sanitizeJson($content)
{
    $data = autoUTF($content);
    $data = preg_replace('/[ \n\r\t ]/', ' ', $data);
    $data = preg_replace('/([\'\"])[ ]{0,}[,:][ ]{0,}([\'\"\[])/', "\\1:\\2", $data);
    
    return $data;
}


/*
 * BEAUTIFY HTML CODE
 * @param string $buffer
 * @return string
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
 * @param string $data
 * @return string
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
 * @param string $input
 * @return string
 */

function getBasicTextCorrections($input)
{
    $prepositionsNonbreak = Array('K', 'k', 'S', 's', 'V', 'v', 'Z', 'z', 'O', 'o', 'U', 'u', 'A', 'a', 'I', 'i');
    
    foreach($prepositionsNonbreak as $toReplace)
    {
        $output = preg_replace('/([ ,.;:!?>])('.$toReplace.')[ ]+/', '\\1\\2&nbsp;', $input);
    }
    
    // radove cislovky
    $output = preg_replace('/(\d{1,}[.]) (\D)/', '\\1&nbsp;\\2', $output);
    
    // rimsle cislice
    $output = preg_replace('/(\D) (^M{0,4}(CM|CD|D?C{0,3})(XC|XL|L?X{0,3})(IX|IV|V?I{0,3})$.)/', '\\1&nbsp;\\2', $output);
    
    // pomlcky
    $output = preg_replace('/ (–|—)/', '&nbsp;\\1', $output);

    // speciality
    $output = preg_replace('/´(\w)/i', '’\\1', $output);
    $output = preg_replace('/([[:alpha:]][.]) ([[:alpha:]][.])/', '\\1&nbsp;\\2', $output);
    $output = preg_replace('/(e)-(mail)/i', '\\1&#8209;\\2', $output);
    
    return $output;
}


/*
 * Show various info to html/console
 * @param string $content
 * @param string $type `html`returns HTML comment
 *                     `log` returns concole.log message
 *                     `warn` returns concole.warn message
 *                     `error`returns concole.error message
 * @return string On development returns various debugging info in browser's console
 */

function getLog($content, $type = null)
{
    if (!isProduction($GLOBALS['siteDomain']))
    {
        switch ($type) {
            case 'html':
                return '<!-- ' . $content . ' --><br>';
                break;
            case 'info':
                return '<script>var cssLabel1 = "color: #2a53cd; font-weight: bold;"; console.info('.$content.')</script>';
                break;
            case 'warn':
                return '<script>var cssLabel1 = "font-weight: bold;"; console.warn('.$content.')</script>';
                break;
            case 'error':
                return '<script>var cssLabel1 = "font-weight: bold;"; console.error('.$content.')</script>';
                break;
            default:
                return '<script>var cssLabel1 = "font-weight: bold;"; console.log('.$content.')</script>';
                break;
        }
    }
}

function showLog($content, $type = '')
{
    echo getLog($content, $type);
}


/**
 * Checks if a folder exist and return canonicalized absolute pathname (sort version)
 * @param string $folder the path being checked
 * @return mixed returns the canonicalized absolute pathname on success otherwise FALSE is returned
 */

function folderExist($folder)
{
    // Get canonicalized absolute pathname
    $path = realpath($folder);

    // If it exist, check if it's a directory
    return ($path !== false AND is_dir($path)) ? $path : false;
}


/**
* Ckeck if a file exists and verify existence in include_path
* @param string $filename the path and file name being checked
* @param boolean $returnFilename `true` returns file name of first file
* @return mixed returns the relative pathname on success otherwise FALSE is returned
*/

function fileIsIncludable($filename, $returnFilename = false) {
    
    $found = false;

    // If the path is entered absolutely, then include path wll be skipped
    if (strpos($filename, '/') === 0) {
        $filePath = $filename;
        if(@is_file($filePath) && @is_readable($filePath)) {
            $found = true;
        }
    } else {
        $pathArray = explode(PATH_SEPARATOR, get_include_path());
        foreach($pathArray as $path) {
            $filePath = $path."/".$filename;
            if(@is_file($filePath) && @is_readable($filePath)) {
                $found = true;
                break;
            }
        }
    }

    $ret = $found;

    // If we have to return the file path and the file exists
    if($returnFilename && $found) {
        $ret = $filePath;
    }

    return $ret;
}


/**
 * Recursively delete folder
 * @param string $dirPath directory path to delete
 */

function deleteDirectory($dirPath)
{
    if (!file_exists($dirPath)) {
        return true;
    }
    if (!is_dir($dirPath) || is_link($dirPath)) {
        return unlink($dirPath);
    }
    foreach (scandir($dirPath) as $item) {
        if ($item == '.' || $item == '..') { continue; }
        if (!deleteDirectory($dirPath . "/" . $item, false)) {
            chmod($dirPath . "/" . $item, 0777);
            if (!deleteDirectory($dirPath . "/" . $item, false)) return false;
        };
    }
    return rmdir($dirPath);
}


/**
 * Recursively delete files
 * @param mixed $filePath  path for array of paths with files for deletation
 */

function deleteFile($filePath)
{
    if (is_array($filePath)) {
        foreach ($filePath as $item) {
            file_exists($item) ? unlink($item) : null;
        }
    } elseif (file_exists($filePath)) {
        unlink($filePath);
    }
}


/**
 * Copy a file, or recursively copy a folder and its contents
 * @author      Aidan Lister <aidan@php.net>
 * @version     1.0.1
 * @link        http://aidanlister.com/2004/04/recursively-copying-directories-in-php/
 * @param       string   $source    Source path
 * @param       string   $dest      Destination path
 * @param       int      $permissions New folder creation permissions
 * @return      bool     Returns true on success, false on failure
 */

function xcopy($source, $dest, $permissions = 0755)
{
    // Check for symlinks
    if (is_link($source)) {
        return symlink(readlink($source), $dest);
    }

    // Simple copy for a file
    if (is_file($source)) {
        showLog('"Zkopírován %c'.$source.'%c −> %c'.$dest.'", "color: #555; font-weight: bold;", null, "font-weight: bold;"');
        echo PHP_EOL;
        return copy($source, $dest);
    }

    // Make destination directory
    if (!is_dir($dest)) {
        mkdir($dest, $permissions);
    }

    // Loop through the folder
    $dir = dir($source);
    while (false !== $entry = $dir->read()) {
        // Skip pointers
        if ($entry == '.' || $entry == '..') {
            continue;
        }
        
        // Deep copy directories
        xcopy("$source/$entry", "$dest/$entry", $permissions);
    }

    // Clean up
    $dir->close();
    return true;
}


/**
 * FILESIZE FORMATER
 * @param string $path path to the file
 * @return string
 */

function filesizeFormatted($path)
{
    $size = filesize($path);
    $units = array( 'B', 'KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB');
    $power = $size > 0 ? floor(log($size, 1024)) : 0;
    
    return number_format($size / pow(1024, $power), 2, ',', ',') . ' ' . $units[$power];
}


/**
 * Is second file older than first?
 * @param string $file1 path to 1st file
 * @param string $file2 path to 2nd file
 * @param boolean $returnFileInfo
 * @return mixed `true` 2nd file is olde or doesn't exist or array with file info
 */

function compareModFilesTime($file1, $file2, $returnFileInfo = false)
{
    $filemtime = null;
    $file1Time = fileIsIncludable($file1) ? filemtime($file1) : showLog('"Neexistuje soubor %c'.$file1.'", cssLabel1', 'error');
    $file2Time = fileIsIncludable($file2) ? filemtime($file2) : showLog('"Neexistuje soubor %c'.$file2.'", cssLabel1', 'error');
    
    if ($returnFileInfo === true && fileIsIncludable($file1) && fileIsIncludable($file2))
    {
        return ["file1" => $file1, "date1" => $file1Time, "file2" => $file2, "date2" => $file2Time];
    }
    elseif (fileIsIncludable($file1) && !fileIsIncludable($file2) || $file1Time > $file2Time)
    {
        return true;
    }
}


/**
 * Return date of last moddifies file
 * @param string $inputPath
 * @return array fie name and date
 */

function getLastModFileTime($inputPaths)
{
    $lastMod = 0;
    $lastModFile = '';
    foreach ($inputPaths as $inputPath)
    {
        if (fileIsIncludable($inputPath) && filemtime($inputPath) > $lastMod) {
            $lastMod = filemtime($inputPath);
            $lastModFile = $inputPath;
        }
    }
    return ["file" => $lastModFile, "date" => $lastMod ];
}





/*
 * Return <script> file by file on develop, one minified file on production
 * @param string $input can be single file or array of files
 * @param string $output
 */

function getScriptLinks($input, $output, $minifierVars = ['echo' => false, 'gzip' => false, 'closure' => true, 'remove_comments' => true])
{
    require_once 'vendor/magic-minifier-3.0.4/class.magic-min.php';
    $minified = new Minifier($minifierVars);
    $data = null;
    $input = (is_array($input)) ? $input : [$input];
    $outputPath = pathinfo($output, PATHINFO_DIRNAME);
    
    folderExist($outputPath) ? null : mkdir($outputPath, 0777, true);
    
    if (isProduction($GLOBALS['siteDomain']))
    {
        return '<script src="' .$minified->merge($output, 'js', $input) . '"></script>';
    }
    else
    {
        foreach ($input as $file)
        {
            if (fileIsIncludable($file)) {
                $data .= '<script src="' . $file . '"></script>';
            } else {
                $data .= getLog('"Nelze načíst soubor %c'.$file.'", cssLabel1', 'error');
            }
        }
        return $data;
    }
    
    //$minified->logs();
}


/**
 * WRITE FILES
 * @param string $outputPath
 * @param string $outputFileName
 * @param string $content
 */

function writeFile($outputFile, $content)
{
    $protocol = (isset($_SERVER['HTTPS']) && ($_SERVER['HTTPS'] == 'on' || $_SERVER['HTTPS'] == 1) || isset($_SERVER['HTTP_X_FORWARDED_PROTO']) && $_SERVER['HTTP_X_FORWARDED_PROTO'] == 'https') ?  'https://' : 'http://';
    $outputFileUrl = $protocol.$_SERVER['HTTP_HOST'].'/'.$outputFile;
    $outputPathParts = pathinfo($outputFile);
    
    
    
    
    (!empty($outputPathParts['dirname']) && !is_dir($outputPathParts['dirname'])) ? mkdir($outputPathParts['dirname'], 0777, true) : null;
    
    if (strlen($content) <= 0)
    {
        showLog('error', '"Prázná vstupní data pro soubor %c'.$outputFileUrl.'", cssLabel1');
    }
    else
    {
        if (!is_file($outputFile) || (is_file($content) && filemtime($content) > filemtime($outputFile) && mime_content_type($content) == 'text/plain'))
        {
            $content = is_file($content) ? file_get_contents($content) : $content;
            $content = normalizeLines(autoUTF($content));
            
            $fp = fopen($outputFile, "w+");
            fputs($fp, $content);
            fclose($fp);
            showLog('"Vytvořen soubor %c'.$outputFileUrl.'", cssLabel1');
        }
        elseif(!is_file($outputFile))
        {
            showLog('"Nelze vytvořit soubor %c'.$outputFileUrl.'", cssLabel1', 'error');
        }
        elseif (!is_writable($outputFile))
        {
            showLog('"Nelze zapisovat do souboru %c'.$outputFileUrl.'", cssLabel1', 'error');
        }
        elseif (filesize($outputFile) <= 0)
        {
            showLog('"Prázdný soubor %c'.$outputFileUrl.'", cssLabel1', 'error');
        }
        else
        {
            showLog('"Soubor %c'.$outputFileUrl.'%c je aktuální; vytvořen: '.date('j.n.Y H:i:s', filemtime($outputFile)).'; velikost: '.filesizeFormatted($outputFile).'", cssLabel1, ""');
        }
    }
}

/*
 * Google analytic tracking code
 * @id string $id
 * @return string
 */

function getGA($id, $type = null)
{
    $data = '';
    
    if (!empty($id))
    {   
        if ($type == 'michalek')
        {
            // http://www.vzhurudolu.cz/prirucka/google-analytics-pridani
            $data .= "<script>";
            $data .= "window.ga=window.ga||function(){(ga.q=ga.q||[]).push(arguments)};ga.l=+new Date;";
            $data .= "ga('create', '".$id."', 'auto');";
            $data .= "ga('send', 'pageview');";
            $data .= "</script>";
            $data .= "<script async src='https://www.google-analytics.com/analytics.js'></script>";
        }
        elseif ($type == 'async')
        {
            // https://developers.google.com/analytics/devguides/collection/analyticsjs/
            $data .= "<script>";
            $data .= "window.ga=window.ga||function(){(ga.q=ga.q||[]).push(arguments)};ga.l=+new Date;";
            $data .= "ga('create', '".$id."', 'auto');";
            $data .= "ga('send', 'pageview');";
            $data .= "</script>";
            $data .= "<script async src='https://www.google-analytics.com/analytics.js'></script>";
        }
        else
        {
            // https://developers.google.com/analytics/devguides/collection/analyticsjs/
            $data .= "<script>";
            $data .= "(function(i,s,o,g,r,a,m){i['GoogleAnalyticsObject']=r;i[r]=i[r]||function(){";
            $data .= "(i[r].q=i[r].q||[]).push(arguments)},i[r].l=1*new Date();a=s.createElement(o),";
            $data .= "m=s.getElementsByTagName(o)[0];a.async=1;a.src=g;m.parentNode.insertBefore(a,m)";
            $data .= "})(window,document,'script','https://www.google-analytics.com/analytics.js','ga');";
            $data .= "ga('create', ''".$id."', 'auto');";
            $data .= "ga('send', 'pageview');";
            $data .= "</script>";
        }
        
        $data = sanitizeJson($data);
        $data = sanitizeOutput($data);
        return $data;
    }   
}