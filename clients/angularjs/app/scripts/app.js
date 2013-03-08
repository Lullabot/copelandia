'use strict';

angular.module('app', ['node', 'nodes'])
  .config(function($routeProvider) {
    $routeProvider
      .when('/', {
        templateUrl: 'views/main.html',
        controller: 'RecipeCtrl'
      })
      .otherwise({
        redirectTo: '/'
      });
  });