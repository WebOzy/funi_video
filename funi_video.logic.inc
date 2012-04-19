<?php
/**
 * This class contains the core methods (functions) needed to choose which player a user should get
 * The methods here create an array which 'route' the user to the correct player, based on rules set by the
 * function, set in the module, and called by the theme layer
 *
 * I've purposely left out Drupal specific stuff as much as possible, for the refactoring into codeigniter
 */
class funiVideoChooser {

	var $roles;
	var $age;
	var $rating;
	var $mature;
	
	var $nodeType;
	var $showFieldTokenValue;
	var $showFieldTokenPromo;
	var $funimatationSunrise;
	var $funimationSunset;
	var $huluSunrise;
	var $huluSunset;
	var $subscriptionSunrise;
	var $subscriptionSunset;
	
	var $now;
	
	var $huluID;
	
	function __construct($node,$show_node,$user) {
		$this->roles = $user->roles;
		$this->age = isset($user->age) ? $user->age : NULL;
		
		$this->rating = isset($node->field_maturity_rating[0]['value']) ? $node->field_maturity_rating[0]['value'] : NULL;
		$this->mature = isset($node->field_is_mature[0]['value']) ? $node->field_is_mature[0]['value'] : NULL;
		$this->nodeType = strtoupper($node->type);
		$this->showFieldTokenValue = $show_node->field_token_video[0]['value'];
		$this->showFieldTokenPromo = $show_node->field_token_promo[0];
		$this->funimationSunrise = isset($node->field_sunrise_date[0]['value']) ? $node->field_sunrise_date[0]['value'] : NULL;
		$this->funimationSunset = !is_null($node->field_sunset_date[0]['value']) ? $node->field_sunset_date[0]['value'] : NULL;
		$this->huluSunrise = isset($node->field_hulu_sunrise_date[0]['value']) ? $node->field_hulu_sunrise_date[0]['value'] : NULL;
		$this->huluSunset = !is_null($node->field_hulu_sunset_date[0]['value']) ? $node->field_hulu_sunset_date[0]['value'] : NULL;
		$this->subscriptionSunrise = isset($node->field_subscription_sunrise_date[0]['value']) ? $node->field_subscription_sunrise_date[0]['value'] : NULL;
		$this->subscriptionSunset = !is_null($node->field_subscription_sunset_date[0]['value']) ? $node->field_subscription_sunset_date[0]['value'] : NULL;
		
		$now = new DateTime('NOW',new DateTimeZone('Europe/London')); // current time in UTC
		$this->now = $now->format('Y-m-d H:i');
		
		// get the user's country code
 		$ip = ip_address();
		$country = ip2cc_get_country($ip);
		$this->country_code = $country->country_code;
		
		$this->huluID = $node->field_hulu_id[0]['safe'];
	}
	
	public function ageGate($minimumAge = 17) { // keep this public, in the event we need to tweak the min age later
		if ( $this->rating=='TV-MA' || $this->rating=='R' || $this->rating=='NR' || $this->mature != 0 ) {
		  			
			if ( is_null($this->age) || $this->age < $minimumAge ) {
			  return TRUE;
			}
			
		}
		return FALSE;
	}
	
	public function countryGate($cancel = 0) {
		if($cancel != 0) { // for development, we'll add a flag to prevent countryGating
			$matches = 1;
		} else {
			$countries = $this->nodeType == "EPISODE" ? $this->showFieldTokenValue : $this->showFieldTokenPromo;
	 		
			// get country code matches : from result to allowed
			$allow = explode( ',' , $countries );
			$matches = 0;
			foreach ( $allow as $i ) { 
				if ( strtoupper( $this->country_code ) == strtoupper( $i ) ) { 
					$matches++;
				} 
			}
		}
	
		if ( $matches == 0 ) { // if we are not in an allowed country…
		  return TRUE;
		} else {
			return FALSE;
		}
	}
	
	public function isSubscribed() {
		if(!in_array('Subscriber',$this->roles)) {
			return FALSE;
		} else {
			return TRUE;
		}
	}
	
	function isActive($rise,$set) {
	  $rise = strtotime($rise);
	  if(is_null($set)) return ($this->now > $rise); // if the sunset fields are null then we'll just access the sunrise date 
	  $set = strtotime($set);   // ..
	  
	  return ($this->now > $rise) && ($this->now < $set); // extra parens for clarity
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