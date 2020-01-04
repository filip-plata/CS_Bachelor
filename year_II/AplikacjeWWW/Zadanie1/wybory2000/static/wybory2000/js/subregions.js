var subregions_url = "/api/region/".concat(region_code, "/subregions/");
var legend_url = "/api/legend/";


function processSubregions(subregions, selector) {
    $(selector).append(subregions);
}

function postProcessSubregions() {
  if ($('#subregions tbody').is(':empty')) {
      $('#subregions').hide();
  } else {
      $('#subregions').show();
  }
}

function processLegend(legend_node, selector) {
    $(selector + " > h2").after(legend_node);
}

function parseResultsBar(bar_json) {
  var list_size = Number(bar_json['size']);
  var data_list = bar_json['data']
  var result = $('<td class="large-display"><div class="candidate-results-bar"></div></td>');
  var div_inside = $('div', result);

  /* Code below is raw html for performance reasons.
    It could hang the browser if it was jQuery, this loop
    could be executed more than 500 times */

  for (var j = 0; j < list_size; j++) {
      var res = '<div class="candidate-res" style="background-color: ' + data_list[j]['color'] +'; width:' + data_list[j]['percent_results'] + '%;"></div>';

      div_inside.append(res);
  }

  return result;
};

function parseSubregionRow(row_json, index) {
  var summary = row_json['summaries'];
  var result = $('<tr></tr>');

  var code_col = $('<td></td>');
  var link_col = $('<td><a></a></td>');
  var valid_votes_col = $('<td></td>');
  var results_bar_col = parseResultsBar(row_json['results']);
  var ballot_papers_col = $('<td class="large-display"></td>');

  ballot_papers_col.text(summary['ballot_papers']);
  code_col.text(summary['code']);
  $('a', link_col).text(summary['str_name']);
  $('a', link_col).attr('href', summary['url']);
  valid_votes_col.text(summary['votes_valid']);

  result.append(code_col);
  result.append(link_col);
  result.append(valid_votes_col);
  result.append(results_bar_col);
  result.append(ballot_papers_col);

  return result;
};

function parseSubregions(subregions_json) {
  var subregions_list = subregions_json['data'];
  var list_size = Number(subregions_json['size']);
  var result = $('<tbody></tbody>');

  for (var i = 0; i < list_size; i++) {
      result.append(parseSubregionRow(subregions_list[i], i));
  }

  return result;
};

function parseSubregionLegendRow(row_json, index) {
    var result = $('<div><span></span></div>');

    var inside_div = $('<div></div>');
    $('span', result).text(row_json['surname']);
    inside_div.css("background", row_json['color']);
    result.append(inside_div);

    return result;
}

function parseSubregionsLegend(legend_json) {
    var legend_list = legend_json['data'];
    var list_size = Number(legend_json['size']);
    var result = $('<div></div>');

    for (var i = 0; i < list_size; i++) {
        result.append(parseSubregionLegendRow(legend_list[i], i));
    }

    return result;
}

$(document).ready(function() {
    obtainData("#subregions > table", subregions_url,
                parseSubregions, processSubregions, postProcessSubregions);
    obtainData("#subregions", legend_url,
               parseSubregionsLegend, processLegend);

    var socket = new WebSocket('ws://' + window.location.host + '/region/');

    socket.onmessage = function(e) {
        if (!e.data.startsWith(region_code)) {
            return;
        }
        /* Poor's man version of data switching */
        $("#votes-summary ul").remove();
        $("#results-candidates tbody").remove();
        obtainData("#votes-summary", summary_url, parseSummary);
        obtainData("#results-candidates > table", candidates_url,
                    parseCandidates, processCandidates, regionPostProcess);

        show_message("Zmieniono liczbę głosów w regionie", false);
    };
});
