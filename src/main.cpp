#include "sensor_poller.hpp"
#include <stdexcept>

using std::invalid_argument;
using std::make_unique;
using std::string;
using std::unique_ptr;

bool ParseCommandLine(int argc, char *argv[], string &in_d_file, string &in_file, string &out_file)
{
  int i = 1;
  in_file = "";
  out_file = "";
  in_d_file = "";
  string command_line = argv[0] + string(" -if <input_parameter_file> -of <output_data_file> [-id <input_data_file>]");
  while (i < argc - 1)
  {
    string arg_it = argv[i++];
    if (arg_it == "-if")
    {
      in_file = argv[i++];
    }
    else if (arg_it == "-of")
    {
      out_file = argv[i++];
    }
    else if (arg_it == "-id")
    {
      in_d_file = argv[i++];
    }
    else if (arg_it == "-h")
    {
      std::cout << command_line << std::endl;
      return false;
    }
    else
    {
      std::cerr << "Wrong command line" << std::endl;
      std::cerr << command_line << std::endl;
      return false;
    }
  }
  if ((out_file == "") || (in_file == ""))
  {
    std::cerr << "Wrong command line" << std::endl;
    std::cerr << command_line << std::endl;
    return false;
  }
  return true;
}

int main(int argc, char *argv[])
{
  // Parametes to get from command line
  string in_file, out_file, in_d_file;

  float tp; // Time period to poll sensors

  if (!ParseCommandLine(argc, argv, in_d_file, in_file, out_file))
    return -1;

  unique_ptr<Poller> poller;
  try
  {
    poller = make_unique<Poller>(in_file, out_file, in_d_file);
  }
  catch (invalid_argument exc)
  {
    std::cerr << exc.what() << std::endl;
    return -1;
  }

  poller->PollSensors();
  return 0;
}