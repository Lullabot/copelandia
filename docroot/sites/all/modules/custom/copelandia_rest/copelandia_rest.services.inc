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
  $endpoint->server_settings = array(
    'formatters' => array(
      'bencode' => TRUE,
      'json' => TRUE,
      'php' => TRUE,
      'rss' => TRUE,
      'xml' => TRUE,
      'jsonp' => FALSE,
    ),
    'parsers' => array(
      'application/json' => TRUE,
      'application/vnd.php.serialized' => TRUE,
      'application/x-www-form-urlencoded' => TRUE,
      'application/xml' => TRUE,
      'multipart/form-data' => TRUE,
      'text/xml' => TRUE,
    ),
  );
  $endpoint->resources = array(
    'node' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
        'create' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
        'update' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
        'delete' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
        'index' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
      ),
      'relationships' => array(
        'files' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
        'comments' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
      ),
      'targeted_actions' => array(
        'attach_file' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
      ),
    ),
    'user' => array(
      'operations' => array(
        'retrieve' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
        'create' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
        'update' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
        'delete' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
      ),
      'actions' => array(
        'login' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
        'logout' => array(
          'enabled' => '1',
          'settings' => array(
            'services' => array(
              'resource_api_version' => '1.0',
            ),
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
        'register' => array(
          'enabled' => '1',
          'settings' => array(
            'services_oauth' => array(
              'credentials' => 'consumer',
              'authorization' => 'basic',
            ),
          ),
        ),
      ),
    ),
  );
  $endpoint->debug = 1;
  $export['server'] = $endpoint;

  return $export;
}
