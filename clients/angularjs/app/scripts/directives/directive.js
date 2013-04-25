'use strict';

/* Directives */

angular.module('directive.protobot', []).
  directive('repeatNtimes', function() {
    return {
    restrict: "E",
    compile: function(tElement, attrs) {
      var content = tElement.children();
      for (var i=1; i<attrs.repeat; i++) {
        tElement.append(content.clone());
      }
    }
  };
});
