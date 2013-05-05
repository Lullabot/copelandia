<?php

/**
 * @file
 * Class for handling REST calls.
 */

class RESTServer {
  protected $endpoint;
  protected $settings;

  /**
   * Constructor. Initialize properties.
   */
  function __construct() {
    $endpoint_name = services_get_server_info('endpoint', '');
    $this->endpoint = services_endpoint_load($endpoint_name);

    // Get the server settings from the endpoint.
    $this->settings = !empty($this->endpoint->server_settings) ? $this->endpoint->server_settings : array();
    // Normalize the settings so that we get the expected structure
    // and sensible defaults.
    $this->settings = rest_server_setup_settings($this->settings);
  }

  /**
   * Handles the call to the REST server
   *
   * @param string $canonical_path
   * @param string $endpoint_path
   * @return void
   */
  public function handle($canonical_path, $endpoint_path) {
    $this->endpoint_path = $endpoint_path;
    services_set_server_info('resource_uri_formatter', array(&$this, 'uri_formatter'));

    $request_method = $this->getRequestMethod();

    $mime_type = '';
    $response_format = $this->getResponseFormatFromURL($canonical_path);

    $canonical_path_array = explode('/', $canonical_path);
    $resource_name = array_shift($canonical_path_array);

    $resources = services_get_resources_apply_settings($this->endpoint->name);

    $controller = FALSE;
    if (!empty($resource_name) && isset($resources[$resource_name])) {
      $resource = $resources[$resource_name];

      // Get the operation and fill with default values
      $controller = $this->resolveController($resource, $request_method, $canonical_path_array, $this->endpoint, $resource_name);
    }
    else {
      // This will stop the 404 from happening when you request just the endpoint.
      if ($this->endpoint->path == $resource_name) {
        $response = t('Services Endpoint "@name" has been setup successfully.', array('@name' => $this->endpoint->name));
        drupal_alter('services_endpoint_response', $response);
        return $response;
      }
      return services_error(t('Could not find resource @name.', array('@name' => $resource_name)), 404);
    }

    if (!$controller) {
      return services_error(t('Could not find the controller.'), 404);
    }

    // Parse the request data
    $arguments = $this->getControllerArguments($controller, $canonical_path_array, $request_method);

    $formats = $this->responseFormatters();
    if (empty($response_format)) {
      $response_format = $this->getResponseFormatContentTypeNegotiations($mime_type, $canonical_path, $controller, $formats);
    }

    $formatter = array();
    if (isset($formats[$response_format])) {
      $formatter = $formats[$response_format];
    }

    // Check if we support the response format and determine the mime type
    if (empty($mime_type) && !empty($formatter) && (!isset($formatter['model']) || $this->supportedControllerModel($controller, $formatter))) {
      $mime_type = $formatter['mime types'][0];
    }

    if (empty($response_format) || empty($mime_type)) {
      return services_error(t('Unknown or unsupported response format.'), 406);
    }

    $this->modelAlterArguments($arguments, $formatter, $controller);

    try {
      $result = services_controller_execute($controller, $arguments);
    }
    catch (ServicesException $e) {
      $result = $this->handleException($e, $controller, $arguments);
    }

    // Set the content type and render output.
    drupal_add_http_header('Content-type', $mime_type);

    return $this->renderFormatterView($controller, $formatter, $result);
  }

  /**
   * Retrieve formatter from URL. If format is in the path, we remove it from $canonical_path.
   *
   * For example <endpoint>/<path>.<format>
   *
   * @param $canonical_path
   *
   * @return string
   */
  protected function getResponseFormatFromURL(&$canonical_path) {
    $matches = array();
    $response_format = '';
    if (preg_match('/^(.+)\.([^\.^\/]+)$/', $canonical_path, $matches)) {
      $canonical_path = $matches[1];
      $response_format = $matches[2];
    }

    return $response_format;
  }

