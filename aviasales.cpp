#include <fstream>
#include <iostream>
#include <string>

#include "database.hpp"

std::string get_command(const std::string& line) {
  size_t space_pos = line.find(' ');
  if (space_pos == std::string::npos) {
    return line;
  }
  return line.substr(0, space_pos);
}

std::string trim(const std::string& str) {
  size_t first = str.find_first_not_of(' ');
  if (first == std::string::npos) {
    return "";
  }
  size_t last = str.find_last_not_of(' ');
  return str.substr(first, (last - first + 1));
}

bool parse_datetime(const std::string& str, DateTime::datetime& out_dt) {
  size_t space_pos = str.find(' ');
  if (space_pos == std::string::npos) {
    std::cout << "Wrong date format: " << str << "\n";
    return false;
  }
  std::string date_str = trim(str.substr(0, space_pos));
  std::string time_str = trim(str.substr(space_pos + 1));

  size_t dot1 = date_str.find('.');
  size_t dot2 = date_str.find('.', dot1 + 1);
  if (dot1 == std::string::npos || dot2 == std::string::npos) {
    std::cout << "Wrong date format: " << date_str << "\n";
    return false;
  }

  std::string day_str = date_str.substr(0, dot1);
  std::string month_str = date_str.substr(dot1 + 1, dot2 - dot1 - 1);
  std::string year_str = date_str.substr(dot2 + 1);

  if (day_str.length() > 2 || month_str.length() > 2 || year_str.length() > 4) {
    std::cout << "Wrong date format: " << date_str << "\n";
    return false;
  }

  int day;
  int month;
  int year;
  try {
    day = std::stoi(day_str);
    month = std::stoi(month_str);
    year = std::stoi(year_str);
  } catch (...) {
    std::cout << "Wrong date format: " << date_str << "\n";
    return false;
  }

  if (month < 1 || month > 12) {
    std::cout << "Month value is invalid: " << month << "\n";
    return false;
  }

  int max_days = 31;
  if (month == 4 || month == 6 || month == 9 || month == 11) {
    max_days = 30;
  } else if (month == 2) {
    bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    max_days = leap ? 29 : 28;
  }
  if (day < 1 || day > max_days) {
    std::cout << "Day value is invalid: " << day << "\n";
    return false;
  }

  size_t colon = time_str.find(':');
  if (colon == std::string::npos) {
    std::cout << "Wrong time format: " << time_str << "\n";
    return false;
  }

  std::string hour_str = time_str.substr(0, colon);
  std::string min_str = time_str.substr(colon + 1);

  if (hour_str.length() > 2 || min_str.length() > 2) {
    std::cout << "Wrong time format: " << time_str << "\n";
    return false;
  }

  int hour;
  int minute;
  try {
    hour = std::stoi(hour_str);
    minute = std::stoi(min_str);
  } catch (...) {
    std::cout << "Wrong time format: " << time_str << "\n";
    return false;
  }

  if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
    std::cout << "Time value is invalid: " << time_str << "\n";
    return false;
  }
  try {
    out_dt = DateTime::datetime(year, month, day, hour, minute, 0);
  } catch (const std::invalid_argument& error) {
    std::cout << error.what() << "\n";
    return false;
  }
  return true;
}

bool parse_date(const std::string& str, int& out_year, int& out_month,
                int& out_day) {
  size_t dot1 = str.find('.');
  size_t dot2 = str.find('.', dot1 + 1);

  if (dot1 == std::string::npos || dot2 == std::string::npos) {
    std::cout << "Wrong date format: " << str << "\n";
    return false;
  }

  std::string day_str = str.substr(0, dot1);
  std::string month_str = str.substr(dot1 + 1, dot2 - dot1 - 1);
  std::string year_str = str.substr(dot2 + 1);

  if (day_str.length() > 2 || month_str.length() > 2 || year_str.length() > 4) {
    std::cout << "Wrong date format: " << str << "\n";
    return false;
  }

  int day;
  int month;
  int year;
  try {
    day = std::stoi(day_str);
    month = std::stoi(month_str);
    year = std::stoi(year_str);
  } catch (...) {
    std::cout << "Wrong date format: " << str << "\n";
    return false;
  }

  if (month < 1 || month > 12) {
    std::cout << "Month value is invalid: " << month << "\n";
    return false;
  }

  int max_days = 31;
  if (month == 4 || month == 6 || month == 9 || month == 11) {
    max_days = 30;
  } else if (month == 2) {
    bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    max_days = leap ? 29 : 28;
  }

  if (day < 1 || day > max_days) {
    std::cout << "Day value is invalid: " << day << "\n";
    return false;
  }

  out_year = year;
  out_month = month;
  out_day = day;
  return true;
}

