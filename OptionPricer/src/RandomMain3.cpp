#include <Rcpp.h>
#include "SimpleMC8.h"
#include "ParkMiller.h"
#include "Vanilla3.h"
#include "MCStatistics.h"
#include "ConvergenceTable.h"
#include "ConfLimits.h"
#include "AntiThetic.h"
#include "VecMeanStdDev.h"
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace Rcpp;
using namespace std;

// [[Rcpp::export]]
double MCEuropeanOptionPricer(double Expiry,
                              double Strike,
                              double Spot,
                              double Vol,
                              double r,
                              unsigned long int NumberOfPaths,
                              unsigned long int baseSeed){

  srand(time(NULL));
  if (baseSeed == 0) baseSeed = rand();

	PayOffCall thePayOff(Strike);
  VanillaOption theOption(thePayOff, Expiry);

  ParametersConstant VolParam(Vol);
  ParametersConstant rParam(r);

  StatisticsMean gatherer1;
  RandomParkMiller generator1(1, baseSeed);

  SimpleMonteCarlo6(theOption,
                    Spot,
                    VolParam,
                    rParam,
                    NumberOfPaths,
                    gatherer1,
                    generator1);

  return gatherer1.GetResultsSoFar()[0][0];
}
