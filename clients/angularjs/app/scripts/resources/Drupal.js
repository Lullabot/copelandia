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
  })
  .config(["$httpProvider", function(provider) {
    // todo: figure out csrf
    // provider.defaults.headers.common['X-CSRF-Token'] = Drupal.settings.restws_csrf_token;
  }]);

angular
  .module('nodes', ['ngResource'])
  .factory('Nodes', function($resource) {
    var Node = $resource(url + '/node.json', {}, {});

    return Node;
  });