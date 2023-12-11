#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <cmath>

using namespace std;

Data::Data(const std::string& filename) {
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw std::runtime_error("Error could not open file with name: " +
                             filename);
  }

  int size = -1;
  file >> size;

  // read in data from file: size+1 bin edges
  for (int i = 0; i < size + 1; ++i) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: size bin contents
  for (int i = 0; i < size; ++i) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }

  // read in uncertainties from file: size bin contents
  for (int i = 0; i < size; ++i) {
    double uncertainty;
    file >> uncertainty;
    m_uncertainty.push_back(uncertainty);
  }

  // done! close the file
  file.close();

  assertSizes();
};

Data::Data(const Data& obj) {
  m_bins = obj.m_bins;
  m_data = obj.m_data;
  m_uncertainty = obj.m_uncertainty;
}


int Data::checkCompatibility(const Data& in, int n) {
  int disagreeingValues = 0;
  for(int i=0; i < m_data.size(); i++) {
    double valueDifference = abs(measurement(i) - in.measurement(i));
    if(valueDifference > n * combinedDeviation(in, i))
      disagreeingValues++;
  }
  return disagreeingValues;
}

Data Data::operator+(const Data& other) {
  Data res = other;
  for (int i=0; i<size(); i++) {
    double omega = 1/pow(error(i), 2);
    double omega_oth = 1/pow(other.error(i), 2);

    res.m_data[i] = (omega * measurement(i) + omega_oth * other.measurement(i)) / (omega + omega_oth);
    res.m_uncertainty[i] = sqrt(1/(omega + omega_oth));
  }
  return res;
}

double Data::combinedDeviation(const Data& in, int i) {
  return sqrt(pow(error(i), 2) + pow(in.error(i), 2));
}


void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }
