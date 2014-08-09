<?php

/**
 * @file
 * Menu callbacks for mltsolr module.
 */

/**
 * Returns an HTMl list of node teasers related to a given node.
 */
function mltsolr_more_like_this($node) {
  $content_types = array('article', 'page');

  // Check the node type before proceeding.
  if (!in_array($node->type, $content_types)) {
    return;
  }

  // Build the content types filter.
  $content_type_filter = new SolrFilterSubQuery('OR');
  foreach ($content_types as $content_type) {
    $content_type_filter->addFilter('bundle', $content_type);
  }

  // Build the issues filter.
  $issues = array();
  if (!empty($node->field_issues[LANGUAGE_NONE])) {
    foreach ($node->field_issues[LANGUAGE_NONE] as $issue) {
      $issues[] = 'node:' . $issue['target_id'];
    }
  }
  $issues_filter = new SolrFilterSubQuery('OR');
  foreach ($issues as $issue) {
    $issues_filter->addFilter('sm_field_issues', $issue);
  }

  // Build the topics filter.
  $topics = array();
  if (!empty($node->field_topics[LANGUAGE_NONE])) {
    foreach ($node->field_topics[LANGUAGE_NONE] as $topic) {
      $topics[] = 'node:' . $topic['target_id'];
    }
  }
  $topics_filter = new SolrFilterSubQuery('OR');
  foreach ($topics as $topic) {
    $topics_filter->addFilter('sm_field_topics', $topic);
  }

  // Leave right now if there are no topics and issues to search for.
  if (empty($topics) && empty($issues)) {
    return;
  }

  // Set conditions with proper grouping.
  // We need to filter by content type and find matches either in issues or
  // topics fields.
  $main_filter = new SolrFilterSubQuery('AND');
  $main_filter->addFilterSubQuery($content_type_filter);
  $issues_or_topics = new SolrFilterSubQuery('OR');
  $issues_or_topics->addFilterSubQuery($issues_filter);
  $issues_or_topics->addFilterSubQuery($topics_filter);
  $main_filter->addFilterSubQuery($issues_or_topics);

  try {
    // Create and configure the query.
    $searcher = 'mltsolr_default_node';
    $query = apachesolr_drupal_query($searcher);
    // We just want to obtain nids back.
    $query->addParam('fl', 'entity_id');
    // Add the conditions built above.
    $query->addFilterSubQuery($main_filter);
    // Sort by publish date in descending order.
    $sort_field = 'ds_field_publish_date_sort';
    $sort_direction = 'desc';
    $query->setAvailableSort($sort_field, $sort_direction);
    $query->setSolrsort($sort_field, $sort_direction);
    // Limit results to 4.
    $query->addParam('rows', 4);

    // Send the the query to Apache Solr and render results if matches are found.
    list($final_query, $response) = apachesolr_do_query($query, pager_find_page());
    if ($response->code == '200' && $response->response->numFound > 0) {
      $nodes = array();
      // Extract nids from the response and load them.
      foreach ($response->response->docs as $result) {
        $node = node_load($result->entity_id);
        if (!empty($node)) {
          $nodes[] = $node;
        }
      }
      if (count($nodes)) {
        // Build and return the list of teasers.
        $output = '<ul>';
        foreach ($nodes as $node) {
          $node_rendered = drupal_render(node_view($node, 'teaser_small'));
          $output = '<li>' . $node_rendered . '</li>';
        }
        $output .= '</ul>';
      }
    }
  }
  catch (Exception $e) {
    // Oopsie, there was an error. Log it and terminate silently.
    watchdog('mltsolr', 'There was an error while processing More Like This block: %error', array('%error' => $e->getMessage()), WATCHDOG_ERROR);
  }
}
