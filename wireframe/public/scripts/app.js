'use strict';

angular.module('app', [])
  .config(function($routeProvider) {
    $routeProvider
      .when('/', {
        controller: 'RecipeCtrl'
      })
      .otherwise({
        redirectTo: '/'
      });
  });
