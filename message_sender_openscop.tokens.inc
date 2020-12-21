<?php
/**
 * Implements hook_tokens().
 */
function message_sender_openscop_tokens($type, $tokens, array $data = [], array $options = []) {
  $replacements = [];

  if ($type == 'message' && !empty($data['message'])) {
    /** @var \Drupal\message\Entity\Message $message */
    $message = $data['message'];
    $comment = NULL;
    $node = NULL;
    if ($message->hasField('field_comment_reference')) {
      $comment = $message->field_comment_reference->entity;
      $node = $comment->getCommentedEntity();
    }
    elseif ($message->hasField('field_node_reference')) {
      $node = $message->field_node_reference->get(0)->entity;

    }

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'render-comment':
          if (!empty($comment)) {
            $render_array = \Drupal::entityTypeManager()->getViewBuilder('comment')->view($comment);
            $text = \Drupal::service('renderer')->renderRoot($render_array);
            $replacements[$original] = $text;
          }
          break;

        case substr($name, 0, 11) == 'render-node':
          if (!empty($node)) {
            $view_mode = str_replace('render-node:', '', $name);
            if (empty($view_mode)) {
              $render_array = \Drupal::entityTypeManager()->getViewBuilder('node')->view($node);
            }
            else {
              $render_array = \Drupal::entityTypeManager()->getViewBuilder('node')->view($node, $view_mode);
            }
            $text = \Drupal::service('renderer')->renderRoot($render_array);
            $replacements[$original] = $text;
          }
          break;

      }
    }
  }

  return $replacements;
}
