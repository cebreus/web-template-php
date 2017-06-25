<?php

namespace SCSSHelpers;

function getCommentString($start, $end) 
{
    $elapsed = round(($end - $start), 4);
    $v = \Leafo\ScssPhp\Version::VERSION;
    $t = date('r');
    return "/* compiled by scssphp $v on $t (${elapsed}s) */\n\n";
}

function getStringFromCompiler($in, $scss, $comment=FALSE)
{
    $start = microtime(true); 
    $r = $scss->compile($in);
    $end = microtime(true);
    if ($comment) {
        $r = getCommentString($start, $end).$r;
    }
    return $r;
}

function getStringFromFile($sourceDir, $file, $scss, $comment=FALSE)
{
    $file_content = file_get_contents($sourceDir.'/'.$file);
    return getStringFromCompiler($file_content, $scss, $comment);
}

function getStringFromDirSCSS($sourceDir, $scss)
{
    $files = scandir($sourceDir);
    $ret = '';

    foreach ($files as $file) {
        if (preg_match('/^\S+\.scss$/', $file)) {
            $ret .= getStringFromFile($sourceDir, $file, $scss);
        }
    }
    return $ret;
}

function buildCSSFromFile($sourceDir, $buildDir, $scss, $file)
{
    $_GET['p'] = $file;
    $server = new \Leafo\ScssPhp\Server($sourceDir, $buildDir, $scss);
    $server->serve();
}

function buildCSSFromDir($sourceDir, $buildDir, $scss)
{
    $files = scandir($sourceDir);

    foreach ($files as $file) {
        if (preg_match('/^\S+\.scss$/', $file)) {
            buildCSSFromFile($sourceDir, $buildDir, $scss, $file);
        }
    }

}
