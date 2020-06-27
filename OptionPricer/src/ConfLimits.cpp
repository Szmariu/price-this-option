// ConfLimits.cpp

#include "MCStatistics.h"
#include "ConfLimits.h"
#include "VecMeanStdDev.h"
#include  <cmath>

using namespace std;

ConfidenceLimits::ConfidenceLimits()
    :
    Payoffs(0.0)
{
}

void ConfidenceLimits::DumpOneResult(double result)
{
    Payoffs.push_back(result);
}

vector<vector<double> > ConfidenceLimits::GetResultsSoFar() const
{
    vector<vector<double> > Results(3);

	//increase size
    Results[0].resize(1);
    Results[0][0] = mean(Payoffs);
	Results[1].resize(1);
	Results[1][0] = stdDev(Payoffs) / pow(Payoffs.size(),0.5);
	Results[2].resize(2);
	Results[2][0] = Results[0][0] - 1.96 * Results[1][0];
	Results[2][1] = Results[0][0] + 1.96 * Results[1][0];
    return Results;
}

StatisticsMC* ConfidenceLimits::clone() const
{
    return new ConfidenceLimits(*this);
}


void ConfidenceLimits::reset(){
    Payoffs.clear();
}
