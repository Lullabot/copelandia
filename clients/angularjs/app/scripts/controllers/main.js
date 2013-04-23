'use strict';

function RecipeCtrl($scope, $http) {
  $scope.images = [];

  $http.get('http://copelandia.lulladev.com/node.json').
    success(function(data, status, headers, config) {
      // this callback will be called asynchronously
      // when the response is available
      console.log('data', data);
      var recipes = $scope.recipes = data;

      // This sucks, but we have to loop through the list and do an individual
      // .get on the file path because of how restws returns the file info for
      // file fields. @see https://github.com/Lullabot/copelandia/issues/10
      for (var i = recipes.list.length - 1; i >= 0; i--) {
        var photos = recipes.list[i].field_recipe_photos;

        for (var x = photos.length - 1; x >= 0; x--) {
          var path = photos[x].file.uri;

          $http.get(path + '.json').
            success(function(data, status, headers, config) {
              $scope.images.push(data.fid = data.url);
            }).
            error(function(data, status, headers, config) {
              // called asynchronously if an error occurs
              // or server returns response with an error status.
            });
        }
      };

    }).
    error(function(data, status, headers, config) {
      // called asynchronously if an error occurs
      // or server returns response with an error status.
    });
}
