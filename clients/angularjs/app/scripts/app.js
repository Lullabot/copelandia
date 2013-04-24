'use strict';

angular.module('app', ['resource.nodes', 'resource.node', 'resource.file']).

  config(function($routeProvider) {
    $routeProvider.
      when('/', {
        templateUrl: 'views/main.html',
        controller: 'recipeController'
      }).
      otherwise({
        redirectTo: '/'
      });
  });