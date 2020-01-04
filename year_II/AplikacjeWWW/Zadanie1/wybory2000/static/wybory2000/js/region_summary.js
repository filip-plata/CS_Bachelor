var summary_url = "/api/region/".concat(region_code, "/summary/");


function parseSummary(summary_json) {
  var result = $('<ul></ul>');

  var children_items =
        [$('<li>Suma głosów<span></span</li>'),
         $('<li>Głosy ważne<span></span</li>'),
         $('<li>Głosy nieważne<span></span</li>'),
         $('<li>Wydane karty do głosowania<span></span</li>')];

  var data_list = [summary_json['votes_total'],
                   summary_json['votes_valid'],
                   summary_json['votes_invalid'],
                   summary_json['ballot_papers']];

  for (var i = 0; i<4; i++) {
    $('span', children_items[i]).text(data_list[i]);
    result.append(children_items[i]);
  }

  return result;
}

$(document).ready(function() {
    obtainData("#votes-summary", summary_url, parseSummary);
});
