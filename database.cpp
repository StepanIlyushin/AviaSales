#include "database.hpp"

#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>

std::string to_upper(const std::string& str) {
  std::string res;
  for (size_t i = 0; i < str.length(); ++i) {
    res += static_cast<char>(std::toupper(str[i]));
  }
  return res;
}

bool is_less(const Ticket& a, const Ticket& b) {
  if (a.departure_time != b.departure_time) {
    return a.departure_time < b.departure_time;
  }
  std::string flight_a = to_upper(a.flight_number);
  std::string flight_b = to_upper(b.flight_number);
  if (flight_a != flight_b) {
    return flight_a < flight_b;
  }
  if (a.price != b.price) {
    return a.price < b.price;
  }
  return a.ticket_count < b.ticket_count;
}

void quick_sort_full(const Ticket** arr, int left, int right) {
  if (left >= right) {
    return;
  }
  const Ticket* p = arr[(left + right) / 2];
  int i = left;
  int j = right;

  while (i <= j) {
    while (is_less(*arr[i], *p)) {
      i++;
    }
    while (is_less(*p, *arr[j])) {
      j--;
    }
    if (i <= j) {
      const Ticket* tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
      i++;
      j--;
    }
  }
  if (left < j) {
    quick_sort_full(arr, left, j);
  }
  if (i < right) {
    quick_sort_full(arr, i, right);
  }
}

void quick_sort_departure(const Ticket** arr, int left, int right) {
  if (left >= right) {
    return;
  }

  const Ticket* p = arr[(left + right) / 2];

  int i = left;

  int j = right;

  while (i <= j) {
    while (true) {
      bool less = false;

      if (arr[i]->departure_time != p->departure_time) {
        less = arr[i]->departure_time < p->departure_time;

      } else if (to_upper(arr[i]->flight_number) !=
                 to_upper(p->flight_number)) {
        less = to_upper(arr[i]->flight_number) < to_upper(p->flight_number);

      } else {
        less = arr[i]->id < p->id;
      }

      if (less) {
        i++;

      } else {
        break;
      }
    }

    while (true) {
      bool greater = false;

      if (p->departure_time != arr[j]->departure_time) {
        greater = p->departure_time < arr[j]->departure_time;

      } else if (to_upper(p->flight_number) !=
                 to_upper(arr[j]->flight_number)) {
        greater = to_upper(p->flight_number) < to_upper(arr[j]->flight_number);

      } else {
        greater = p->id < arr[j]->id;
      }

      if (greater) {
        j--;

      } else {
        break;
      }
    }

    if (i <= j) {
      const Ticket* tmp = arr[i];

      arr[i] = arr[j];

      arr[j] = tmp;

      i++;

      j--;
    }
  }

  if (left < j) {
    quick_sort_departure(arr, left, j);
  }

  if (i < right) {
    quick_sort_departure(arr, i, right);
  }
}

void quick_sort_arrival(const Ticket** arr, int left, int right) {
  if (left >= right) {
    return;
  }
  const Ticket* p = arr[(left + right) / 2];
  int i = left;
  int j = right;

  while (i <= j) {
    while (true) {
      bool less = false;
      if (arr[i]->arrival_time != p->arrival_time) {
        less = arr[i]->arrival_time < p->arrival_time;
      } else if (to_upper(arr[i]->flight_number) !=
                 to_upper(p->flight_number)) {
        less = to_upper(arr[i]->flight_number) < to_upper(p->flight_number);
      } else {
        less = arr[i]->id < p->id;
      }
      if (less) {
        i++;
      } else {
        break;
      }
    }
    while (true) {
      bool greater = false;
      if (p->arrival_time != arr[j]->arrival_time) {
        greater = p->arrival_time < arr[j]->arrival_time;
      } else if (to_upper(p->flight_number) !=
                 to_upper(arr[j]->flight_number)) {
        greater = to_upper(p->flight_number) < to_upper(arr[j]->flight_number);
      } else {
        greater = p->id < arr[j]->id;
      }
      if (greater) {
        j--;
      } else {
        break;
      }
    }
    if (i <= j) {
      const Ticket* tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
      i++;
      j--;
    }
  }
  if (left < j) {
    quick_sort_arrival(arr, left, j);
  }
  if (i < right) {
    quick_sort_arrival(arr, i, right);
  }
}

