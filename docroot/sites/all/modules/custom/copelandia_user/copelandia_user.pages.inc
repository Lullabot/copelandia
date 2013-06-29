<?php
/**
 * @file copelandia_user.pages.inc
 * Callback implementations for copelandia_user module.
 */

/**
 * Simple callback to register a user based in a given email address.
 *
 * @return
 *   array with status and message entries.
 */
function copelandia_user_register() {
  $response = array('status' => 'OK');
  if (!empty($_POST['email'])) {
    $email = $_POST['email'];
    $nid = db_query('SELECT uid FROM {users} WHERE mail = :email', array(':email' => $email))->fetchField();
    if ($nid) {
      $response['status'] = 'KO';
      $response['message'] = 'Email already exists';
    }
    else {
      $new_user = array(
        'name' => preg_replace('/[^a-z0-9]/', '', $email),
        'mail' => $email,
      );
      $account = user_save(null, $new_user);
      $response['message'] = user_pass_reset_url($account);
    }
  }
  return $response;
}
