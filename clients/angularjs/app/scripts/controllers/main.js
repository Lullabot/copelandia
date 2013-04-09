'use strict';

function RecipeCtrl($scope, $http) {
  // $scope.recipes = Nodes.get({limit: 25});
  $http({method: 'GET', url: 'http://copelandia.lulladev.com/node.json'}).
    success(function(data, status, headers, config) {
      // this callback will be called asynchronously
      // when the response is available
      console.log(data);
    }).
    error(function(data, status, headers, config) {
      // called asynchronously if an error occurs
      // or server returns response with an error status.
    });
}