void quick_sort_from_to(const Ticket** arr, int left, int right) {
  if (left >= right) {
    return;
  }
  const Ticket* p = arr[(left + right) / 2];
  int i = left;
  int j = right;

  while (i <= j) {
    while (true) {
      bool less = false;
      if (arr[i]->price != p->price) {
        less = arr[i]->price < p->price;
      } else if (arr[i]->departure_time != p->departure_time) {
        less = arr[i]->departure_time < p->departure_time;
      } else if (to_upper(arr[i]->flight_number) !=
                 to_upper(p->flight_number)) {
        less = arr[i]->flight_number < p->flight_number;
      } else if (arr[i]->ticket_count != p->ticket_count) {
        less = arr[i]->ticket_count < p->ticket_count;
      } else {
        less = arr[i]->id < p->id;
      }
      if (less) {
        i++;
      } else {
        break;
      }
    }
    while (true) {
      bool greater = false;
      if (p->price != arr[j]->price) {
        greater = p->price < arr[j]->price;
      } else if (p->departure_time != arr[j]->departure_time) {
        greater = p->departure_time < arr[j]->departure_time;
      } else if (to_upper(p->flight_number) !=
                 to_upper(arr[j]->flight_number)) {
        greater = p->flight_number < arr[j]->flight_number;
      } else if (p->ticket_count != arr[j]->ticket_count) {
        greater = p->ticket_count < arr[j]->ticket_count;
      } else {
        greater = p->id < arr[j]->id;
      }
      if (greater) {
        j--;
      } else {
        break;
      }
    }
    if (i <= j) {
      const Ticket* tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
      i++;
      j--;
    }
  }
  if (left < j) {
    quick_sort_from_to(arr, left, j);
  }
  if (i < right) {
    quick_sort_from_to(arr, i, right);
  }
}

struct ScheduleEvent {
  const Ticket* ticket;
  bool is_departure;
};

void quick_sort_schedule(ScheduleEvent* arr, int left, int right) {
  if (left >= right) {
    return;
  }
  ScheduleEvent p = arr[(left + right) / 2];
  int i = left;
  int j = right;

  while (i <= j) {
    while (true) {
      DateTime::datetime time_i = arr[i].is_departure
                                      ? arr[i].ticket->departure_time
                                      : arr[i].ticket->arrival_time;
      DateTime::datetime time_p =
          p.is_departure ? p.ticket->departure_time : p.ticket->arrival_time;
      bool less = false;
      if (time_i != time_p) {
        less = time_i < time_p;
      } else if (to_upper(arr[i].ticket->flight_number) !=
                 to_upper(p.ticket->flight_number)) {
        less = to_upper(arr[i].ticket->flight_number) <
               to_upper(p.ticket->flight_number);
      } else {
        less = arr[i].ticket->id < p.ticket->id;
      }
      if (less) {
        i++;
      } else {
        break;
      }
    }
    while (true) {
      DateTime::datetime time_p =
          p.is_departure ? p.ticket->departure_time : p.ticket->arrival_time;
      DateTime::datetime time_j = arr[j].is_departure
                                      ? arr[j].ticket->departure_time
                                      : arr[j].ticket->arrival_time;
      bool greater = false;
      if (time_p != time_j) {
        greater = time_p < time_j;
      } else if (to_upper(p.ticket->flight_number) !=
                 to_upper(arr[j].ticket->flight_number)) {
        greater = to_upper(p.ticket->flight_number) <
                  to_upper(arr[j].ticket->flight_number);
      } else {
        greater = p.ticket->id < arr[j].ticket->id;
      }
      if (greater) {
        j--;
      } else {
        break;
      }
    }
    if (i <= j) {
      ScheduleEvent temp = arr[i];
      arr[i] = arr[j];
      arr[j] = temp;
      i++;
      j--;
    }
  }
  if (left < j) {
    quick_sort_schedule(arr, left, j);
  }
  if (i < right) {
    quick_sort_schedule(arr, i, right);
  }
}

