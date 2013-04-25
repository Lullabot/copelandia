'use strict';

var app = angular.module('app', ['resource.nodes', 'resource.node', 'resource.file', 'directive.protobot']).

  config(function($routeProvider) {
    $routeProvider.
      when('/', {
        templateUrl: 'views/main.html'
      }).
      when('/prototest', {
        templateUrl: 'views/prototest.html'
      }).
      otherwise({
        redirectTo: '/'
      });
  });