  /**
   * Determine response format and mime type using headers to negotiate content types.
   */
  protected function getResponseFormatContentTypeNegotiations(&$mime_type, $canonical_path, $controller, $formats) {
    drupal_add_http_header('Vary', 'Accept');

    // Negotiate response format based on accept-headers if we
    // don't have a response format.
    $mime_candidates = array();
    $mime_map = array();
    // Add all formatters that accepts raw data, or supports the format model
    foreach ($formats as $format => $formatter) {
      if (!isset($formatter['model']) || $this->supportedControllerModel($controller, $formatter)) {
        foreach ($formatter['mime types'] as $m) {
          $mime_candidates[] = $m;
          $mime_map[$m] = $format;
        }
      }
    }

    // Get the best matching format, default to json
    $response_format = 'json';
    if (isset($_SERVER['HTTP_ACCEPT'])) {
      $mime = $this->mimeParse();
      $mime_type = $mime->best_match($mime_candidates, $_SERVER['HTTP_ACCEPT']);
      $response_format = $mime_map[$mime_type];
    }

    return $response_format;
  }


  /**
   * Determine the request method
   */
  protected function getRequestMethod() {
    $method = $_SERVER['REQUEST_METHOD'];
    if ($method == 'POST' && isset($_SERVER['HTTP_X_HTTP_METHOD_OVERRIDE'])) {
      $method = $_SERVER['HTTP_X_HTTP_METHOD_OVERRIDE'];
    }
    if ($method == 'POST' && (isset($_GET['_method']) && $_GET['_method'])) {
      $method = $_GET['_method'];
    }
    if (isset($_GET['_method'])) {
      unset($_GET['_method']);
    }

    return $method;
  }

  /**
   * Give the model (if any) a opportunity to alter the arguments.
   * This might be needed for the model to ensure that all the required
   * information is requested.
   *
   * @param $arguments
   */
  protected function modelAlterArguments(&$arguments, $formatter, $controller) {
    //
    if (isset($formatter['model'])) {
      $cm = &$controller['models'][$formatter['model']];
      if (!isset($cm['arguments'])) {
        $cm['arguments'] = array();
      }

      // Check if any of the model arguments have been overridden
      if (isset($cm['allow_overrides'])) {
        foreach ($cm['allow_overrides'] as $arg) {
          if (isset($_GET[$formatter['model'] . ':' . $arg])) {
            $cm['arguments'][$arg] = $_GET[$formatter['model'] . ':' . $arg];
          }
        }
      }

      if (isset($cm['class']) && class_exists($cm['class'])) {
        if (method_exists($cm['class'], 'alterArguments')) {
          call_user_func_array($cm['class'] . '::alterArguments', array(&$arguments, $cm['arguments']));
        }
      }
    }
  }

  /**
   * Formats a resource uri
   *
   * @param array $path
   *  An array of strings containing the component parts of the path to the resource.
   * @return string
   *  Returns the formatted resource uri
   */
  public function uri_formatter($path) {
    return url($this->endpoint_path . '/' . join($path, '/'), array(
      'absolute' => TRUE,
    ));
  }

  /**
   * Parses controller arguments from request
   *
   * @param array $controller
   *  The controller definition
   * @param array $path
   * @param string $method
   *  The method used to make the request
   * @return void
   */
  protected function getControllerArguments($controller, $path, $method) {
    $data = $this->parseRequest($method, $controller);
    drupal_alter('rest_server_request_parsed', $data, $controller);

    $headers = $this->parseRequestHeaders();
    drupal_alter('rest_server_headers_parsed', $headers);

    $sources = array(
      'path' => $path,
      'param' => $_GET,
      'data' => $data,
      'headers' => $headers,
    );
    // Map source data to arguments.
    return $this->getControllerArgumentsFromSources($controller, $sources);
  }

