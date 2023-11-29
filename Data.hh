#ifndef DATA_HH
#define DATA_HH

#include <vector>
#include <string>

class Data {
 public:
  Data(const std::string& filename);

  unsigned int size() const { return m_data.size(); }
  double measurement(int i) const { return m_data[i]; }
  double binCenter(int i) const { return (binLow(i) + binHigh(i)) / 2.0; }
  double binLow(int i) const { return m_bins[i]; }
  double binHigh(int i) const { return m_bins[i+1]; }
  double error(int i) const { return m_uncertainty[i]; }
  int checkCompatibility(const Data& in, int n);

 private:
  Data() {}  // disallow empty data set
  void assertSizes();
  double combinedDeviation(const Data& in, int i);
  std::vector<double> m_data;
  std::vector<double> m_bins;
  std::vector<double> m_uncertainty;
};

#endif
