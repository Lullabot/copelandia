'use strict';

function recipeController($scope, Node, File) {
  // Init local cache.
  $scope.cache = {};

  var recipes = $scope.recipes = Node.GetNodes();
  console.log(recipes);
  $scope.getImage = function(recipe) {
    if (recipe.field_recipe_photos.length !== 0) {
      var fileId = recipe.field_recipe_photos[0].file.id;

      $scope.cache['images'] = $scope.cache['images'] || {};
      if (!$scope.cache['images'][fileId]) {
        // Call server
        $scope.cache['images'][fileId] = File.get({fid: fileId});
      }

      // Get value from cache;
      return $scope.cache['images'][fileId].url;
    }
  };

}

function PagesController($scope, $http, $route, $routeParams, $compile) {
  $route.current.templateUrl = 'views/' + $routeParams.name + ".html";

  $http.get($route.current.templateUrl).then(function (msg) {
    $('.container').html($compile(msg.data)($scope));
  });

}
PagesController.$inject = ['$scope', '$http', '$route', '$routeParams', '$compile'];
