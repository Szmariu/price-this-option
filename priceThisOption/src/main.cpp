// Rcpp magic
#include <Rcpp.h>
using namespace Rcpp;

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
using namespace std;

random_device rd{};
mt19937 gen{rd()};




class asianOption {
  public:
    // Constructor
    asianOption(
      double price,
      double vol,
      double r,
      int t
    ) {
      price_ = price;
      r_ = r;
      vol_ = vol;

      // Conver t into years and days
      t_years_ = 252 / t;
      t_days_ = t - (t_years_ * 252);
    }



    // Run a very simple Monte Carlo simulation
    double runSimulation(int nreps) {
      // This will hold the sum of all values
      // Double type will be long enough for this instrument
      double rollingSum = 0.0;

      // Loop hovever many times we need
      for (int i = 0; i < nreps; i++) {
        rollingSum += getOneSimulation();
      }

      // Return the average value
      return rollingSum / nreps;
    }

  private:
    // Declare the attributes of the class
    double price_, vol_, r_;
    int t_years_; // full years
    int t_days_; // remainder in days



    // Return one price after t
    // This allows for more than a year
    // I guess the calculation of the price with volatility may not exactly be correct
    // But this is Cpp, not finance
    double getOnePrice() {
      // Define the distribution
      normal_distribution<> d{0, vol_};

      // Get the price at the start
      double thisPrice = price_;

      // For each full year
      for(int i = 0; i < t_years_; i++) {
        thisPrice = max(0, thisPrice * d(gen));
      }

      // For the remaining days
      thisPrice = max(0, thisPrice * ( d(gen) * (t_days_ / 252) ) );
      return thisPrice;
    }



    // Get one path, discounted etc.
    double getOneSimulation(){
      double instrumentPrice = getOnePrice();
      double optionValue;


    }
};


double getGeometricAsianPrice(
    double price,
    double vol,
    double r,
    int t
) {

  asianOption thisOption(price, vol, r, t);

  return thisOption.runSimulation(1000);
}




