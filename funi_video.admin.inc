<?php
/**
 * Implementation of hook_admin_settings() for configuring the module
 */
function funi_video_admin_settings(&$form_state) {
	  $form['funi_video_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Subscription player settings'),
    '#collapsible' => FALSE,
  );
  $form['funi_video_settings']['funi_video_sub_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Subscription Player URL'),
    '#default_value' => variable_get('funi_video_sub_url', ''),
    '#description' => t('Url for the custom subscription player. Should be in the format /sites/all/...'),
    '#size' => 85,
    '#maxlength' => 255,
    '#required' => FALSE,
  );
  
 // Choose roles to track.
  $form['roletracking'] = array(
    '#type' => 'fieldset',
    '#title' => t('Roles to get the custom player'),
    '#collapsible' => TRUE,
  );
  $result = db_query('SELECT * FROM {role} ORDER BY name');

  $form['roletracking']['roles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site Roles'),
    '#collapsible' => TRUE,
    '#description' => t('Define what user roles should use the custom player. Roles that are not selected will get the default player.')
  );

  while ($role = db_fetch_object($result)) {
     // can't use empty spaces in varname
    $role_varname = $string = str_replace(' ', '_', $role->name);
    $form['roletracking']['roles']["funi_video_role_{$role_varname}"] = array(
      '#type' => 'checkbox',
      '#title' => t($role->name),
      '#default_value' => variable_get("funi_video_role_{$role_varname}", FALSE),
    );
  }
  return system_settings_form($form);  
}

/**
 * Page callback for multianalytics admin landing page form.
 */
/*
function funi_video_admin_list() {
	_funi_video_addcssjs();
	
	$result = db_query('SELECT aid, title, type, enabled FROM {funi_video} ORDER BY type, title');

  $form = array();
  $enabled = array();
  while ($row = db_fetch_object($result)) {
  	$disabledClass = ($row->enabled == 0) ? 'class="madisabled"' : '';
  	
  	$snippets[$row->aid] = ''; // create the keys
  	
  	$form[$row->aid]['title'] = array('#value' => $row->title);
  	$form[$row->aid]['edit'] = array('#value' => l(t('Edit'), 'admin/build/funi_video/'. $row->aid .'/edit'));
  	$form[$row->aid]['delete'] = array('#value' => l(t('Delete'), 'admin/build/funi_video/'. $row->aid .'/delete'));
  	
  	$enabled[] = $row->enabled;
  }
  
  $form['enabled'] = array(
  	'#type' => 'checkboxes',
  	'#options' => $snippets,
  	'#default_value' => $enabled,
  );

	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Save'),
	);
	
	$form['cancel'] = array(
		'#type' => 'markup',
		'#value' => l(t('Cancel Changes'), 'admin/build/funi_video'),
	);
	
  return $form;
}
*/

/*
function theme_funi_video_admin_list($form) {
	
	if(element_children($form) == NULL) {
     $output = '<p>'. t('Click on the "New Analytics" tab to get started.') .'</p>';
  } else {
  
	  $header = array(
	    array('data' => t('Snippet Title')),
	    array('data' => t('Operations'), 'colspan' => 2),
	    //array('data' => t('Enabled')),
	  );
	
		$rows = array();
		foreach (element_children($form) as $key) {
			$row = array();
			if (isset($form[$key]['title'])) {
	 
				$row[] = drupal_render($form[$key]['title']);
				$row[] = array('data' => drupal_render($form[$key]['edit']));
				$row[] = array('data' => drupal_render($form[$key]['delete']));
				
				//$enabled = drupal_render($form['enabled'][$key]);
				//$row[] = array('data' => $enabled, ‘class’ => ‘checkbox’);
	 
				$rows[] = $row;
			}
		}
		 
		$output = theme('table', $header, $rows);
		//$output .= drupal_render($form);
	}
	
	if(variable_get("funi_video_mode", 0) == 1) { 
		$message = user_access('View Funi Video debug') ?  t('Funi Video is in !debug mode.',array('!debug' => l('debug','admin/build/funi_video/settings'))) : t('Funi Video is in debug mode.');
		drupal_set_message($message); 
	}
	
	return $output;
}
*/

/*
function funi_video_admin_list_submit($form,$form_state) {
  	$funi_video = array();
  	$funi_video = array(
  		$form_state['values']['title'], 
    	$form_state['values']['analytics_type'], 
    	$form_state['values']['enabled'],
    	isset($form_state['values']['body']) ? $form_state['values']['body'] : NULL,
    	$form_state['values']['visibility'],
    	trim($form_state['values']['pages']),
    	$form_state['values']['funi_video_adwords_language'],
    	$form_state['values']['label'],
    	$form_state['values']['format'],
    	$form_state['values']['color'],
    	$form_state['values']['aid'] > 0 ? $form_state['values']['aid'] : 0
  	);
  	
  	if($form_state['values']['aid'] > 0) { // update record
  			db_query("UPDATE {funi_video} SET title='%s', type=%d, enabled=%d, body='%s', visibility=%d, pages='%s', language='%s', label='%s', format=%d, color='%s' WHERE aid=%d", $funi_video);
  			drupal_set_message(t('The snippet has been updated.'));
		} else { // insert new record
			db_query("INSERT INTO {funi_video} (title, type, enabled, body, visibility, pages, language, label, format, color) 
						VALUES ('%s', %d, %d, '%s', %d,'%s','%s','%s',%d,'%s')", $funi_video);
			drupal_set_message(t('The snippet has been created.'));
		}
}
*/

