<?php

header('Content-type: text/css; charset=UTF-8', true);

require_once '../../init.inc';
require_once '../../functions.inc';
require_once '../../vendor/scssphp-0.6.7/scss.inc.php';
require_once '../../vendor/helpers/scss-helpers.php';

use Leafo\ScssPhp\Compiler;

$buildDir = '../../build/css';
$cssTempDir = '../../.tmp';
$sourceDir = '.';
$scss = new Compiler();
$scss->setImportPaths($sourceDir);

isProduction($GLOBALS['siteDomain']) ? $scss->setFormatter('Leafo\ScssPhp\Formatter\Crunched') : $scss->setFormatter('Leafo\ScssPhp\Formatter\Expanded');

!file_exists($buildDir) ? mkdir($buildDir, 0777, true) : null;
!file_exists($cssTempDir) ? mkdir($cssTempDir, 0777, true) : null;


// Prepare data
// --------------

ob_start();

isProduction($GLOBALS['siteDomain']) ? null : \SCSSHelpers\buildCSSFromFile($sourceDir, $cssTempDir, $scss, 'show-breakpoints.scss');

$fonts  = '@font-face {font-family: "Open Sans"; font-style: normal; font-weight: 300; src: local("Open Sans Light"), local("OpenSans-Light"), url("/build/fonts/open-sans-300-latin-ext.woff") format("woff");}';
$fonts .= '@font-face {font-family: "Material Icons"; font-style: normal; font-weight: 400; src: local("Material Icons"), local("MaterialIcons-Regular"), url("/build/fonts/material-icons.woff") format("woff");}';
$fonts .= '.material-icons {font-family: "Material Icons"; font-weight: normal; font-style: normal; font-size: 24px; line-height: 1; letter-spacing: normal; text-transform: none; display: inline-block; white-space: nowrap; word-wrap: normal; direction: ltr; font-feature-settings: "liga";}';
$fonts = \SCSSHelpers\getStringFromCompiler($fonts, $scss, TRUE);

echo $fonts . PHP_EOL . PHP_EOL;

\SCSSHelpers\buildCSSFromFile($sourceDir, $cssTempDir, $scss, 'bootstrap.scss');
\SCSSHelpers\buildCSSFromFile($sourceDir, $cssTempDir, $scss, 'custom.scss');

$fileGeneratedContent = ob_get_contents();

ob_end_flush();


// Generate static file
// --------------

$fileGeneratedContent = preg_replace('!/\*[^*]*\*+([^/][^*]*\*+)*/ !', PHP_EOL.PHP_EOL, $fileGeneratedContent);
$fileGeneratedContent = trim($fileGeneratedContent);

writeFile($buildDir.'/', 'static.min.css', $fileGeneratedContent);