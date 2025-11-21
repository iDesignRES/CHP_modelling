#include <cassert>

#include "../src/Cost.cpp"
#include "test_utils.h"

int main() {
  assert(approxEqual(cecpi(1994), 268.1));
  assert(approxEqual(cecpi(1995), 381.1));
  assert(approxEqual(cecpi(1996), 381.7));
  assert(approxEqual(cecpi(1997), 386.5));
  assert(approxEqual(cecpi(1998), 389.5));
  assert(approxEqual(cecpi(1999), 390.6));
  assert(approxEqual(cecpi(2000), 394.1));
  assert(approxEqual(cecpi(2001), 394.3));
  assert(approxEqual(cecpi(2002), 395.6));
  assert(approxEqual(cecpi(2003), 402.0));
  assert(approxEqual(cecpi(2004), 444.2));
  assert(approxEqual(cecpi(2005), 468.2));
  assert(approxEqual(cecpi(2006), 499.6));
  assert(approxEqual(cecpi(2007), 525.4));
  assert(approxEqual(cecpi(2008), 550.8));
  assert(approxEqual(cecpi(2009), 521.9));
  assert(approxEqual(cecpi(2010), 575.4));
  assert(approxEqual(cecpi(2011), 525.4));
  assert(approxEqual(cecpi(2012), 584.6));
  assert(approxEqual(cecpi(2013), 587.3));
  assert(approxEqual(cecpi(2014), 586.77));
  assert(approxEqual(cecpi(2015), 592.0));
  assert(approxEqual(cecpi(2016), 606.0));
  assert(approxEqual(cecpi(2017), 623.5));
  assert(approxEqual(cecpi(2018), 638.1));
  assert(approxEqual(cecpi(2019), 652.9));
  assert(approxEqual(cecpi(2020), 668.0));

  return 0;
}
