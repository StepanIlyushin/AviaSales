#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>

#include "datetime.hpp"

struct Ticket {
  int id;
  std::string flight_number;
  std::string departure_airport;
  std::string arrival_airport;
  DateTime::datetime departure_time;
  DateTime::datetime arrival_time;
  int ticket_count;
  double price;

  Ticket()
      : id(0),
        flight_number(""),
        departure_airport(""),
        arrival_airport(""),
        departure_time(2000, 1, 1, 0, 0, 0),
        arrival_time(2000, 1, 1, 0, 0, 0),
        ticket_count(0),
        price(0.0) {}
};

class Database {
 private:
  Ticket* records_;
  int size_;
  int capacity_;
  int next_id_;

  void resize(int new_capacity);

 public:
  Database();
  ~Database();

  void add(const std::string& flight, const std::string& dep,
           const std::string& arr, const DateTime::datetime& dt_dep,
           const DateTime::datetime& dt_arr, int count, double price);

  void print() const;

  int get_total_records() const;
  int get_total_tickets() const;

  void delete_id(int id);
  void clear();
  void reset();

  void print_departure(const std::string& airport, int year, int month,
                       int day) const;
  void print_arrival(const std::string& airport, int year, int month,
                     int day) const;
  void print_schedule(const std::string& airport, int year, int month,
                      int day) const;

  void buy_ticket(const std::string& flight_number, int year, int month,
                  int day);

  void print_from_to(const std::string& dep, const std::string& arr, int year,
                     int month, int day) const;

  void print_find(const std::string& dep, const std::string& arr) const;

  void export_to_file(const std::string& filename) const;
  void save(const std::string& filename) const;
  void bothways(const std::string& dep, const std::string& arr) const;
};

#endif