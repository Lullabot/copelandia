<?php
/**
 * @file
 * copelandia_rest.oauth.inc
 */

/**
 * Implements hook_default_oauth_common_context().
 */
function copelandia_rest_default_oauth_common_context() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 1;
  $context->name = 'copelandia';
  $context->title = 'copelandia';
  $context->authorization_options = array(
    'access_token_lifetime' => NULL,
    'page_title' => 'Copelandia client authorization',
    'message' => NULL,
    'warning' => NULL,
    'deny_access_title' => NULL,
    'grant_access_title' => NULL,
    'disable_auth_level_selection' => NULL,
    'signature_methods' => array(
      0 => 'HMAC-SHA1',
      1 => 'HMAC-SHA256',
      2 => 'HMAC-SHA384',
      3 => 'HMAC-SHA512',
    ),
    'default_authorization_levels' => array(
      0 => 'basic',
    ),
  );
  $context->authorization_levels = array(
    'basic' => array(
      'name' => 'basic',
      'title' => 'Basic authentication',
      'default' => 1,
      'delete' => 0,
      'description' => '',
    ),
  );
  $export['copelandia'] = $context;

  return $export;
}
