
jQuery(window).bind('scroll', function (){
  if (jQuery(window).scrollTop() > 900){
    jQuery('#main-nav').addClass('navbar-fixed-top');
  } else {
    jQuery('#main-nav').removeClass('navbar-fixed-top');
  }
});

jQuery(document).ready(function($) {
  "use strict";
  $('#main-nav .nav').onePageNav({
    currentClass: 'active',
    scrollOffset: 69,
  });  
});

$(document).ready(function(){
  $('#testimonial').parallax("50%", 0.4);
})


$(document).ready(function() {
      $(".owl-carousel").owlCarousel({
        autoPlay: 3000,
        items : 4,
        itemsDesktop : [1199,3],
        itemsDesktopSmall : [979,3]
      });

    });