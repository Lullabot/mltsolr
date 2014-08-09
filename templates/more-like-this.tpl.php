<?php
/**
 * @file
 * Placeholder to host the list of rendered teasers.
 *
 * The AngularJS controller can access the data-content-id property which
 * contains the nid to use to search for related content.
 */
?>
<div data-ng-controller="moreLikeThisController" data-content-id="<?php print $nid; ?>" data-ng-cloak>
  <h2 class="pane-title" data-ng-show="teasers">More Like This</h2>
  <div id="more-like-this" data-ng-bind-html="teasers"></div>
</div>