Database::Database() {
  size_ = 0;
  capacity_ = 2;
  next_id_ = 1;

  records_ = new Ticket[capacity_];
}

Database::~Database() { delete[] records_; }

void Database::resize(int new_capacity) {
  Ticket* new_records = new Ticket[new_capacity];

  for (int i = 0; i < size_; ++i) {
    new_records[i] = records_[i];
  }

  delete[] records_;

  records_ = new_records;
  capacity_ = new_capacity;
}

void Database::add(const std::string& flight, const std::string& dep,
                   const std::string& arr, const DateTime::datetime& dt_dep,
                   const DateTime::datetime& dt_arr, int count, double price) {
  if (size_ == capacity_) {
    resize(capacity_ * 2);
  }
  records_[size_].id = next_id_;
  records_[size_].flight_number = flight;
  records_[size_].departure_time = dt_dep;
  records_[size_].arrival_time = dt_arr;
  records_[size_].ticket_count = count;
  records_[size_].price = price;

  records_[size_].departure_airport = "";
  for (size_t i = 0; i < dep.length(); i++) {
    records_[size_].departure_airport +=
        static_cast<char>(std::toupper(dep[i]));
  }

  records_[size_].arrival_airport = "";
  for (size_t i = 0; i < arr.length(); i++) {
    records_[size_].arrival_airport += static_cast<char>(std::toupper(arr[i]));
  }

  size_++;
  next_id_++;
}

int Database::get_total_records() const { return size_; }

int Database::get_total_tickets() const {
  int total = 0;
  for (int i = 0; i < size_; ++i) {
    total += records_[i].ticket_count;
  }
  return total;
}

void Database::print() const {
  const Ticket** sorted = new const Ticket*[size_];
  for (int i = 0; i < size_; ++i) {
    sorted[i] = &records_[i];
  }

  quick_sort_full(sorted, 0, size_ - 1);

  for (int i = 0; i < size_; ++i) {
    const Ticket& t = *sorted[i];

    std::cout << t.id << ", " << t.flight_number << ", " << t.departure_airport
              << ", " << t.arrival_airport << ", "
              << DateTime::to_string(t.departure_time, "%Y.%m.%d %H:%M") << ", "
              << DateTime::to_string(t.arrival_time, "%Y.%m.%d %H:%M") << ", "
              << t.ticket_count << ", " << t.price << "\n";
  }

  delete[] sorted;
}

void Database::delete_id(int id) {
  for (int i = 0; i < size_; ++i) {
    if (records_[i].id == id) {
      for (int j = i; j < size_ - 1; ++j) {
        records_[j] = records_[j + 1];
      }
      size_--;
      return;
    }
  }
}

void Database::clear() {
  int new_size = 0;
  for (int i = 0; i < size_; ++i) {
    if (records_[i].ticket_count > 0) {
      records_[new_size] = records_[i];
      new_size++;
    }
  }
  size_ = new_size;
}

