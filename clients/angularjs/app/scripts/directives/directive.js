'use strict';

/* Directives */

angular.module('directive.protobot', []).
  directive('protoRepeat', function() {
    return {
    restrict: "A",
    compile: function(tElement, attrs) {
      console.log(arguments);
      var content = tElement.children();
      for (var i=1; i<attrs.protoRepeat; i++) {
        tElement.append(content.clone());
      }
    }
  };
});
