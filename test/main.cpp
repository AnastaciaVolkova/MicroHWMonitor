#include "sensor_poller.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::invalid_argument;
using std::istringstream;
using std::make_unique;
using std::stof;
using std::string;
using std::unique_ptr;
using std::vector;

// Read data to vector from file
void FillVector(vector<float> &x, ifstream &ifs)
{
  istringstream iss;
  bool to_continue = true;
  while (to_continue)
  {
    string val;
    if (!getline(iss, val, ' '))
    {
      if (!getline(ifs, val))
      {
        to_continue = false;
      }
      else
      {
        to_continue = val != " ";
        iss = istringstream(val);
        getline(iss, val, ' ');
        x.push_back(std::stod(val));
      }
    }
    else
    {
      x.push_back(stof(val));
    }
  }
}

// Compare dut vector and reference vector
bool CompareVectors(const vector<float> &dut, const vector<float> &ref)
{
  float eps = 1e-16;
  if (dut.size() != ref.size())
    return false;
  for (int i = 0; i < dut.size(); i++)
  {
    if (abs(dut[i] - ref[i]) > eps)
      return false;
  }
  return true;
}

bool Test1()
{
  string out_file = "../test/ut_out.txt";
  string out_d_file = "../test/data.txt";
  {
    unique_ptr<Poller> poller;
    try
    {
      poller = make_unique<Poller>("../test/param.xml", out_file, out_d_file);
    }
    catch (invalid_argument exception)
    {
      cerr << exception.what() << endl;
      return -1;
    }
    // Run function to test
    poller->PollSensors();
  }
  ifstream ifs_dut(out_file);
  ifstream ifs_ref(out_d_file);
  vector<float> dut;
  vector<float> ref;
  FillVector(dut, ifs_dut);
  FillVector(ref, ifs_ref);
  bool is_passed = CompareVectors(dut, ref);
  if (is_passed)
    std::cout << "Test is ok" << std::endl;
  else
    std::cout << "Test failed" << std::endl;

  return is_passed;
}

int main()
{
  Test1();
  return 0;
}