void Database::print_departure(const std::string& airport, int year, int month,
                               int day) const {
  std::string search_airport;
  for (size_t i = 0; i < airport.length(); i++) {
    search_airport += static_cast<char>(std::toupper(airport[i]));
  }

  const Ticket** filtered = new const Ticket*[size_];
  int count = 0;
  for (int i = 0; i < size_; ++i) {
    if (records_[i].departure_airport == search_airport &&
        records_[i].departure_time.year() == year &&
        records_[i].departure_time.month() == month &&
        records_[i].departure_time.day() == day) {
      filtered[count] = &records_[i];
      count++;
    }
  }

  quick_sort_departure(filtered, 0, count - 1);

  std::string last_flight;
  DateTime::datetime last_time(2000, 1, 1);

  for (int i = 0; i < count; ++i) {
    std::string current_flight = to_upper(filtered[i]->flight_number);
    if (current_flight == last_flight &&
        filtered[i]->departure_time == last_time) {
      continue;
    }
    last_flight = current_flight;
    last_time = filtered[i]->departure_time;

    std::cout << filtered[i]->flight_number << ", "
              << filtered[i]->arrival_airport << ", "
              << DateTime::to_string(filtered[i]->departure_time, "%H:%M")
              << "\n";
  }

  delete[] filtered;
}

void Database::print_arrival(const std::string& airport, int year, int month,
                             int day) const {
  std::string search_airport;
  for (size_t i = 0; i < airport.length(); i++) {
    search_airport += static_cast<char>(std::toupper(airport[i]));
  }

  const Ticket** filtered = new const Ticket*[size_];
  int count = 0;
  for (int i = 0; i < size_; ++i) {
    if (records_[i].arrival_airport == search_airport &&
        records_[i].arrival_time.year() == year &&
        records_[i].arrival_time.month() == month &&
        records_[i].arrival_time.day() == day) {
      filtered[count] = &records_[i];
      count++;
    }
  }

  quick_sort_arrival(filtered, 0, count - 1);

  std::string last_flight;
  DateTime::datetime last_time(2000, 1, 1);

  for (int i = 0; i < count; ++i) {
    std::string current_flight = to_upper(filtered[i]->flight_number);
    if (current_flight == last_flight &&
        filtered[i]->arrival_time == last_time) {
      continue;
    }
    last_flight = current_flight;
    last_time = filtered[i]->arrival_time;

    std::cout << filtered[i]->flight_number << ", "
              << filtered[i]->departure_airport << ", "
              << DateTime::to_string(filtered[i]->arrival_time, "%H:%M")
              << "\n";
  }

  delete[] filtered;
}

void Database::print_schedule(const std::string& airport, int year, int month,
                              int day) const {
  std::string search_airport;
  for (size_t i = 0; i < airport.length(); i++) {
    search_airport += static_cast<char>(std::toupper(airport[i]));
  }
  ScheduleEvent* events = new ScheduleEvent[static_cast<size_t>(size_) * 2];
  int count = 0;

  for (int i = 0; i < size_; ++i) {
    if (records_[i].departure_airport == search_airport &&
        records_[i].departure_time.year() == year &&
        records_[i].departure_time.month() == month &&
        records_[i].departure_time.day() == day) {
      events[count].ticket = &records_[i];
      events[count].is_departure = true;
      count++;
    }
    if (records_[i].arrival_airport == search_airport &&
        records_[i].arrival_time.year() == year &&
        records_[i].arrival_time.month() == month &&
        records_[i].arrival_time.day() == day) {
      events[count].ticket = &records_[i];
      events[count].is_departure = false;
      count++;
    }
  }

  quick_sort_schedule(events, 0, count - 1);

  std::string last_flight;
  DateTime::datetime last_time(2000, 1, 1);
  bool last_is_dep = false;

  for (int i = 0; i < count; ++i) {
    DateTime::datetime dt = events[i].is_departure
                                ? events[i].ticket->departure_time
                                : events[i].ticket->arrival_time;

    std::string current_flight = to_upper(events[i].ticket->flight_number);

    if (current_flight == last_flight && dt == last_time &&
        events[i].is_departure == last_is_dep) {
      continue;
    }
    last_flight = current_flight;
    last_time = dt;
    last_is_dep = events[i].is_departure;

    std::string other_airport = events[i].is_departure
                                    ? events[i].ticket->arrival_airport
                                    : events[i].ticket->departure_airport;
    std::string type_str = events[i].is_departure ? "departure" : "arrival";

    std::cout << events[i].ticket->flight_number << ", " << other_airport
              << ", " << DateTime::to_string(dt, "%H:%M") << ", " << type_str
              << "\n";
  }

  delete[] events;
}

