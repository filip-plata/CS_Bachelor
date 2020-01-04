function isNumeric(n) {
  return !isNaN(parseFloat(n)) && isFinite(n);
}

/* This is a hack */
var url = window.location.href;
var splitted_url = url.split("/")
var last_uri = splitted_url[splitted_url.length - 2]
var region_code;

if (isNumeric(last_uri)) {
  region_code = last_uri;
} else {
  region_code = "";
}

function login_list_element_cons() {
    var result = $("<li><a>Logowanie</a></li>");
    $('a', result).attr('href', "/user/logowanie/");
    return result;
}

function logout_list_element_cons() {
    var result = $("<li><a>Wyloguj</a></li>");
    $('a', result).attr('href', "/");
    $('a', result).attr('onclick', "logout()");
    return result;
}

function register_list_element_cons() {
    var result = $('<li><a>Rejestracja</a></li>');
    $('a', result).attr('href', "/user/rejestracja/");
    return result;
}

var login_list_element = login_list_element_cons();
var logout_list_element = logout_list_element_cons();
var register_list_element = register_list_element_cons();

$(document).ready(function() {
    setup_navigation();
});

function get_login() {
    return localStorage.getItem('login');
}

function get_password() {
    return localStorage.getItem('password');
}

function login(login_, password_) {
    localStorage.setItem('login', login_);
    localStorage.setItem('password', password_);
    window.location.replace("/");
};

function logout() {
    localStorage.removeItem('login');
    localStorage.removeItem('password');
    window.location.replace("/");
};

function is_logged() {
    return !(localStorage.getItem('login') === null);
}

function setup_navigation() {
    if (is_logged()) {
      $('nav ul').append(logout_list_element);
    } else {
      $('nav ul').append(login_list_element);
      $('nav ul').append(register_list_element);
    }
}

function getCookie(name) {
    var cookieValue = null;
    if (document.cookie && document.cookie !== '') {
        var cookies = document.cookie.split(';');
        for (var i = 0; i < cookies.length; i++) {
            var cookie = jQuery.trim(cookies[i]);
            if (cookie.substring(0, name.length + 1) === (name + '=')) {
                cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                break;
            }
        }
    }
    return cookieValue;
}

var csrftoken = getCookie('csrftoken');


function defaultProcessData(new_el, selector) {
    $(selector).append(new_el);
}

function subpageKey(str) {
    /* We might want to control creation of key
       for the localStore, eg. make it dependant on
       region_code
    */
    return str;
}

function defaultPost() {
}

function obtainData(selector_str, url, parseResponse,
                    processData = defaultProcessData,
                    postprocess = defaultPost) {
    var storage_key = subpageKey(selector_str);
    var id_data = localStorage.getItem(storage_key);
    var old_insert;

    if (!(id_data === null)) {
        old_insert = $($.parseHTML(id_data));
        processData(old_insert, selector_str);
    }

    $.ajax ({
        url: url,
        type: 'get',
        tryCount : 0,
        retryLimit : 3,
        success: function(data) {
            var new_data = parseResponse(data);

          	if (!(id_data === null)) {
                  old_insert.replaceWith(new_data);
          	} else {
    	 	       processData(new_data, selector_str);
    	    	}

          	localStorage.setItem(storage_key, new_data.prop("outerHTML"));
            postprocess();
        },
        error: function (request, error) {
          console.log(arguments);
          if (textStatus == 'timeout') {
            this.tryCount++;
            if (this.tryCount <= this.retryLimit) {
                $.ajax(this);
                return;
            }
            return;
          }
          if (xhr.status == 500) {
              show_message("Nie udało się pobrać danych");
          } else {
              show_message("Nie udało się nawiązać połączenia z serwerem");
          }
        },
    });
}

/* Top bar popup */

function show_message(msg, err = false) {
  $("#notification").text(msg);
  $("#notification").fadeIn("slow");
  $("#notification").delay(1000).fadeOut("slow");

  if (err) {
      $("#notification").addClass("error-notify");
  } else {
      $("#notification").addClass("success-notify");
  }
}

/***************/
