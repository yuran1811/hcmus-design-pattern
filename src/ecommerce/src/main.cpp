#include "core/Application.hpp"

const vector<Item> ITEMS = {
    {"Laptop", {999'99, 2}},  {"Mouse", {19'99, 2}},
    {"Keyboard", {29'99, 2}}, {"Tablet", {499'99, 2}},
    {"Phone", {599'99, 2}},   {"Headphones", {199'99, 2}}};

// Coupons will be expired in 10 minutes from the time of execution
const time_t MOCK_TIME = time(nullptr) + 10 * 60;

const vector<Coupon> COUPONS = {{"SALE_10", 10, false, MOCK_TIME, 10},
                                {"SALE_20", 20, false, MOCK_TIME, 10},
                                {"SALE_30", 30, false, MOCK_TIME, 10}};

int main() {
  CouponSystem::getInstance()->importCoupons(COUPONS);

  Application* app = new Application();
  app->setItems(ITEMS);
  app->setCoupons(COUPONS);
  app->run();

  delete app;
  return 0;
}
