'use strict';

/**
 * resource.node
 *
 * Node.GetNodes();
 *   Calls --> node
 *
 * Node.GetNode( { id: 12 } );
 *   Calls -> node/12
 */
angular.module('resource.node', ['ngResource']).
  factory('Node', function($resource) {
    var Node = $resource('http://copelandia.lulladev.com/node/:id',
        {id: '@id'},
        {
            GetNodes: { method: "GET", params: {}},
            GetNode: { method: "GET", params: { id: 0 }}
        });
    return Node;
  });


angular.module('resource.file', ['ngResource']).
  // File.get(:fid);
  factory('File', function($resource) {
    var File = $resource('http://copelandia.lulladev.com/file/:fid', {}, {});
    return File;
  });