'use strict';

angular.module('app', ['ngResource']).

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