void handle_add(std::string args, Database& db) {
  size_t pos = args.find(',');
  if (pos == std::string::npos) {
    std::cout << "Unknown command\n";
    return;
  }
  std::string flight = trim(args.substr(0, pos));
  args.erase(0, pos + 1);

  pos = args.find(',');
  if (pos == std::string::npos) {
    std::cout << "Unknown command\n";
    return;
  }
  std::string dep = trim(args.substr(0, pos));
  args.erase(0, pos + 1);

  pos = args.find(',');
  if (pos == std::string::npos) {
    std::cout << "Unknown command\n";
    return;
  }
  std::string arr = trim(args.substr(0, pos));
  args.erase(0, pos + 1);

  pos = args.find(',');
  if (pos == std::string::npos) {
    std::cout << "Unknown command\n";
    return;
  }
  std::string dt_dep_str = trim(args.substr(0, pos));
  args.erase(0, pos + 1);

  pos = args.find(',');
  if (pos == std::string::npos) {
    std::cout << "Unknown command\n";
    return;
  }
  std::string dt_arr_str = trim(args.substr(0, pos));
  args.erase(0, pos + 1);

  pos = args.find(',');
  if (pos == std::string::npos) {
    std::cout << "Unknown command\n";
    return;
  }
  std::string count_str = trim(args.substr(0, pos));
  args.erase(0, pos + 1);

  std::string price_str = trim(args);

  int count;
  double price;
  try {
    count = std::stoi(count_str);
    price = std::stod(price_str);
  } catch (...) {
    std::cout << "Unknown command\n";
    return;
  }
  DateTime::datetime dt_dep(2000, 1, 1);
  DateTime::datetime dt_arr(2000, 1, 1);
  if (!parse_datetime(dt_dep_str, dt_dep)) {
    return;
  }
  if (!parse_datetime(dt_arr_str, dt_arr)) {
    return;
  }

  if (dt_arr < dt_dep) {
    std::cout << "Wrong arrival time\n";
    return;
  }

  db.add(flight, dep, arr, dt_dep, dt_arr, count, price);
}

void handle_load(const std::string& filename, Database& db) {
  std::ifstream in(filename);
  if (!in.is_open()) {
    std::cout << "Can not open database\n";
    return;
  }

  db.reset();

  std::string line;
  while (std::getline(in, line)) {
    line = trim(line);
    if (!line.empty()) {
      handle_add(line, db);
    }
  }

  in.close();
}

