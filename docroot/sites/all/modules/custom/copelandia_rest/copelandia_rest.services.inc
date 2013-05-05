<?php
/**
 * @file
 * copelandia_rest.services.inc
 */

/**
 * Implements hook_default_services_endpoint().
 */
function copelandia_rest_default_services_endpoint() {
  $export = array();

  $endpoint = new stdClass();
  $endpoint->disabled = FALSE; /* Edit this to true to make a default endpoint disabled initially */
  $endpoint->api_version = 3;
  $endpoint->name = 'server';
  $endpoint->server = 'rest_server';
  $endpoint->path = 'api';
  $endpoint->authentication = array(
    'services_oauth' => array(
      'oauth_context' => 'copelandia',
      'authorization' => 'basic',
      'credentials' => 'consumer',
    ),
  );
  $endpoint->server_settings = array();
  $endpoint->resources = array(
    'user' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => '1',
        ),
        'create' => array(
          'enabled' => '1',
        ),
        'update' => array(
          'enabled' => '1',
        ),
        'delete' => array(
          'enabled' => '1',
        ),
      ),
      'actions' => array(
        'login' => array(
          'enabled' => '1',
        ),
        'logout' => array(
          'enabled' => '1',
          'settings' => array(
            'services' => array(
              'resource_api_version' => '1.1',
            ),
          ),
        ),
        'register' => array(
          'enabled' => '1',
        ),
      ),
    ),
  );
  $endpoint->debug = 1;
  $export['server'] = $endpoint;

  return $export;
}
