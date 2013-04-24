'use strict';

function recipeController($scope, Node, Nodes, File) {
  // Init local cache.
  $scope.cache = {};

  var recipes = $scope.recipes = Nodes.get();
  console.log(recipes);
  
  $scope.getImages = function(recipes) {
    for (var i = recipes.list.length - 1; i >= 0; i--) {
      if (recipes.list[i].field_recipe_photos.length !== 0) {
        $scope.cache['images'] = $scope.cache['images'] || {};
        if ($scope.cache['images'][i]) {
          // Get values from cache.
          $scope.recipes.list[i].image = $scope.cache['images'][i];
        } else {
          // Call server.
          $scope.recipes.list[i].image = File.get({fid: recipes.list[i].field_recipe_photos[0].file.id});
          $scope.cache['images'][i] = $scope.recipes.list[i].image;
        }
      } 
    }
  };

}