  /**
   * array $controller
   *   Controller definition
   * array $sources
   *   Array of sources for arguments. Consists of following elements:
   *  'path' - path requested
   *  'params' - GET variables
   *  'data' - parsed POST data
   *  'headers' - request headers
   *
   * @return array
   */
  protected function getControllerArgumentsFromSources($controller, $sources) {
    $arguments = array();
    if (!isset($controller['args'])) {
      return array();
    }

    foreach ($controller['args'] as $argument_number => $argument_info) {
      // Fill in argument from source
      if (isset($argument_info['source'])) {
        if (is_array($argument_info['source'])) {
          list($source) = array_keys($argument_info['source']);
          $key = $argument_info['source'][$source];
          if (isset($sources[$source][$key])) {
            $arguments[$argument_number] = $sources[$source][$key];
          }
        }
        else {
          if (isset($sources[$argument_info['source']])) {
            $arguments[$argument_number] = $sources[$argument_info['source']];
          }
        }
        // Convert to array if argument expected to be array.
        if (isset($argument_info['type']) && $argument_info['type'] == 'array' && isset($arguments[$argument_number])) {
          $arguments[$argument_number] = (array)$arguments[$argument_number];
        }
      }
      // When argument isn't set, insert default value if provided or
      // throw a exception if the argument isn't optional.
      if (!isset($arguments[$argument_number])) {
        if (!isset($argument_info['optional']) || !$argument_info['optional']) {
          return services_error(t('Missing required argument @arg', array('@arg' => $argument_info['name'])), 401);
        }
        // Set default value or NULL if default value is not set.
        $arguments[$argument_number] = isset($argument_info['default value']) ? $argument_info['default value'] : NULL;
      }
    }
    return $arguments;
  }

  protected function parseRequestHeaders() {
    $headers = array();
    if (!empty($_SERVER['HTTP_IF_MODIFIED_SINCE'])) {
      $headers['IF_MODIFIED_SINCE'] = strtotime(preg_replace('/;.*$/', '', $_SERVER['HTTP_IF_MODIFIED_SINCE']));
    }
    return $headers;
  }

  protected function parseRequest($method, $controller) {
    switch ($method) {
      case 'POST':
      case 'PUT':
        if (isset($_SERVER['CONTENT_TYPE'])) {
          $type = self::parseContentHeader($_SERVER['CONTENT_TYPE']);
        }

        // Get the mime type for the request, default to form-urlencoded
        if (isset($type['value'])) {
          $mime = $type['value'];
        }
        else {
          $mime = 'application/x-www-form-urlencoded';
        }

        // Get the parser for the mime type
        $parser = $this->requestParser($mime, $controller);
        if (!$parser) {
          return services_error(t('Unsupported request content type @mime', array('@mime' => $mime)), 406);
        }
        // php://input is not available with enctype="multipart/form-data".
        // see http://php.net/manual/en/wrappers.php.php
        if ($mime == 'multipart/form-data') {
          $data = call_user_func($parser);
        }
        else {
          // Read the raw input stream
          if (module_exists('inputstream')) {
            $handle = fopen('drupal://input', 'r');
          }
          else {
            $handle = fopen('php://input', 'r');
          }

          if ($handle) {
            $data = call_user_func($parser, $handle);
            fclose($handle);
          }
        }
        return $data;

      default:
        return array();
    }
  }

  public static function parseContentHeader($value) {
    $ret_val = array();
    $value_pattern = '/^([^;]+)(;\s*(.+)\s*)?$/';
    $param_pattern = '/([a-z]+)=(([^\"][^;]+)|(\"(\\\"|[^"])+\"))/';
    $vm=array();

    if (preg_match($value_pattern, $value, $vm)) {
      $ret_val['value'] = $vm[1];
      if (count($vm)>2) {
        $pm = array();
        if (preg_match_all($param_pattern, $vm[3], $pm)) {
          $pcount = count($pm[0]);
          for ($i=0; $i<$pcount; $i++) {
            $value = $pm[2][$i];
            if (drupal_substr($value, 0, 1) == '"') {
              $value = stripcslashes(drupal_substr($value, 1, mb_strlen($value)-2));
            }
            $ret_val['param'][$pm[1][$i]] = $value;
          }
        }
      }
    }

    return $ret_val;
  }

  public static function contentFromStream($handle) {
    $content = '';
    while (!feof($handle)) {
      $content .= fread($handle, 8192);
    }
    return $content;
  }