int main() {
  Database db;
  std::string line;

  while (true) {
    std::cout << "> ";

    if (!std::getline(std::cin, line)) {
      break;
    }

    line = trim(line);

    if (line.empty()) {
      continue;
    }

    std::string cmd = get_command(line);

    if (line == "quit") {
      break;
    }
    if (line == "print") {
      db.print();
    } else if (line == "total records") {
      std::cout << db.get_total_records() << "\n";
    } else if (line == "total tickets") {
      std::cout << db.get_total_tickets() << "\n";
    } else if (cmd == "add") {
      if (line.size() <= 4) {
        std::cout << "Unknown command\n";
      } else {
        std::string args = line.substr(4);
        handle_add(args, db);
      }
    } else if (line == "clear") {
      db.clear();
    } else if (cmd == "delete") {
      if (line.size() <= 7) {
        std::cout << "Unknown command\n";
      } else {
        std::string id_str = trim(line.substr(7));
        try {
          int id = std::stoi(id_str);
          db.delete_id(id);
        } catch (...) {
          std::cout << "Unknown command\n";
        }
      }
    } else if (cmd == "departure") {
      if (line.size() <= 10) {
        std::cout << "Unknown command\n";
      } else {
        std::string args = trim(line.substr(9));
        size_t space_pos = args.find(' ');

        if (space_pos == std::string::npos) {
          std::cout << "Unknown command\n";
        } else {
          std::string airport = trim(args.substr(0, space_pos));
          std::string date_str = trim(args.substr(space_pos + 1));

          int y;
          int m;
          int d;
          if (parse_date(date_str, y, m, d)) {
            db.print_departure(airport, y, m, d);
          }
        }
      }
    } else if (cmd == "arrival") {
      if (line.size() <= 8) {
        std::cout << "Unknown command\n";
      } else {
        std::string args = trim(line.substr(7));
        size_t space_pos = args.find(' ');

        if (space_pos == std::string::npos) {
          std::cout << "Unknown command\n";
        } else {
          std::string airport = trim(args.substr(0, space_pos));
          std::string date_str = trim(args.substr(space_pos + 1));

          int y;
          int m;
          int d;
          if (parse_date(date_str, y, m, d)) {
            db.print_arrival(airport, y, m, d);
          }
        }
      }
    } else if (cmd == "schedule") {
      if (line.size() <= 9) {
        std::cout << "Unknown command\n";
      } else {
        std::string args = trim(line.substr(8));
        size_t space_pos = args.find(' ');

        if (space_pos == std::string::npos) {
          std::cout << "Unknown command\n";
        } else {
          std::string airport = trim(args.substr(0, space_pos));
          std::string date_str = trim(args.substr(space_pos + 1));

          int y;
          int m;
          int d;
          if (parse_date(date_str, y, m, d)) {
            db.print_schedule(airport, y, m, d);
          }
        }
      }
    } else if (cmd == "buy") {
      if (line.size() <= 4) {
        std::cout << "Unknown command\n";
      } else {
        std::string args = trim(line.substr(3));
        size_t space_pos = args.find(' ');

        if (space_pos == std::string::npos) {
          std::cout << "Unknown command\n";
        } else {
          std::string flight = trim(args.substr(0, space_pos));
          std::string date_str = trim(args.substr(space_pos + 1));

          int y;
          int m;
          int d;
          if (parse_date(date_str, y, m, d)) {
            db.buy_ticket(flight, y, m, d);
          }
        }
      }
    } else if (cmd == "from") {
      if (line.size() <= 5) {
        std::cout << "Unknown command\n";
      } else {
        std::string args = trim(line.substr(4));
        size_t to_pos = args.find(" to ");

        if (to_pos == std::string::npos) {
          std::cout << "Unknown command\n";
        } else {
          std::string dep = trim(args.substr(0, to_pos));
          args = trim(args.substr(to_pos + 4));

          size_t on_pos = args.find(" on ");
          if (on_pos == std::string::npos) {
            std::cout << "Unknown command\n";
          } else {
            std::string arr = trim(args.substr(0, on_pos));
            std::string date_str = trim(args.substr(on_pos + 4));
            int y;
            int m;
            int d;
            if (parse_date(date_str, y, m, d)) {
              db.print_from_to(dep, arr, y, m, d);
            }
          }
        }
      }
    } else if (cmd == "find") {
      if (line.size() <= 5) {
        std::cout << "Unknown command\n";
      } else {
        std::string args = trim(line.substr(4));
        size_t to_pos = args.find(" to ");
        ;
        if (to_pos == std::string::npos) {
          std::cout << "Unknown command\n";
        } else {
          std::string dep = trim(args.substr(0, to_pos));
          std::string arr = trim(args.substr(to_pos + 4));

          db.print_find(dep, arr);
        }
      }
    } else if (cmd == "export") {
      if (line.size() <= 7) {
        std::cout << "Unknown command\n";
      } else {
        std::string filename = trim(line.substr(7));

        if (filename.empty()) {
          std::cout << "Unknown command\n";
        } else {
          db.export_to_file(filename);
        }
      }
    } else if (cmd == "save") {
      if (line.size() <= 5) {
        std::cout << "Unknown command\n";
      } else {
        std::string filename = trim(line.substr(5));

        if (filename.empty()) {
          std::cout << "Unknown command\n";
        } else {
          db.save(filename);
        }
      }
    } else if (cmd == "load") {
      if (line.size() <= 5) {
        std::cout << "Unknown command\n";
      } else {
        std::string filename = trim(line.substr(5));

        if (filename.empty()) {
          std::cout << "Unknown command\n";
        } else {
          handle_load(filename, db);
        }
      }
    } else if (cmd == "bothways") {
      if (line.size() <= 9) {
        std::cout << "Unknown command\n";
      } else {
        std::string args = trim(line.substr(8));
        size_t space_pos = args.find(' ');
        if (space_pos == std::string::npos) {
          std::cout << "Unknown command\n";
        } else {
          std::string dep = trim(args.substr(0, space_pos));
          std::string arr = trim(args.substr(space_pos + 1));

          db.bothways(dep, arr);
        }
      }
    } else {
      std::cout << "Unknown command: " << cmd << "\n";
    }
  }

  return 0;
}