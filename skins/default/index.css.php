<?php

header('Content-type: text/css; charset=UTF-8', true);

require_once '../../init.inc';
require_once '../../functions.inc';
require_once '../../vendor/scssphp-0.6.7/scss.inc.php';
require_once '../../vendor/helpers/scss-helpers.php';

use Leafo\ScssPhp\Compiler;

$buildDir = '../../build/css';
$cssTempDir = '../../.tmp/css';
$sourceDir = '.';
$scss = new Compiler();
$scss->setImportPaths($sourceDir);

!file_exists($buildDir) ? mkdir($buildDir, 0777, true) : null;
!file_exists($cssTempDir) ? mkdir($cssTempDir, 0777, true) : null;


// Prepare data
// --------------

ob_start();
$scss->setFormatter('Leafo\ScssPhp\Formatter\Crunched');
\SCSSHelpers\buildCSSFromFile($sourceDir, $cssTempDir, $scss, '_show-grid.scss');
$showGrid = ob_get_contents();
ob_end_clean();

ob_start();
$scss->setFormatter('Leafo\ScssPhp\Formatter\Expanded');
\SCSSHelpers\buildCSSFromFile($sourceDir, $cssTempDir, $scss, 'fonts/fonts.scss');
\SCSSHelpers\buildCSSFromFile($sourceDir, $cssTempDir, $scss, 'bootstrap.scss');
\SCSSHelpers\buildCSSFromFile($sourceDir, $cssTempDir, $scss, 'custom.scss');
$fileGeneratedContent = ob_get_contents();
ob_end_clean();

ob_start();
$scss->setFormatter('Leafo\ScssPhp\Formatter\Crunched');
\SCSSHelpers\buildCSSFromFile($sourceDir, $cssTempDir.'-crunched', $scss, 'fonts/fonts.scss');
\SCSSHelpers\buildCSSFromFile($sourceDir, $cssTempDir.'-crunched', $scss, 'bootstrap.scss');
\SCSSHelpers\buildCSSFromFile($sourceDir, $cssTempDir.'-crunched', $scss, 'custom.scss');
$fileContentSanitized = ob_get_contents();
ob_end_clean();


// Sanitize content
// --------------

$fileContentSanitized = preg_replace('!/\*[^*]*\*+([^/][^*]*\*+)*/!', PHP_EOL.PHP_EOL, $fileContentSanitized);
$fileContentSanitized = preg_replace('/ -[ \n\r\t]+/', ' - ', $fileContentSanitized);
$fileContentSanitized = preg_replace('/[\n\r\t]+/', PHP_EOL, $fileContentSanitized);
$fileContentSanitized = preg_replace('/[ ]+/', ' ', $fileContentSanitized);
$fileContentSanitized = trim($fileContentSanitized);


// Write file
// --------------

writeFile($buildDir.'/static.min.css', $fileContentSanitized);


// Show result
// --------------

echo $fileGeneratedContent;
echo isProduction($GLOBALS['siteDomain']) ? '' : $showGrid;