  public static function fileRecieve($handle, $validators = array()) {
    $validators['file_validate_name_length'] = array();

    $type = RESTServer::parseContentHeader($_SERVER['CONTENT_TYPE']);
    $disposition = RESTServer::parseContentHeader($_SERVER['HTTP_CONTENT_DISPOSITION']);

    $filename = file_munge_filename(trim(basename($disposition['params']['filename'])));

    // Rename potentially executable files, to help prevent exploits.
    if (preg_match('/\.(php|pl|py|cgi|asp|js)$/i', $filename) && (drupal_substr($filename, -4) != '.txt')) {
      $type['value'] = 'text/plain';
      $filename .= '.txt';
    }

    $filepath = file_destination(file_create_path(file_directory_temp() . '/' . $filename), FILE_EXISTS_RENAME);
    $file = (object)array(
      'uid' => 0,
      'filename' => $filename,
      'filepath' => $filepath,
      'filemime' => $type['value'],
      'status' => FILE_STATUS_TEMPORARY,
      'timestamp' => time(),
    );
    RESTServer::streamToFile($handle, $filepath);
    $file->filesize = filesize($filepath);

    // Call the validation functions.
    $errors = array();
    foreach ($validators as $function => $args) {
      array_unshift($args, $file);
      $errors = array_merge($errors, call_user_func_array($function, $args));
    }
    if (!empty($errors)) {
      return services_error(t('Errors while validating the file - @errors', array('@errors' => implode(" ", $errors))), 406);
    }

    drupal_write_record('files', $file);

    return $file;
  }

  public static function streamToFile($source, $file) {
    $fp = fopen($file, 'w');
    if ($fp) {
      self::streamCopy($source, $fp);
      fclose($fp);
      return TRUE;
    }
    return FALSE;
  }

  public static function streamCopy($source, $destination) {
    while (!feof($source)) {
      $content = fread($source, 8192);
      fwrite($destination, $content);
    }
  }

  protected function renderFormatterView($controller, $formatter, $result) {
    // Wrap the results in a model class if required by the formatter
    if (isset($formatter['model'])) {
      $cm = $controller['models'][$formatter['model']];
      $model_arguments = isset($cm['arguments'])?$cm['arguments']:array();

      $model_class = new ReflectionClass($cm['class']);
      $result = $model_class->newInstanceArgs(array($result, $model_arguments));
    }

    $view_class = new ReflectionClass($formatter['view']);
    $view_arguments = isset($formatter['view arguments'])?$formatter['view arguments']:array();
    $view = $view_class->newInstanceArgs(array($result, $view_arguments));
    return $view->render();
  }

  /**
   * Get best match parser for $controller based on $mime type.
   */
  protected function requestParser($mime, $controller = NULL) {
    // Check if the controller has declared support for parsing the mime type.
    if ($controller && !empty($controller['rest request parsers'])) {
      $parser = $this->matchParser($mime, $controller['rest request parsers']);
      if ($parser) {
        return $parser;
      }
    }

    $parsers = rest_server_request_parsers();

    // Remove parsers that have been disabled for this endpoint.
    foreach (array_keys($parsers) as $key) {
      if (!$this->settings['parsers'][$key]) {
        unset($parsers[$key]);
      }
    }

    return $this->matchParser($mime, $parsers);
  }

  /**
   * Create a instance of the Mimeparse utility class.
   *
   * @return Mimeparse
   */
  protected function mimeParse() {
    static $mimeparse;
    if (!$mimeparse) {
      module_load_include('php', 'rest_server', 'lib/mimeparse');
      $mimeparse = new Mimeparse();
    }
    return $mimeparse;
  }

  /**
   * Matches a mime-type against a set of parsers.
   *
   * @param string $mime
   *  The mime-type of the request.
   * @param array $parsers
   *  An associative array of parser callbacks keyed by mime-type.
   * @return mixed
   *  Returns a parser callback or FALSE if no match was found.
   */
  protected function matchParser($mime, $parsers) {
    $mimeparse = $this->mimeParse();
    $mime_type = $mimeparse->best_match(array_keys($parsers), $mime);

    return ($mime_type) ? $parsers[$mime_type] : FALSE;
  }

  public static function parseURLEncoded($handle) {
    parse_str(self::contentFromStream($handle), $data);
    return $data;
  }

  public static function parsePHP($handle) {
    return unserialize(self::contentFromStream($handle));
  }

