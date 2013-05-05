<?php
/**
 * @file
 * copelandia_rest.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function copelandia_rest_user_default_permissions() {
  $permissions = array();

  // Exported permission: oauth authorize consumers in copelandia.
  $permissions['oauth authorize consumers in copelandia'] = array(
    'name' => 'oauth authorize consumers in copelandia',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'oauth_common',
  );

  // Exported permission: oauth register consumers in copelandia.
  $permissions['oauth register consumers in copelandia'] = array(
    'name' => 'oauth register consumers in copelandia',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'oauth_common',
  );

  return $permissions;
}
