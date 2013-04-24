'use strict';

angular.module('resource.node', ['ngResource']).
  // Node.get(:nid);
  factory('Node', function($resource) {
    var Node = $resource('http://copelandia.lulladev.com/node/:nid', {}, {});
    return Node;
  });

angular.module('resource.nodes', ['ngResource']).  
  // Nodes.get();
  factory('Nodes', function($resource) {
    var Nodes = $resource('http://copelandia.lulladev.com/node.json', {}, {});
    return Nodes;
  });

angular.module('resource.file', ['ngResource']).
  // File.get(:fid);
  factory('File', function($resource) {
    var File = $resource('http://copelandia.lulladev.com/file/:fid', {}, {});
    return File;
  });