void Database::buy_ticket(const std::string& flight_number, int year, int month,
                          int day) {
  std::string search_flight;
  for (size_t i = 0; i < flight_number.length(); i++) {
    search_flight += static_cast<char>(std::toupper(flight_number[i]));
  }

  Ticket* best_ticket = nullptr;

  for (int i = 0; i < size_; ++i) {
    if (to_upper(records_[i].flight_number) == search_flight &&
        records_[i].departure_time.year() == year &&
        records_[i].departure_time.month() == month &&
        records_[i].departure_time.day() == day &&
        records_[i].ticket_count > 0) {
      if (best_ticket == nullptr || records_[i].price < best_ticket->price) {
        best_ticket = &records_[i];
      }
    }
  }
  if (best_ticket == nullptr) {
    std::cout << "no tickets\n";
  } else {
    best_ticket->ticket_count--;
    std::cout << best_ticket->price << "\n";
  }
}

void Database::print_from_to(const std::string& dep, const std::string& arr,
                             int year, int month, int day) const {
  std::string search_dep;
  for (size_t i = 0; i < dep.length(); i++) {
    search_dep += static_cast<char>(std::toupper(dep[i]));
  }

  std::string search_arr;
  for (size_t i = 0; i < arr.length(); i++) {
    search_arr += static_cast<char>(std::toupper(arr[i]));
  }

  const Ticket** filtered = new const Ticket*[size_];
  int count = 0;

  for (int i = 0; i < size_; ++i) {
    if (records_[i].departure_airport == search_dep &&
        records_[i].arrival_airport == search_arr &&
        records_[i].departure_time.year() == year &&
        records_[i].departure_time.month() == month &&
        records_[i].departure_time.day() == day &&
        records_[i].ticket_count > 0) {
      filtered[count] = &records_[i];
      count++;
    }
  }

  if (count == 0) {
    std::cout << "no tickets\n";
    delete[] filtered;
    return;
  }

  quick_sort_from_to(filtered, 0, count - 1);

  for (int i = 0; i < count; ++i) {
    std::cout << filtered[i]->flight_number << ", "
              << filtered[i]->departure_airport << ", "
              << filtered[i]->arrival_airport << ", "
              << DateTime::to_string(filtered[i]->departure_time,
                                     "%Y.%m.%d %H:%M")
              << ", "
              << DateTime::to_string(filtered[i]->arrival_time,
                                     "%Y.%m.%d %H:%M")
              << ", " << filtered[i]->ticket_count << ", " << filtered[i]->price
              << "\n";
  }
  delete[] filtered;
}

