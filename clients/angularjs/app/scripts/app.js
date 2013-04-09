'use strict';

angular.module('app', [])
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