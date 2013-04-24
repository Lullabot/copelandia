<?php

/**
 * @file
 * Site aliases for Copelandia.
 */

/**
 * Dev environment.
 */
$aliases['dev'] = array(
  'root' => '/var/www/copelandia.lulladev.com/docroot',
  'uri' => 'copelandia.lulladev.com',
  'remote-host' => 'lulladev.com',
  'path-aliases' =>
  array (
    '%drush' => '/usr/local/share/drush',
    '%site' => 'sites/default/',
  ),
  'path-aliases' => array(
    '%dump-dir' => '/tmp',
  ),
  'source-command-specific' => array (
    'sql-sync' => array (
      'structure-tables-key' => 'common',
    ),
  ),
);
