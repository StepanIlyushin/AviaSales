#pragma once
#include <cstdint>
#include <stdexcept>
#include <string>

namespace DateTime {

enum class weekday {
  sunday,
  monday,
  tuesday,
  wednesday,
  thursday,
  friday,
  saturday
};

class timediff {
 private:
  int32_t days_;
  int8_t hours_;
  int8_t minutes_;
  int8_t seconds_;

 public:
  timediff(int days, int hours, int minutes, int seconds);

  int days() const;     // Получить количество дней
  int hours() const;    // Получить количество часов
  int minutes() const;  // Получить количество минут
  int seconds() const;  // Получить количество секунд
  int total_hours()
      const;  // Получить общее количество часов (с учётом количества дней)
  int total_minutes() const;  // Получить общее количество минут
  int total_seconds() const;  // Получить общее количество секунд

  bool operator==(const timediff& other) const;
  bool operator!=(const timediff& other) const;
  bool operator<(const timediff& other) const;
  bool operator<=(const timediff& other) const;
  bool operator>(const timediff& other) const;
  bool operator>=(const timediff& other) const;

  timediff operator+(const timediff& other) const;
  timediff operator-(const timediff& other) const;
};

class time {
 private:
  int8_t hour_;
  int8_t minute_;
  int8_t second_;

 public:
  time(int hour, int minute, int second);
  int hour() const;    // Получить количество часов
  int minute() const;  // Получить количество минут
  int second() const;  // Получить количество секунд

  void add_hours(int nb_hours);  // Добавить заданное количество часов
  void add_minutes(int nb_minutes);  // Добавить заданное количество минут
  void add_seconds(int nb_seconds);  // Добавить заданное количество секунд

  bool operator==(const time& other) const;
  bool operator!=(const time& other) const;
  bool operator<(const time& other) const;
  bool operator<=(const time& other) const;
  bool operator>(const time& other) const;
  bool operator>=(const time& other) const;

  timediff operator-(const time& other) const;
};

std::string to_string(
    const time&);  // Преобразовать в строку в формате "HH:mm:ss"

class date {
 private:
  int16_t year_;
  int8_t month_;
  int8_t day_;

 public:
  date(int year, int month, int day);

  int year() const;                   // Получить год
  int month() const;                  // Получить месяц
  int day() const;                    // Получить день
  DateTime::weekday weekday() const;  // Получить день недели для даты
  bool is_leapyear() const;  // true, если год високосностный, иначе false

  void add_days(int nb_days);  // Добавить заданное количество дней

  date next() const;  // Получить дату для следующего дня
  date prev() const;  // Получить дату для предыдущего дня

  static bool is_leapyear(int year);  // Проверка года на високосность

  bool operator==(const date& other) const;
  bool operator!=(const date& other) const;
  bool operator<(const date& other) const;
  bool operator<=(const date& other) const;
  bool operator>(const date& other) const;
  bool operator>=(const date& other) const;

  timediff operator-(const date& other) const;

  date& operator++();
  date operator++(int);
  date& operator--();
  date operator--(int);
};

std::string to_string(
    const date&);  // Преобразовать в строку в формате "yyyy.MM.dd"

class datetime {
 private:
  date date_;
  time time_;

 public:
  datetime(int year, int month, int day, int hour = 0, int minute = 0,
           int second = 0);
  datetime(const date& dt, const time& tm);

  int year() const;
  int month() const;
  int day() const;
  int hour() const;
  int minute() const;
  int second() const;
  DateTime::weekday weekday() const;

  bool is_leapyear() const;
  static bool is_leapyear(int year);

  void add_days(int nb_days);
  void add_hours(int nb_hours);
  void add_minutes(int nb_minutes);
  void add_seconds(int nb_seconds);

  bool operator==(const datetime& other) const;
  bool operator!=(const datetime& other) const;
  bool operator<(const datetime& other) const;
  bool operator<=(const datetime& other) const;
  bool operator>(const datetime& other) const;
  bool operator>=(const datetime& other) const;

  timediff operator-(const datetime& other) const;
  datetime operator-(const timediff& ts) const;
  datetime operator+(const timediff& ts) const;
};

std::string to_string(const datetime&,
                      const char* format = "%Y.%m.%d %H:%M:%S");

}  // namespace DateTime