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


double backgroundModel(double x) {
  return 0.005 - 0.00001 * x + 0.08 * exp(-0.015 * x);
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
    cout << datasetnames[i] << ": " << datasets[i].measurement(27);
    cout << " +- " << datasets[i].error(27) << endl;

    Data ds1 = datasets[i];
    Data ds2 = datasets[(i+1) % 4];
    double sigma = sqrt(pow(ds1.error(27), 2) + pow(ds2.error(27), 2));

    cout << "Difference to next dataset: ";
    cout << ds2.measurement(27) - ds1.measurement(27) << endl;
    cout << "sigma: " << sigma << ", 2 sigma: " << 2*sigma << endl;
    cout << "Disagreeing values (n=1): " << ds1.checkCompatibility(ds2, 1) << endl;
  }

  cout << "******************************************************" << endl;

  Data combinedDataset = datasets[0];
  for (int i=1; i<datasets.size(); i++) {
    combinedDataset = combinedDataset + datasets[i];
  }

  cout << "Crosssection in bin 27 for combined experiments";
  cout << ": " << combinedDataset.measurement(27);
  cout << " +- " << combinedDataset.error(27) << endl;

  cout << "******************************************************" << endl;

  // Exercise 2

  for (int i=0; i<datasets.size()-1; i++) {
    for (int j=i+1; j<datasets.size(); j++) {
      cout << "Comparing datasets " << datasetnames[i];
      cout << " and " << datasetnames[j] << ":" << endl;

      cout << "  Values differing by 2 sigma: ";
      cout << datasets[i].checkCompatibility(datasets[j], 2) << endl;
      cout << "  Values differing by 3 sigma: ";
      cout << datasets[i].checkCompatibility(datasets[j], 3) << endl;
    }
  }

  cout << "Expected differing values: " << endl;
  cout << "  2 sigma: " << (1 - 0.954) * datasets[0].size() << endl;
  cout << "  3 sigma: " << (1 - 0.997) * datasets[0].size() << endl;

  cout << "******************************************************" << endl;

  cout << "Chi^2 comparison with background" << endl;

  for (int i=0; i<datasets.size(); i++) {
    cout << "  Dataset " << datasetnames[i] << ": ";
    cout << datasets[i].chiSquare(backgroundModel, 4) << endl;
  }

  cout << "  Combined Dataset: " << combinedDataset.chiSquare(backgroundModel, 4) << endl;

  return 0;
}
