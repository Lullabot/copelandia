<?php
/**
 * @file
 * copelandia_rest.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function copelandia_rest_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "oauth_common" && $api == "oauth") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "services" && $api == "services") {
    return array("version" => "3");
  }
}
