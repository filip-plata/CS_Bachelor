/* Commission endpoint url */
var comm_url;

$(document).ready(function() {

    $(function() {
      $("#site-search").autocomplete({
          source: comm_url,
          minLength: 3,
      });
    });


    $( '#menu-btn' ).click(function(){
        $('.mobile-menu').toggleClass('expand')
    })
});