  public static function parseXML($handle) {
    // get/hold the old error state
    $old_error_state = libxml_use_internal_errors(1);

    // clear all libxml errors
    libxml_clear_errors();

    // get a now SimpleXmlElement object from the XML string
    $xml_data = simplexml_load_string(self::contentFromStream($handle));

    // if $xml_data is Null then we expect errors
    if (!$xml_data) {
      // build an error message string
      $message = '';
      $errors = libxml_get_errors();
      foreach ($errors as $error) {
        $message .= t('Line @line, Col @column: @message', array('@line' => $error->line, '@column' => $error->column, '@message' => $error->message)) . "\n\n";
      }

      // clear all libxml errors and restore the old error state
      libxml_clear_errors();
      libxml_use_internal_errors($old_error_state);

      // throw an error
      services_error($message, 406);
    }
    // whew, no errors, restore the old error state
    libxml_use_internal_errors($old_error_state);

    // unmarshal the SimpleXmlElement, and return the resulting array
    $php_array = self::unmarshalXML($xml_data, NULL);
    return (array) $php_array;
  }

  /**
   * A recusive function that unmarshals an XML string, into a php array.
   */
  protected static function unmarshalXML($node, $array) {
	// For all child XML elements
	foreach ($node->children() as $child) {
	  if (count($child->children()) > 0) {
		// if the child has children
		$att = 'is_array';
		if ($child->attributes()->$att) {
			$new_array = array();
			// recursive through <item>
			foreach($child->children() as $item){
				$new_array[] = self::unmarshalXML($item, $array[$item->getName()]);
			}
		}
		else {
      // else, simply create an array where the key is name of the element
		  $new_array = self::unmarshalXML($child, $array[$child->getName()]);
		}
		// add $new_array to $array
		$array[$child->getName()] = $new_array;
	  }
	  else {
    // Use the is_raw attribute on value elements for select type fields to
    // pass form validation. Example:
    // <field_terms_select>
    //    <und is_array="true">
    //      <item>
    //        <tid is_raw="true">10513</tid>
    //      </item>
    //      <item>
    //        <tid is_raw="true">10523</tid>
    //      </item>
    //    </und>
    //  </field_terms_select>
		if ($child->attributes()->is_raw) {
			return (string) $child;
		}
		$array[$child->getName()] = (string) $child;
	  }
	}
	// return the resulting array
	return $array;
  }

  public static function parseJSON($handle) {
    return json_decode(self::contentFromStream($handle), TRUE);
  }

  public static function parseFile($handle) {
    return self::contentFromStream($handle);
  }

  public static function parseMultipart() {
    return $_POST;
  }

  public static function parseYAML($handle) {
    if (($library = libraries_load('spyc')) && !empty($library['loaded'])) {
      return Spyc::YAMLLoadString(self::contentFromStream($handle));
    }
    else {
      watchdog('REST Server', 'Spyc library not found!', array(), WATCHDOG_ERROR);
      return array();
    }
  }

  protected function responseFormatters($format = NULL) {
    $formatters = rest_server_response_formatters();

    // Remove formatters that have been disabled for this endpoint.
    foreach (array_keys($formatters) as $key) {
      if (!$this->settings['formatters'][$key]) {
        unset($formatters[$key]);
      }
    }

    if ($format) {
      return isset($formatters[$format]) ? $formatters[$format] : FALSE;
    }

    return $formatters;
  }

  protected function supportedControllerModel($controller, $format) {
    if (
      // The format uses models
      isset($format['model']) &&
      // The controller provides models
      isset($controller['models']) &&
      // The controller supports the model required by the format
      isset($controller['models'][$format['model']])) {
        return $controller['models'][$format['model']];
    }
  }

