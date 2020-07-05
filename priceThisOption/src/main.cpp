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

// For the random number generation
random_device rd{};
mt19937 gen{rd()};

// Geometric mean
// Taken from here: https://stackoverflow.com/questions/19980319/efficient-way-to-compute-geometric-mean-of-many-numbers
// Should mitigate the problem of overflowing
// Could use the more complex bucket one version,
// but should be fine for options of 10+ years so no need
double geometric_mean(vector<double> const & data){
  double m = 1.0;
  long long ex = 0;
  double invN = 1.0 / data.size();

  for (double x : data)
  {
    int i;
    double f1 = frexp(x, &i);
    m *= f1;
    ex += i;
  }

  return pow( numeric_limits<double>::radix, ex * invN) * pow(m, invN);
}

// Define the main class
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
      t_ = t;

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
    double price_, vol_, r_, t_;
    int t_years_; // full years
    int t_days_; // remainder in days



    // Return one price after t
    // This allows for more than a year
    // I guess the calculation of the price with volatility may not exactly be correct
    // But this is Cpp, not finance
    double getOnePrice() {

      // Define the distribution
      double dailyVolatility = vol_ / sqrt(252);
      normal_distribution<> d{0, dailyVolatility};

      // Current price
      double thisPrice = price_;

      // Holds all the prices
      vector<double> pricePath;
      pricePath.push_back(thisPrice);

      for(int i = 1; i < t_; i++) {
        // Move the price slightly (assumes that price cannot be negative)
        thisPrice = max(0, thisPrice * d(gen));
        // Add the new price to the vector
        pricePath.push_back(thisPrice);
      }

      // Return geometric mean
      // Rolling sum would not work beacause of overflow
      return geometric_mean(pricePath);
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




