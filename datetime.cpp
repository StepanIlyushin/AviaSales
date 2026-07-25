#include "datetime.hpp"

#include <string>

namespace DateTime {

static int get_days_in_month(int year, int month) {
  if (month == 2) {
    return date::is_leapyear(year) ? 29 : 28;
  }
  if (month == 4 || month == 6 || month == 9 || month == 11) {
    return 30;
  }
  return 31;
}

static int count_days_from_0(const date& dt) {
  int y = dt.year();

  int total_days = 0;

  for (int i = 1; i < y; ++i) {
    if (date::is_leapyear(i)) {
      total_days += 366;
    } else {
      total_days += 365;
    }
  }

  int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  for (int i = 1; i < dt.month(); ++i) {
    total_days += days_in_month[i];
  }

  if (dt.month() > 2 && date::is_leapyear(dt.year())) {
    total_days += 1;
  }

  total_days += dt.day();

  return total_days;
}

time::time(int hour, int minute, int second) {
  if (hour < 0 || hour >= 24) {
    throw std::invalid_argument("hours must be between 0 and 23");
  }
  if (minute < 0 || minute >= 60) {
    throw std::invalid_argument("minutes must be between 0 and 59");
  }
  if (second < 0 || second >= 60) {
    throw std::invalid_argument("seconds must be between 0 and 59");
  }
  hour_ = static_cast<int8_t>(hour);
  minute_ = static_cast<int8_t>(minute);
  second_ = static_cast<int8_t>(second);
}

int time::hour() const { return hour_; }

int time::minute() const { return minute_; }

int time::second() const { return second_; }

void time::add_hours(int nb_hours) { add_seconds(nb_hours * 3600); }

void time::add_minutes(int nb_minutes) { add_seconds(nb_minutes * 60); }

void time::add_seconds(int nb_seconds) {
  long long total = hour_ * 3600LL + minute_ * 60LL + second_ + nb_seconds;

  if (total < 0) {
    total = 0;
  }

  if (total > 86399) {
    total = 86399;
  }

  hour_ = static_cast<int8_t>(total / 3600);
  minute_ = static_cast<int8_t>((total / 60) % 60);
  second_ = static_cast<int8_t>(total % 60);
}

bool time::operator==(const time& other) const {
  return (hour_ == other.hour_ && minute_ == other.minute_ &&
          second_ == other.second_);
}

bool time::operator!=(const time& other) const { return !(*this == other); }

bool time::operator<(const time& other) const {
  int this_total_seconds = hour_ * 3600 + minute_ * 60 + second_;
  int other_total_seconds =
      other.hour_ * 3600 + other.minute_ * 60 + other.second_;
  return this_total_seconds < other_total_seconds;
}

bool time::operator>(const time& other) const { return other < *this; }

bool time::operator<=(const time& other) const {
  return (*this < other) || (*this == other);
}

bool time::operator>=(const time& other) const { return !(*this < other); }

timediff time::operator-(const time& other) const {
  int this_total_seconds = hour_ * 3600 + minute_ * 60 + second_;
  int other_total_seconds =
      other.hour_ * 3600 + other.minute_ * 60 + other.second_;

  int diff_sec = this_total_seconds - other_total_seconds;

  int h = diff_sec / 3600;
  int rem = diff_sec % 3600;

  int m = rem / 60;
  int s = rem % 60;

  return timediff(0, h, m, s);
}

std::string to_string(const time& t) {
  std::string result;

  if (t.hour() < 10) {
    result += "0";
  }
  result += std::to_string(t.hour()) + ":";

  if (t.minute() < 10) {
    result += "0";
  }
  result += std::to_string(t.minute()) + ":";

  if (t.second() < 10) {
    result += "0";
  }
  result += std::to_string(t.second());

  return result;
}

timediff::timediff(int days, int hours, int minutes, int seconds) {
  if (hours < -23 || hours > 23) {
    throw std::invalid_argument("hours must be between -23 and 23");
  }
  if (minutes < -59 || minutes > 59) {
    throw std::invalid_argument("minutes must be between -59 and 59");
  }
  if (seconds < -59 || seconds > 59) {
    throw std::invalid_argument("seconds must be between -59 and 59");
  }

  bool has_positive = (days > 0 || hours > 0 || minutes > 0 || seconds > 0);
  bool has_negative = (days < 0 || hours < 0 || minutes < 0 || seconds < 0);

  if (has_negative && has_positive) {
    throw std::invalid_argument("all arguments must have the same sign");
  }

  days_ = static_cast<int32_t>(days);
  hours_ = static_cast<int8_t>(hours);
  minutes_ = static_cast<int8_t>(minutes);
  seconds_ = static_cast<int8_t>(seconds);
}

int timediff::days() const { return days_; }
int timediff::hours() const { return hours_; }
int timediff::minutes() const { return minutes_; }
int timediff::seconds() const { return seconds_; }

int timediff::total_hours() const { return days_ * 24 + hours_; }

int timediff::total_minutes() const { return total_hours() * 60 + minutes_; }

int timediff::total_seconds() const { return total_minutes() * 60 + seconds_; }

bool timediff::operator==(const timediff& other) const {
  return this->total_seconds() == other.total_seconds();
}

bool timediff::operator!=(const timediff& other) const {
  return !(*this == other);
}

bool timediff::operator<(const timediff& other) const {
  return this->total_seconds() < other.total_seconds();
}

bool timediff::operator>(const timediff& other) const { return other < *this; }

bool timediff::operator<=(const timediff& other) const {
  return (*this < other) || (*this == other);
}

bool timediff::operator>=(const timediff& other) const {
  return !(*this < other);
}

timediff timediff::operator+(const timediff& other) const {
  int total_sec = this->total_seconds() + other.total_seconds();

  int d = total_sec / (3600 * 24);
  int rem = total_sec % (3600 * 24);
  int h = rem / 3600;
  rem = rem % 3600;
  int m = rem / 60;
  int s = rem % 60;

  return timediff(d, h, m, s);
}

timediff timediff::operator-(const timediff& other) const {
  int total_sec = this->total_seconds() - other.total_seconds();

  int d = total_sec / (3600 * 24);
  int rem = total_sec % (3600 * 24);
  int h = rem / 3600;
  rem = rem % 3600;
  int m = rem / 60;
  int s = rem % 60;

  return timediff(d, h, m, s);
}

bool date::is_leapyear(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool date::is_leapyear() const { return is_leapyear(year_); }

date::date(int year, int month, int day) {
  if (month < 1 || month > 12) {
    throw std::invalid_argument("month must be between 1 and 12");
  }

  int max_days = 31;

  if (month == 4 || month == 6 || month == 9 || month == 11) {
    max_days = 30;
  } else if (month == 2) {
    if (is_leapyear(year)) {
      max_days = 29;
    } else {
      max_days = 28;
    }
  }

  if (day < 1 || day > max_days) {
    throw std::invalid_argument("day is invalid for the given month and year");
  }

  year_ = static_cast<int16_t>(year);
  month_ = static_cast<int8_t>(month);
  day_ = static_cast<int8_t>(day);
}

int date::year() const { return year_; }

int date::month() const { return month_; }

int date::day() const { return day_; }

DateTime::weekday date::weekday() const {
  int m = static_cast<unsigned char>(month_);
  int y = year_;
  int d = static_cast<unsigned char>(day_);

  if (m < 3) {
    m += 12;
    y -= 1;
  }

  int k = y % 100;
  int j = y / 100;

  int h = (d + 13 * (m + 1) / 5 + k + k / 4 + j / 4 + 5 * j) % 7;

  int day_index = (h + 6) % 7;

  return static_cast<DateTime::weekday>(day_index);
}

void date::add_days(int nb_days) {
  if (nb_days > 0) {
    for (int i = 0; i < nb_days; ++i) {
      day_++;

      if (day_ > get_days_in_month(year_, month_)) {
        day_ = 1;
        month_++;

        if (month_ > 12) {
          month_ = 1;
          year_++;
        }
      }
    }
  } else if (nb_days < 0) {
    for (int i = 0; i > nb_days; --i) {
      day_--;

      if (day_ < 1) {
        month_--;

        if (month_ < 1) {
          month_ = 12;
          year_--;
        }
        day_ = static_cast<int8_t>(get_days_in_month(year_, month_));
      }
    }
  }
}

date date::next() const {
  date res = *this;
  res.add_days(1);
  return res;
}

date date::prev() const {
  date res = *this;
  res.add_days(-1);
  return res;
}

date& date::operator++() {
  this->add_days(1);
  return *this;
}

date date::operator++(int) {
  date temp = *this;
  this->add_days(1);
  return temp;
}

date& date::operator--() {
  this->add_days(-1);
  return *this;
}

date date::operator--(int) {
  date temp = *this;
  this->add_days(-1);
  return temp;
}

bool date::operator==(const date& other) const {
  return year_ == other.year_ && month_ == other.month_ && day_ == other.day_;
}

bool date::operator!=(const date& other) const { return !(*this == other); }

bool date::operator<(const date& other) const {
  if (year_ != other.year_) {
    return year_ < other.year_;
  }

  if (month_ != other.month_) {
    return month_ < other.month_;
  }

  return day_ < other.day_;
}

bool date::operator>(const date& other) const { return other < *this; }

bool date::operator<=(const date& other) const {
  return (*this < other) || (*this == other);
}

bool date::operator>=(const date& other) const { return !(*this < other); }

timediff date::operator-(const date& other) const {
  int days_this = count_days_from_0(*this);
  int days_other = count_days_from_0(other);

  int diff = days_this - days_other;

  return timediff(diff, 0, 0, 0);
}

std::string to_string(const date& d) {
  std::string result = std::to_string(d.year()) + ".";

  if (d.month() < 10) {
    result += "0";
  }
  result += std::to_string(d.month()) + ".";

  if (d.day() < 10) {
    result += "0";
  }
  result += std::to_string(d.day());

  return result;
}

datetime::datetime(int year, int month, int day, int hour, int minute,
                   int second)
    : date_(year, month, day), time_(hour, minute, second) {}

datetime::datetime(const date& dt, const time& tm) : date_(dt), time_(tm) {}

int datetime::year() const { return date_.year(); }
int datetime::month() const { return date_.month(); }
int datetime::day() const { return date_.day(); }
int datetime::hour() const { return time_.hour(); }
int datetime::minute() const { return time_.minute(); }
int datetime::second() const { return time_.second(); }
DateTime::weekday datetime::weekday() const { return date_.weekday(); }
bool datetime::is_leapyear() const { return date_.is_leapyear(); }
bool datetime::is_leapyear(int year) { return date::is_leapyear(year); }

void datetime::add_days(int nb_days) { date_.add_days(nb_days); }

void datetime::add_minutes(int nb_minutes) { add_seconds(nb_minutes * 60); }

void datetime::add_hours(int nb_hours) { add_seconds(nb_hours * 3600); }

void datetime::add_seconds(int nb_seconds) {
  long long total_s = time_.hour() * 3600LL + time_.minute() * 60LL +
                      time_.second() + nb_seconds;

  long long days_change = total_s / 86400;
  total_s = total_s % 86400;

  if (total_s < 0) {
    days_change -= 1;
    total_s += 86400;
  }

  date_.add_days(static_cast<int>(days_change));

  time_ = time(static_cast<int>(total_s / 3600),
               static_cast<int>((total_s / 60) % 60),
               static_cast<int>(total_s % 60));
}

bool datetime::operator==(const datetime& other) const {
  return (date_ == other.date_ && time_ == other.time_);
}

bool datetime::operator!=(const datetime& other) const {
  return !(*this == other);
}

bool datetime::operator<(const datetime& other) const {
  if (date_ == other.date_) {
    return time_ < other.time_;
  }
  return date_ < other.date_;
}

bool datetime::operator>(const datetime& other) const { return other < *this; }

bool datetime::operator<=(const datetime& other) const {
  return (*this < other) || (*this == other);
}

bool datetime::operator>=(const datetime& other) const {
  return !(*this < other);
}

timediff datetime::operator-(const datetime& other) const {
  timediff d_diff = date_ - other.date_;
  timediff t_diff = time_ - other.time_;

  return d_diff + t_diff;
}

datetime datetime::operator+(const timediff& ts) const {
  datetime res = *this;

  res.add_days(ts.days());
  res.add_hours(ts.hours());
  res.add_minutes(ts.minutes());
  res.add_seconds(ts.seconds());

  return res;
}

datetime datetime::operator-(const timediff& ts) const {
  datetime res = *this;

  res.add_days(-ts.days());
  res.add_hours(-ts.hours());
  res.add_minutes(-ts.minutes());
  res.add_seconds(-ts.seconds());

  return res;
}

static std::string pad_zero(int num) {
  if (num < 10) {
    return "0" + std::to_string(num);
  }
  return std::to_string(num);
}

std::string to_string(const datetime& dt, const char* format) {
  std::string result;
  const char* p = format;

  const char* month_names[] = {
      "",     "January", "February",  "March",   "April",    "May",     "June",
      "July", "August",  "September", "October", "November", "December"};
  const char* month_short[] = {"",    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                               "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  const char* day_names[] = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                             "Thursday", "Friday", "Saturday"};
  const char* day_short[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

  while (*p != '\0') {
    if (*p == '%') {
      p++;
      if (*p == '\0') {
        result += '%';
        break;
      }

      switch (*p) {
        case 'Y':
          result += std::to_string(dt.year());
          break;

        case 'y':
          result += pad_zero(dt.year() % 100);
          break;
        case 'm':
          result += pad_zero(dt.month());
          break;
        case 'd':
          result += pad_zero(dt.day());
          break;
        case 'b':
          result += month_short[dt.month()];
          break;
        case 'B':
          result += month_names[dt.month()];
          break;
        case 'a':
          result += day_short[static_cast<int>(dt.weekday())];
          break;
        case 'A':
          result += day_names[static_cast<int>(dt.weekday())];
          break;
        case 'H':
          result += pad_zero(dt.hour());
          break;
        case 'M':
          result += pad_zero(dt.minute());
          break;
        case 'S':
          result += pad_zero(dt.second());
          break;
        default:
          result += '%';
          result += *p;
          break;
      }
    } else {
      result += *p;
    }

    p++;
  }

  return result;
}

}  // namespace DateTime