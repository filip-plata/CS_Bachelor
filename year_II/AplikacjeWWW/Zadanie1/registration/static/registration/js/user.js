var login_url = "/user/login-api/";
var register_url = "/user/register-api/";

$(document).ready(function () {
    var login_form = $("#login-form");
    var register_form = $("#register-form");

    login_form.find('button').click(function() {
        var login_ = login_form.find('input[name="login"]').val();
        var password_ = login_form.find('input[name="hasło"]').val();

        $.ajax ({
            url: login_url,
            type: 'POST',
            data: {'login': login_,
                   'password': password_,
                    'csrfmiddlewaretoken': csrftoken,
                  },
            tryCount : 0,
            retryLimit : 3,
            success: function(data) {
                login(login_, password_);
            },
            error: function (request, error) {
              console.log(arguments);
              show_message("Login lub hasło są nieprawidłowe", true);
            },
        });
    });

    register_form.find('button').click(function() {
        var login_ = register_form.find('input[name="username"]').val();
        var password1_ = register_form.find('input[name="password1"]').val();
        var password2_ = register_form.find('input[name="password2"]').val();

        if (password1_ != password1_) {
          show_message("Wpisane hasła są różne", true);
          return;
        }

        if (login_ == "") {
          show_message("Login nie może być pusty", true);
          return;
        }

        if (password1_.length < 8) {
          show_message("Hasło powinno mieć przynajmniej 8 znaków", true);
          return;
        }

        $.ajax ({
            url: register_url,
            type: 'POST',
            data: {'login': login_,
                   'password': password1_,
                    'csrfmiddlewaretoken': csrftoken,
                  },
            tryCount : 0,
            retryLimit : 3,
            success: function(data) {
                show_message('Rejestracja przebiegła pomyślnie');
            },
            error: function (request, error) {
              console.log(arguments);
              show_message("Login jest już zajęty", true);
            },
        });
    });
});
