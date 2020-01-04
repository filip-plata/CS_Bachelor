/* Provided by base.js */
var region_code;
/***********************/

var candidates_url = "/api/region/".concat(region_code, "/candidates/");
var can_edit = (region_code.length == 8);
var votes_update_url = "/api/votes-update/";


function regionPostProcess() {
      var old_value;
      var new_value;
      var err_msg_wrong_data = "Wystąpił błąd. Czy liczba głosów jest nieujemna?";
      var err_msg_wrong_auth = "Musis być zalogowany aby zmieniać ilość głosów";
      var suc_msg = "Zapisano zmiany";

      $('.edit').click(function(){
        $(this).addClass('editMode');
        old_value = $(this).text();
      });

      $(".edit").focusout(function(){
        $(this).removeClass("editMode");
        var id = this.id;
        var split_id = id.split("_");
        var candidate_id = split_id[1];
        new_value = $(this).text();

        if (new_value != old_value) {
          $.ajax({
            url: votes_update_url,
            type: 'post',
            data: { 'candidate-id': candidate_id,
                    'region-code': region_code,
                    'votes': new_value,
                    'login': get_login(),
                    'password': get_password(),
                    'csrfmiddlewaretoken': csrftoken,
            },
            error: function (jqXHR, exception) {
                console.log(arguments);
                if (jqXHR.status == 403) {
                  show_message(err_msg_wrong_data, true);
                } else if (jqXHR.status == 401) {
                  show_message(err_msg_wrong_auth, true);
                }
            },
            success: function () {
                show_message(suc_msg);
                $("#votes-summary").empty();
                obtainData("#votes-summary", summary_url, parseSummary);
            }
          });
        }
      });
}


function processCandidates(candidates_tb, selector_str) {
    $(selector_str).append(candidates_tb);
}

function parseRow(data_dict, index) {
    var result = $('<tr></tr>');
    var index_col = $('<td></td>');
    var name_col = $('<td></td>');
    var result_col = $('<td><div></div></td>');
    var percents_col = $('<td><div class="progress"></div></td>');
    var percents_col_inner = $('<div class="progress-bar" role="progressbar" aria-valuemin="0" aria-valuemax="100"></div>');
    var percents_col_second = $('<div></div>');

    index_col.text(index);
    name_col.text(data_dict['name'] + ' ' + data_dict['surname']);
    percents_col_inner.attr('style', "width: " +
                            data_dict['percent_results'] + "%");
    percents_col_second.text(data_dict['percent_results'].toFixed(2) + '%');

    $('div', result_col).attr('id', "votes_" + data_dict['id']);
    if (can_edit) {
        $('div', result_col).addClass("edit");
        $('div', result_col).attr('contenteditable', "true");
    }
    $('div', result_col).text(data_dict['candidate_result']);
    $('div', percents_col).append(percents_col_inner);

    percents_col.append(percents_col_second);

    result.append(index_col);
    result.append(name_col);
    result.append(result_col);
    result.append(percents_col);

    return result;
}

function parseCandidates(candidates_json) {
  var candidates_list = candidates_json['data'];
  var list_size = candidates_json['size'];
  var result = $('<tbody></tbody>');

  for (var i = 0; i < list_size; i++) {
      result.append(parseRow(candidates_list[i], i+1));
  }

  return result;
}

$(document).ready(function() {
    obtainData("#results-candidates > table", candidates_url,
                parseCandidates, processCandidates, regionPostProcess);
});
