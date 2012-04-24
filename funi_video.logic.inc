<?php
/**
 * This class contains the core methods (functions) needed to calculate a video's active states
 *
 * I've purposely left out Drupal specific stuff as much as possible, for the refactoring into codeigniter
 */
class funiVideoStates {
	
	var $nodeType;
	var $funimationSunrise;
	var $funimationSunset;
	var $huluSunrise;
	var $huluSunset;
	var $subscriptionSunrise;
	var $subscriptionSunset;
	
	var $now;
	
	var $huluID;
	
	function __construct($node) {

		$this->rating = isset($node->field_maturity_rating[0]['value']) ? $node->field_maturity_rating[0]['value'] : NULL;
		$this->mature = isset($node->field_is_mature[0]['value']) ? $node->field_is_mature[0]['value'] : NULL;
		$this->nodeType = strtoupper($node->type);
		$this->funimationSunrise = isset($node->field_sunrise_date[0]['value']) ? $node->field_sunrise_date[0]['value'] : NULL;
		$this->funimationSunset = !is_null($node->field_sunset_date[0]['value']) ? $node->field_sunset_date[0]['value'] : NULL;
		$this->huluSunrise = isset($node->field_hulu_sunrise_date[0]['value']) ? $node->field_hulu_sunrise_date[0]['value'] : NULL;
		$this->huluSunset = !is_null($node->field_hulu_sunset_date[0]['value']) ? $node->field_hulu_sunset_date[0]['value'] : NULL;
		$this->subscriptionSunrise = isset($node->field_subscription_sunrise_date[0]['value']) ? $node->field_subscription_sunrise_date[0]['value'] : NULL;
		$this->subscriptionSunset = !is_null($node->field_subscription_sunset_date[0]['value']) ? $node->field_subscription_sunset_date[0]['value'] : NULL;
		
		$now = new DateTime('NOW',new DateTimeZone('Europe/London')); // current time in UTC
		$this->now = $now->format('Y-m-d H:i');
		
		$this->huluID = $node->field_hulu_id[0]['safe'];
		
	}
		
	// takes sunrise and sunset dates and decides if the video is active
	// if either is null, then it defaults to other logic
	function isActive($rise,$set) {
		
		if(is_null($rise) && is_null($set)) return FALSE;
		
		$now = strtotime($this->now);
	  $rise = strtotime($rise);
	  if(is_null($set)) return ($now > $rise); // if the sunset fields are null then we'll just access the sunrise date 
	  $set = strtotime($set);   // ..
	  
	  return ($now > $rise) && ($now < $set); // extra parens for clarity
	}
	
	public function funimationIsActive() {
		return $this->isActive($this->funimationSunrise, $this->funimationSunset);
	}
	public function subscriptionIsActive() {
		return $this->isActive($this->subscriptionSunrise, $this->subscriptionSunset);
	}
	public function huluIsActive() {
		if(!empty($this->huluID) && strtoupper($this->country_code!='CA')) {
			return $this->isActive($this->huluSunrise, $this->huluSunset);
		} else {
			return FALSE;
		}
	}
}