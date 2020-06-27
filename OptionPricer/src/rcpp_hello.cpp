#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
List rcpp_hello() {
  CharacterVector x = CharacterVector::create("foo", "bar");
  NumericVector y   = NumericVector::create(0.0, 1.0);
  List z            = List::create(x, y);
  return z;
}

// [[Rcpp::export]]
double squareMe(double x) {
  return x*x;
}