void Database::print_find(const std::string& dep,
                          const std::string& arr) const {
  std::string search_dep;
  for (size_t i = 0; i < dep.length(); i++) {
    search_dep += static_cast<char>(std::toupper(dep[i]));
  }

  std::string search_arr;
  for (size_t i = 0; i < arr.length(); i++) {
    search_arr += static_cast<char>(std::toupper(arr[i]));
  }

  const Ticket* best_direct = nullptr;
  const Ticket* best_t1 = nullptr;
  const Ticket* best_t2 = nullptr;
  double min_price = -1.0;

  for (int i = 0; i < size_; ++i) {
    if (records_[i].departure_airport == search_dep &&
        records_[i].arrival_airport == search_arr &&
        records_[i].ticket_count > 0) {
      if (min_price < 0 || records_[i].price < min_price) {
        min_price = records_[i].price;
        best_direct = &records_[i];
      } else if (records_[i].price == min_price) {
        if (is_less(records_[i], *best_direct)) {
          best_direct = &records_[i];
        }
      }
    }
  }

  const Ticket** dep_flights = new const Ticket*[size_];
  int dep_count = 0;
  for (int i = 0; i < size_; ++i) {
    if (records_[i].departure_airport == search_dep &&
        records_[i].ticket_count > 0) {
      dep_flights[dep_count++] = &records_[i];
    }
  }

  const Ticket** arr_flights = new const Ticket*[size_];
  int arr_count = 0;
  for (int i = 0; i < size_; ++i) {
    if (records_[i].arrival_airport == search_arr &&
        records_[i].ticket_count > 0) {
      arr_flights[arr_count++] = &records_[i];
    }
  }

  for (int i = 0; i < dep_count; ++i) {
    for (int j = 0; j < arr_count; ++j) {
      if (dep_flights[i]->arrival_airport ==
          arr_flights[j]->departure_airport) {
        if (arr_flights[j]->departure_time > dep_flights[i]->arrival_time) {
          DateTime::timediff diff =
              arr_flights[j]->departure_time - dep_flights[i]->arrival_time;

          if (diff.total_seconds() <= 86400) {
            double total_price = dep_flights[i]->price + arr_flights[j]->price;

            if (min_price < 0 || total_price < min_price) {
              min_price = total_price;
              best_t1 = dep_flights[i];
              best_t2 = arr_flights[j];
              best_direct = nullptr;
            } else if (total_price == min_price && best_t1 != nullptr) {
              if (is_less(*dep_flights[i], *best_t1)) {
                best_t1 = dep_flights[i];
                best_t2 = arr_flights[j];
                best_direct = nullptr;
              }
            }
          }
        }
      }
    }
  }

  delete[] dep_flights;
  delete[] arr_flights;

  if (min_price < 0) {
    std::cout << "no tickets\n";
  } else if (best_direct != nullptr) {
    std::cout << best_direct->flight_number << ", "
              << best_direct->departure_airport << ", "
              << best_direct->arrival_airport << ", "
              << DateTime::to_string(best_direct->departure_time,
                                     "%Y.%m.%d %H:%M")
              << ", "
              << DateTime::to_string(best_direct->arrival_time,
                                     "%Y.%m.%d %H:%M")
              << ", " << best_direct->ticket_count << ", " << best_direct->price
              << "\n";
  } else if (best_t1 != nullptr && best_t2 != nullptr) {
    std::cout << best_t1->flight_number << ", " << best_t1->departure_airport
              << ", " << best_t1->arrival_airport << ", "
              << DateTime::to_string(best_t1->departure_time, "%Y.%m.%d %H:%M")
              << ", "
              << DateTime::to_string(best_t1->arrival_time, "%Y.%m.%d %H:%M")
              << ", " << best_t1->ticket_count << ", " << best_t1->price
              << "\n";

    std::cout << best_t2->flight_number << ", " << best_t2->departure_airport
              << ", " << best_t2->arrival_airport << ", "
              << DateTime::to_string(best_t2->departure_time, "%Y.%m.%d %H:%M")
              << ", "
              << DateTime::to_string(best_t2->arrival_time, "%Y.%m.%d %H:%M")
              << ", " << best_t2->ticket_count << ", " << best_t2->price
              << "\n";
  }
}

void Database::export_to_file(const std::string& filename) const {
  std::ofstream out(filename);
  if (!out.is_open()) {
    std::cout << "Can not write to file\n";
    return;
  }

  const Ticket** sorted = new const Ticket*[size_];
  for (int i = 0; i < size_; ++i) {
    sorted[i] = &records_[i];
  }

  quick_sort_full(sorted, 0, size_ - 1);

  for (int i = 0; i < size_; ++i) {
    const Ticket& t = *sorted[i];

    out << t.flight_number << ", " << t.departure_airport << ", "
        << t.arrival_airport << ", "
        << DateTime::to_string(t.departure_time, "%Y.%m.%d %H:%M") << ", "
        << DateTime::to_string(t.arrival_time, "%Y.%m.%d %H:%M") << ", "
        << t.ticket_count << ", " << t.price << "\n";
  }

  delete[] sorted;
  out.close();
}

