#include <iostream>
#include <vector>
#include <functional>
#include <string>
#include <cmath>

#include "Data.hh"

// generic function comparing two values of some type T used later for int and
// double
template <class T>
bool testEqual(const std::string& name, T expected, T real) {
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;

  vector<Data> datasets;
  vector<string> datasetnames = {"exp_A", "exp_B", "exp_C", "exp_D"};

  for(string dsname : datasetnames)
    datasets.push_back(Data(dsname));

  for(int i=0; i<datasets.size(); i++) {
    cout << "Crosssection in bin 27 for experiment ";
    cout << datasetnames[i] << ": " << datasets[i].measurement(27) << endl;

    Data ds1 = datasets[i];
    Data ds2 = datasets[(i+1) % 4];
    double sigma = sqrt(pow(ds1.error(27), 2) + pow(ds2.error(27), 2));

    cout << "Difference to next dataset: ";
    cout << ds2.measurement(27) - ds1.measurement(27) << endl;
    cout << "sigma: " << sigma << ", 2 sigma: " << 2*sigma << endl;
    cout << "Disagreeing values (n=1): " << ds1.checkCompatibility(ds2, 1) << endl;
  }

  cout << "******************************************************" << endl;
  // create an object which holds data of experiment A
  Data datA("exp_A");

  // here is the data from experiment A
  cout << "bin 27: from " << datA.binLow(27) << " to " << datA.binHigh(27)
       << endl;
  cout << "measurement of experiment A in bin 27: " << datA.measurement(27)
       << endl;

  return 0;
}
