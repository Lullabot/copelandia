'use strict';

// todo: config for URL
var url = 'http://copelandia.lulladev.com';

angular
  .module('node', ['ngResource'])
  .factory('Node', function($resource) {
      var Node = $resource(url + '/node/:nid', {}, {
        update: { method: 'PUT' }
      });

      Node.prototype.update = function(node) {
        return Node.update({nid: this.nid}, angular.extend({}, this, {nid: undefined}), node);
      }

      return Node;
  });

angular
  .module('nodes', ['ngResource'])
  .factory('Nodes', function($resource) {
    var Node = $resource(
      url + '/node.json', 
      {}, 
      {get: {method: 'JSONP'}}
    );

    return Node;
  });