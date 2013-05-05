<?php

/**
 * @file Documentation about Services alter hooks.
 */

/**
 * Allow to alter arguments before they are passed to service callback.
 *
 * @param $controller
 *   Controller definition
 * @param $args
 *   Array of arguments
 * @param $options
 */
function hook_services_request_preprocess_alter($controller, &$args, $options) {

}

/**
 * Alter results of the services call.
 *
 * @param $controller
 *   Controller definition
 * @param array $args
 *   Arguments passed to services callback
 * @param array $result
 *   Array of results that can be altered
 */
function hook_services_request_postprocess_alter($controller, $args, &$result) {

}