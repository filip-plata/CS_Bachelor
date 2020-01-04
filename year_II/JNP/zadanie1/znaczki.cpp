#include <iostream>
#include <tuple>
#include <regex>
#include <algorithm>
#include <vector>

using namespace std;

// Stored information: year, post or country name,
// price, stamp name, price in string format.
typedef tuple<int, string, double, string, string> POST_STAMP_DATA;
typedef pair<int, int> INTERVAL;
typedef pair<vector<POST_STAMP_DATA>, vector<INTERVAL>> STAMPS_AND_INTERVALS;

const string date_string = "([1-9][[:digit:]]{3})";

const regex date_interval =
    regex("^\\s*" + date_string + "\\s+" + date_string + "\\s*$");
const regex post_stamp_data =
    regex("^\\s*(.+)\\s+([0-9]{1,8}(?:[\\.|,]{1}[0-9]{1,2})"
          "?)\\s+" +
          date_string + "\\s+(.*\\S)\\s*$");

// ############# UTILS

string rtrim(string str) {
  return str.erase(str.find_last_not_of(" \n\r\t") + 1);
}

string remove_multiple_whitespaces(string str) {
  return regex_replace(str, regex("[' ']{2,}"), " ");
}

string replace_comma_by_dot(string str) {
  return regex_replace(str, regex("[',']{1}"), ".");
}

bool cmp_only_years(const POST_STAMP_DATA &a, const POST_STAMP_DATA &b) {
  return get<0>(a) < get<0>(b);
}

// ############# END UTILS

// ############# INTERVAL

bool is_valid_interval(INTERVAL dates) { return dates.first <= dates.second; }

INTERVAL parse_interval(smatch sm) {
  return make_pair(stoi(sm[1]), stoi(sm[2]));
}

// ############ END INTERVAL

// ############ POST_STAMP_DATA

POST_STAMP_DATA parse_post_stamp(smatch sm) {

  string country_name = rtrim(remove_multiple_whitespaces(sm[4]));
  int year = stoi(sm[3]);
  string value_str = sm[2];
  double value = stod(replace_comma_by_dot(value_str));
  string name = rtrim(remove_multiple_whitespaces(sm[1]));

  return make_tuple(year, country_name, value, name, value_str);
}

// ############## END POST_STAMP_DATA

// ############## MAIN PARSE

void print_error(string line, int line_number) {
  cerr << "Error in line " << line_number << ":" << line << endl;
}

STAMPS_AND_INTERVALS parse_data() {
  string line;
  bool end_of_stamps = false;
  STAMPS_AND_INTERVALS data;
  int line_number = 0;
  smatch sm;

  while (getline(cin, line)) {
    line_number++;

    if (regex_match(line, sm, date_interval)) {
      INTERVAL dates = parse_interval(sm);

      if (!is_valid_interval(dates)) {
        print_error(line, line_number);
      } else {
        data.second.push_back(dates);
        end_of_stamps = true;
      }

      continue;
    }

    if (!end_of_stamps && regex_match(line, sm, post_stamp_data)) {
      data.first.push_back(parse_post_stamp(sm));
    } else {
      print_error(line, line_number);
    }
  }

  return data;
}

// ############ END MAIN PARSE

// ############ PRINT SOLUTION

void print_record(POST_STAMP_DATA record) {
  cout << get<0>(record) << " " << get<1>(record) << " " << get<4>(record)
       << " " << get<3>(record) << "\n";
}

void print_solution(vector<POST_STAMP_DATA> post_stamps,
                    vector<INTERVAL> intervals) {
  for (const INTERVAL interval : intervals) {
    POST_STAMP_DATA start_date(interval.first, {}, {}, {}, {});
    POST_STAMP_DATA end_date(interval.second, {}, {}, {}, {});
    auto stamps_begin = lower_bound(post_stamps.begin(), post_stamps.end(),
                                    start_date, cmp_only_years);
    auto stamps_end = upper_bound(post_stamps.begin(), post_stamps.end(),
                                  end_date, cmp_only_years);
    for_each(stamps_begin, stamps_end, print_record);
  }
}

// ############ END PRINT SOLUTION

int main() {
  vector<POST_STAMP_DATA> post_stamps;
  vector<INTERVAL> intervals;
  std::tie(post_stamps, intervals) = parse_data();
  sort(post_stamps.begin(), post_stamps.end());
  print_solution(post_stamps, intervals);
  return 0;
}
