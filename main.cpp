#include "Query.hpp"
#include <iostream>
#include <fstream>

int main()
{
  std::ifstream file("input_file.html");
  Query::TextQuery input_query(file);
  auto q = Query::Query("</p>") | Query::Query("</body>");
  auto or_query = q.evaluate(input_query);
  display(std::cout, or_query);
  return 0;
}
