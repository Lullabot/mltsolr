<?php
/**
 * @file
 * Renders related content through Solr.
 */

$plugin = array(
  'single' => TRUE,
  'title' => t('More like this Solr'),
  'description' => t('Lists related content for a given node.'),
  'category' => t('custom'),
  'render callback' => 'mltsolr_more_like_this_render',
  'all contexts' => TRUE,
);

/**
 * Renders a More Like This block.
 */
function mltsolr_more_like_this_render($subtype, $conf, $panel_args, $context = NULL) {
  $node = $context[0]->data;

  // Load the AngularJS controller to request related content to our API.
  drupal_add_js(drupal_get_path('module', 'mltsolr') . '/plugins/content_types/mltsolr_more_like_this.js');
  // Renders a wrapper template to host the list of teasers.
  $block = new stdClass();
  $block->title = '';
  $block->content = theme('more_like_this', array('nid' => $node->nid));
  return $block;
}