  protected function resolveController($resource, $method, $path, $endpoint, $resource_name) {
    $pc = count($path);
    $class = NULL;
    $operation = NULL;

    // We do not group "if" conditions on purpose for better readability.

    // 'index' method.
    if ($method == 'GET' && isset($resource['operations']['index']) && $this->checkNumberOfArguments($pc, $resource['operations']['index'])) {
      $class = 'operations';
      $operation = 'index';
    }

    // 'retrieve' method.
    // First path element should be not empty.
    if ($method == 'GET' && isset($resource['operations']['retrieve']) && $this->checkNumberOfArguments($pc, $resource['operations']['retrieve']) && !empty($path[0]) && $pc >= 1) {
      $class = 'operations';
      $operation = 'retrieve';
    }

    // 'relationships'
    // First path element should be not empty,
    // second should be name of targeted action.
    if ($method == 'GET' && $pc >= 2 && isset($resource['relationships'][$path[1]])
         && $this->checkNumberOfArguments($pc, $resource['relationships'][$path[1]], 1) && isset($path[0])) {
      $class = 'relationships';
      $operation = $path[1];
    }

    // 'update'
    // First path element should be not empty.
    if ($method == 'PUT' && isset($resource['operations']['update']) && $this->checkNumberOfArguments($pc, $resource['operations']['update']) && !empty($path[0]) && $pc >= 1) {
      $class = 'operations';
      $operation = 'update';
    }

    // 'delete'
    // First path element should be not empty.
    if ($method == 'DELETE' && isset($resource['operations']['delete']) && $this->checkNumberOfArguments($pc, $resource['operations']['delete']) && !empty($path[0]) && $pc >= 1) {
      $class = 'operations';
      $operation = 'delete';
    }

    // 'create' method.
    // First path element should be not empty.
    if ($method == 'POST' && isset($resource['operations']['create']) && $this->checkNumberOfArguments($pc, $resource['operations']['create'])) {
      $class = 'operations';
      $operation = 'create';
    }

    // 'actions'
    // Forst path element should be action name
    if ($method == 'POST' && $pc >= 1 && isset($resource['actions'][$path[0]]) && $this->checkNumberOfArguments($pc, $resource['actions'][$path[0]], 1)) {
      $class = 'actions';
      $operation = $path[0];
    }

    // 'targeted_actions'
    // First path element should be not empty,
    // second should be name of targeted action.
    if ($method == 'POST' && $pc >= 2 && isset($resource['targeted_actions'][$path[1]])
         && $this->checkNumberOfArguments($pc, $resource['targeted_actions'][$path[1]], 1) && !empty($path[0])) {
      $class = 'targeted_actions';
      $operation = $path[1];
    }

    $controller = FALSE;
    if (!empty($class) && !empty($operation) && !empty($resource[$class][$operation])) {
      $controller = $resource[$class][$operation];
      if (isset($resource['file']) && empty($controller['file'])) {
        $controller['file'] = $resource['file'];
      }
    }
    services_request_apply_version($controller, array('method' => $operation, 'resource' => $resource_name));
    return $controller;
  }

  /**
   * Count possible numbers of 'path' arguments of the method.
   */
  private function checkNumberOfArguments($args_number, $resource_operation, $required_args = 0) {
    $not_required_args = 0;

    if (isset($resource_operation['args'])) {
      foreach ($resource_operation['args'] as $argument) {
        if (isset($argument['source']) && is_array($argument['source']) && isset($argument['source']['path'])) {
          if (!empty($argument['optional'])) {
            $not_required_args++;
          }
          else {
            $required_args++;
          }
        }
      }
    }

    return $args_number >= $required_args && $args_number <= $required_args + $not_required_args;
  }

  function handleException($exception, $controller = array(), $arguments = array()){
    $error_code = $exception->getCode();
    $error_message = $exception->getMessage();
    $error_data = method_exists($exception, 'getData') ? $exception->getData() : '';

    switch ($error_code) {
      case 204:
        $error_header_status_message = '204 No Content: ' . $error_message;
        break;
      case 304:
        $error_header_status_message =  '304 Not Modified: ' . $error_message;
        break;
      case 401:
        $error_header_status_message =  '401 Unauthorized: ' . $error_message;
        break;
      case 404:
        $error_header_status_message =  '404 Not found: ' . $error_message;
        break;
      case 406:
        $error_header_status_message = '406 Not Acceptable: ' . $error_message;
        break;
      default:
        if ($error_code >= 400 && $error_code < 600) {
          $error_header_status_message = $error_code . ':' . $error_message;
        }
        else {
          $error_header_status_message = '500 Internal Server Error: An error occurred (' . $error_code . '): ' . $error_message;
        }
        break;
    }

    $error_alter_array = array(
      'code' => $error_code,
      'header_message' => &$error_header_status_message,
      'body_data' => &$error_data,
    );
    drupal_alter('rest_server_execute_errors', $error_alter_array, $controller, $arguments);

    drupal_add_http_header('Status', strip_tags($error_header_status_message));
    if ($this->endpoint->debug) {
      debug($exception);
    }

    return $error_data;
  }
}
