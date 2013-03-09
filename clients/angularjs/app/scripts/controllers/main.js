'use strict';

function RecipeCtrl($scope, Nodes, Node) {
  $scope.recipes = Nodes.get({limit: 25});
};