void Database::save(const std::string& filename) const {
  std::ofstream out(filename);
  if (!out.is_open()) {
    std::cout << "Can not write to file\n";
    return;
  }

  for (int i = 0; i < size_; ++i) {
    out << records_[i].flight_number << ", " << records_[i].departure_airport
        << ", " << records_[i].arrival_airport << ", "
        << DateTime::to_string(records_[i].departure_time, "%d.%m.%Y %H:%M")
        << ", "
        << DateTime::to_string(records_[i].arrival_time, "%d.%m.%Y %H:%M")
        << ", " << records_[i].ticket_count << ", " << records_[i].price
        << "\n";
  }

  out.close();
}

void Database::reset() {
  size_ = 0;
  next_id_ = 1;
}

void Database::bothways(const std::string& dep, const std::string& arr) const {
  std::string search_dep;
  for (size_t i = 0; i < dep.length(); i++) {
    search_dep += static_cast<char>(std::toupper(dep[i]));
  }

  std::string search_arr;
  for (size_t i = 0; i < arr.length(); i++) {
    search_arr += static_cast<char>(std::toupper(arr[i]));
  }
  const Ticket** out_flights = new const Ticket*[size_];
  int out_count = 0;

  const Ticket** back_flights = new const Ticket*[size_];
  int back_count = 0;

  for (int i = 0; i < size_; ++i) {
    if (records_[i].ticket_count > 0) {
      if (records_[i].departure_airport == search_dep &&
          records_[i].arrival_airport == search_arr) {
        out_flights[out_count] = &records_[i];
        out_count++;
      } else if (records_[i].departure_airport == search_arr &&
                 records_[i].arrival_airport == search_dep) {
        back_flights[back_count] = &records_[i];
        back_count++;
      }
    }
  }
  if (out_count == 0 || back_count == 0) {
    std::cout << "no tickets\n";
    delete[] out_flights;
    delete[] back_flights;
    return;
  }

  const Ticket* best_out = nullptr;
  const Ticket* best_back = nullptr;
  double min_price = -1.0;

  for (int i = 0; i < out_count; i++) {
    for (int j = 0; j < back_count; j++) {
      if (out_flights[i]->arrival_time < back_flights[j]->departure_time) {
        double cur_price = out_flights[i]->price + back_flights[j]->price;
        if (min_price < 0 || cur_price < min_price) {
          min_price = cur_price;
          best_out = out_flights[i];
          best_back = back_flights[j];
        } else if (cur_price == min_price) {
          if (is_less(*out_flights[i], *best_out)) {
            best_out = out_flights[i];
            best_back = back_flights[j];
          } else if (!is_less(*best_out, *out_flights[i])) {
            if (is_less(*back_flights[j], *best_back)) {
              best_out = out_flights[i];
              best_back = back_flights[j];
            }
          }
        }
      }
    }
  }

  if (min_price < 0) {
    std::cout << "no tickets\n";
  } else {
    std::cout << best_out->flight_number << ", " << best_out->departure_airport
              << ", " << best_out->arrival_airport << ", "
              << DateTime::to_string(best_out->departure_time, "%Y.%m.%d %H:%M")
              << ", "
              << DateTime::to_string(best_out->arrival_time, "%Y.%m.%d %H:%M")
              << ", " << best_out->price << "\n";

    std::cout << best_back->flight_number << ", "
              << best_back->departure_airport << ", "
              << best_back->arrival_airport << ", "
              << DateTime::to_string(best_back->departure_time,
                                     "%Y.%m.%d %H:%M")
              << ", "
              << DateTime::to_string(best_back->arrival_time, "%Y.%m.%d %H:%M")
              << ", " << best_back->price << "\n";
  }
  delete[] out_flights;
  delete[] back_flights;
}