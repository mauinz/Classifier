#include "../Segmentor/segmentor.hpp"

char const* greet()
{
  Segmentor * myseg = new Segmentor;
  delete myseg;
  return "hello, world";
  
}
 
#include <boost/python.hpp>
 
BOOST_PYTHON_MODULE(hello_ext)
{
  using namespace boost::python;
  def("greet", greet);
}
