#include <gtest/gtest.h>
#include "datetime.hpp"
#include "database.hpp"

//проверка на leapyear
TEST(DateTimeTest, IsLeapYearValidation) {
    EXPECT_TRUE(DateTime::date::is_leapyear(2000));
    EXPECT_FALSE(DateTime::date::is_leapyear(2021));
}

//проверка добавления времени
TEST(DateTimeTest, TimeAddition) {
  DateTime::time t(10, 30, 0);
  t.add_minutes(45);
  
  EXPECT_EQ(t.hour(), 11);
  EXPECT_EQ(t.minute(), 15);
  EXPECT_EQ(t.second(), 0);
}

//проверка добавления билетов
TEST(DatabaseTest, AddTicketAndCountRecords) {
  Database db;
  
  DateTime::datetime dt_dep(2026, 7, 31, 12, 0, 0);
  DateTime::datetime dt_arr(2026, 7, 31, 14, 0, 0);

  EXPECT_EQ(db.get_total_records(), 0);
  EXPECT_EQ(db.get_total_tickets(), 0);

  db.add("SU100", "SVO", "LED", dt_dep, dt_arr, 10, 5000.0);
  
  EXPECT_EQ(db.get_total_records(), 1);
  EXPECT_EQ(db.get_total_tickets(), 10);
}

//Проверка успешной покупки билетов
TEST(DatabaseTest, BuyTicketDecreasesCount) {
  Database db;
  DateTime::datetime dt_dep(2026, 7, 31, 12, 0, 0);
  DateTime::datetime dt_arr(2026, 7, 31, 14, 0, 0);

  db.add("SU100", "SVO", "LED", dt_dep, dt_arr, 5, 5000.0);
  
  EXPECT_EQ(db.get_total_tickets(), 5);

  db.buy_ticket("SU100", 2026, 7, 31);

  EXPECT_EQ(db.get_total_tickets(), 4);
}