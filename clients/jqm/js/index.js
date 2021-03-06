/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */
var app = {
    // Application Constructor
    initialize: function() {
        this.bindEvents();
        var nid = 0;

        $.getJSON('http://copelandia.lulladev.com/node.json')
          .done(function(data) {
            $.each(data.list, function(i, item) {
              $('a', '<li><a/></li>').text(item.title).attr('href', '#recipe').attr('data-nid', item.nid).parent().appendTo('ul.recipes');
            });
            var mylist = $('ul.recipes');
            var listitems = mylist.children('li').get();
            listitems.sort(function(a, b) {
               return $(a).text().toUpperCase().localeCompare($(b).text().toUpperCase());
            })
            $.each(listitems, function(idx, itm) { mylist.append(itm); });
            $('ul.recipes').listview('refresh');
            $('ul.recipes a').click(function() {
              nid = $(this).data('nid');
              $.getJSON('http://copelandia.lulladev.com/node.json?nid=' + nid)
                .done(function(data) {
                  $.each(data.list, function(i, item) {
                    $('.recipe-title').text(item.title);
                    $('.recipe-description').html(item.body.value);
                    $('.recipe-cook').text(item.field_recipe_cook_time);
                    $('.recipe-instructions').html(item.field_recipe_instructions.value);
                    $('.recipe-prep').text(item.field_recipe_prep_time);
                    $('.recipe-servings').text(item.field_recipe_servings);
                  });
              });

            });
          });

            },
    // Bind Event Listeners
    //
    // Bind any events that are required on startup. Common events are:
    // 'load', 'deviceready', 'offline', and 'online'.
    bindEvents: function() {
        document.addEventListener('deviceready', this.onDeviceReady, false);
    },
    // deviceready Event Handler
    //
    // The scope of 'this' is the event. In order to call the 'receivedEvent'
    // function, we must explicity call 'app.receivedEvent(...);'
    onDeviceReady: function() {
        app.receivedEvent('deviceready');
    },
    // Update DOM on a Received Event
    receivedEvent: function(id) {
        var parentElement = document.getElementById(id);
        var listeningElement = parentElement.querySelector('.listening');
        var receivedElement = parentElement.querySelector('.received');

        listeningElement.setAttribute('style', 'display:none;');
        receivedElement.setAttribute('style', 'display:block;');

        console.log('Received Event: ' + id);
    }
};
