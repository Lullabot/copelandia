'use strict';

angular.module('app', ['resource.nodes', 'resource.node', 'resource.file']).

  config(function($routeProvider) {
    $routeProvider.
      when('/', {}).
      otherwise({
        redirectTo: '/'
      });
  });