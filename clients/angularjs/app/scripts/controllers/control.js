'use strict';

function recipeController($scope, Node, Nodes, File) {
  // Init local cache.
  $scope.cache = {};

  var recipes = $scope.recipes = Nodes.get();
  
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