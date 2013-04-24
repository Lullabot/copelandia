# Copelandia

Copelandia is a proof of concept for a [COPE]
(http://blog.programmableweb.com/2009/10/13/cope-create-once-publish-everywhere/)
oriented Drupal site, to decouple the Drupal Backend from the Frontend.

## INSTALLATION

1. Clone this repository's project.
1. Set up a local virtualhost, such as copelandia.local pointing to root-of-this-project/docroot.
 * Also add the entry at your hosts file.
1. Install the site by going to http://copelandia.local/install.php
 * You can also install Drupal via Drush.
1. Sync your local database and files with the development environment with the following
   commands:
 * Note: you may need to review permissions at the files directory.

``` bash
drush syncdb
drush syncfiles
```
1. Open http://copelandia.local in your browser. You should get the same output as the one at
http://copelandia.lulladev.com.
