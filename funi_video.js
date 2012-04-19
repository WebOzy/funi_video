$(document).ready(function() {

  $("#edit-analytics-type-1").change( function(){
    $('#gadwordsFieldset').slideDown();
    $('#customFieldset').hide();
  });
  $("#edit-analytics-type-0").change( function(){
    $('#gadwordsFieldset').hide();
    $('#customFieldset').slideDown();
  });
});
