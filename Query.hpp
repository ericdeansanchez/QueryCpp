/* ===------------------------------- Query --------------------------------===>/
  Eric Sanchez @ericdeansanchez
  Solution(s)/Extension(s) to problems in cpp_primer
          
  Exemplar: 
  
  int main()
  {
      std::ifstream file("input_file.html");
      Query::TextQuery input_query(file);
      auto q = Query::Query("</p>") | Query::Query("</body>");
      auto or_query = q.evaluate(input_query);
      display(std::cout, or_query);
      return 0;
  }

  Output: 

  (</p> | </body>) occurs 5 times
          (line 41)         </p>
          (line 50)         </p>
          (line 57)         </p>
          (line 59)       </p>
          (line 68) </body>
          
/<===----------------------------------------------------------------------=== */
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <vector>

namespace Query {

// forward declared to be used in TextQuery
class QueryResult;

class TextQuery {
public:
    using line_number = std::vector<std::string>::size_type;
    explicit TextQuery(std::ifstream&);
    QueryResult query(const std::string&) const;

private:
    // to be vectorized representation of input file
    std::shared_ptr<std::vector<std::string>> file;
    // map string to set of line_number(s)
    std::map<std::string, std::shared_ptr<std::set<line_number>>> word_map;
};

/**
 * TextQuery constructor takes an `ifstream` object
 * and populates its corresponding file by calling
 * `push_back`. Each `push_back` increments the
 * `current_line_number`.
 * TextQuery does most of the heavy-lifting.
 * @param is reference to an `ifstream` object.
 */
TextQuery::TextQuery(std::ifstream& is)
    : file(std::make_shared<std::vector<std::string>>())
{
    std::string text;
    while (std::getline(is, text)) {
        file->push_back(text);

        auto current_line_number = file->size() - 1;
        std::istringstream line(text);
        std::string word;

        // TODO: use auto lookup != word_map.end() pattern instead of word_map[word]
        while (line >> word) {
            auto& lines = word_map[word];
            if (!lines) {
                lines.reset(new std::set<line_number>);
            }
            lines->insert(current_line_number);
        }
    }
}

class QueryResult {
    friend std::ostream& display(std::ostream&, const QueryResult&);
    friend class Query;
    using line_number = std::vector<std::string>::size_type;

public:
    QueryResult(std::string s, std::shared_ptr<std::set<line_number>> p,
        std::shared_ptr<std::vector<std::string>> f)
        : sought(std::move(s))
        , lines(std::move(p))
        , file(std::move(f))
    {
    }

    // `begin` iterator to `lines` set of `line_numbers`
    std::set<line_number>::iterator begin() const { return lines->begin(); }
    // `end` iterator to `lines` set of `line_numbers`
    std::set<line_number>::iterator end() const { return lines->end(); }
    // `shared_ptr` to this input_file `file`
    std::shared_ptr<std::vector<std::string>> get_file() const { return file; }

private:
    // `sought` is the queried-for std::string
    std::string sought;
    // `lines` is the set of lines where `sought` is found
    std::shared_ptr<std::set<line_number>> lines;
    // `file` is a vectorized string representation of input file
    std::shared_ptr<std::vector<std::string>> file;
};

QueryResult TextQuery::query(const std::string& sought) const
{
    static std::shared_ptr<std::set<line_number>>
        empty_set(std::make_shared<std::set<line_number>>());
    auto lookup = word_map.find(sought);

    // if lookup fails to find `sought` return `empty_set`
    // of `line_number`(s); otherwise, return `word_map`
    // `lookup`(s) set of `line_numer`(s)
    if (lookup == word_map.end()) {
        return QueryResult(sought, empty_set, file);
    } else {
        return QueryResult(sought, lookup->second, file);
    }
}

/* abstract */
class QueryBase {
    friend class Query;

protected:
    // `line_number` as defined in TextQuery
    using line_number = TextQuery::line_number;
    virtual ~QueryBase() = default;

private:
    // return the QueryResult that matches a given Query
    virtual QueryResult evaluate(const TextQuery&) const = 0;
    // repr is a std::string representation of a given Query
    virtual std::string repr() const = 0;
};

/* concrete */
class WordQuery : public QueryBase {
    friend class Query;

    WordQuery(const std::string& s)
        : query_word(s)
    {
    }

    // concrete: definitions for all inherited virtual functions
    QueryResult evaluate(const TextQuery& text_query) const
        override { return text_query.query(query_word); }

    std::string repr() const override { return query_word; }
    // WordQuery "holds" the queried-for word
    std::string query_word;
};

/**
 * interface (concrete): does not inherit from QueryBase
 * provides access through non-overriden member functions
 */
class Query {
    friend Query operator~(const Query&);
    friend Query operator&(const Query&, const Query&);
    friend Query operator|(const Query&, const Query&);

public:
    // build a new WordQuery
    Query(const std::string& s)
        : through_query_base(new WordQuery(s)){};

    // interface functions -->> calls corresponding QueryBase operation(s)
    QueryResult evaluate(const TextQuery& text_query_object) const
    {
        return through_query_base->evaluate(text_query_object);
    }

    std::string repr() const { return through_query_base->repr(); }

private:
    std::shared_ptr<QueryBase> through_query_base;

    Query(std::shared_ptr<QueryBase> query_object)
        : through_query_base(query_object)
    {
    }
};

/* abstract : concrete subclasses must define `evaluate` */
class BinaryQuery : public QueryBase {
protected:
    // construct from two Query objects `lhs` and `rhs` given a particular symbol
    // (i.e. "&" or "|").
    BinaryQuery(const Query& _lhs, const Query& _rhs, std::string symbol)
        : lhs(_lhs)
        , rhs(_rhs)
        , operand_symbol(symbol)
    {
    }

    Query lhs, rhs;

    // representation of BinaryQuery
    std::string repr() const { return "("
        + lhs.repr() + " "
        + operand_symbol + " "
        + rhs.repr() + ")"; }

    // operand symbol: expects "&" or "|"
    std::string operand_symbol;
};

/* concrete */
class AndQuery : public BinaryQuery {
    friend Query operator&(const Query&, const Query&);

    AndQuery(const Query& lhs, const Query& rhs)
        : BinaryQuery(lhs, rhs, "&")
    {
    }

    // concrete: definitions of all inherited virtual functions
    QueryResult evaluate(const TextQuery&) const override;
};

inline Query operator&(const Query& lhs, const Query& rhs)
{
    return std::shared_ptr<QueryBase>(new AndQuery(lhs, rhs));
}

// evaluates AndQuery given a TextQuery object
QueryResult AndQuery::evaluate(const TextQuery& text) const
{
    auto left = lhs.evaluate(text), right = rhs.evaluate(text);
    auto res = std::make_shared<std::set<line_number>>();
    std::set_intersection(left.begin(), left.end(), right.begin(), right.end(),
        std::inserter(*res, res->begin()));
    return QueryResult(repr(), res, left.get_file());
}

/* concrete */
class OrQuery : public BinaryQuery {
    friend Query operator|(const Query&, const Query&);
    OrQuery(const Query& lhs, const Query& rhs)
        : BinaryQuery(lhs, rhs, "|")
    {
    }
    QueryResult evaluate(const TextQuery&) const override;
};

inline Query operator|(const Query& lhs, const Query& rhs)
{
    return std::shared_ptr<QueryBase>(new OrQuery(lhs, rhs));
}

// evaluates OrQuery given a TextQuery object
QueryResult OrQuery::evaluate(const TextQuery& text) const
{
    auto right = rhs.evaluate(text), left = lhs.evaluate(text);
    auto res = std::make_shared<std::set<line_number>>(left.begin(), left.end());
    res->insert(right.begin(), right.end());
    return QueryResult(repr(), res, left.get_file());
}

/* concrete */
class NotQuery : public QueryBase {
    friend Query operator~(const Query&);

    NotQuery(const Query& q)
        : query(q)
    {
    }

    // concrete: definitions for all inherited virtual functions
    std::string repr() const override { return "~(" + query.repr() + ")"; };
    QueryResult evaluate(const TextQuery&) const override;
    Query query;
};

inline Query operator~(const Query& operand)
{
    return std::shared_ptr<QueryBase>(new NotQuery(operand));
}

// evaluates NotQuery given a TextQuery object
QueryResult NotQuery::evaluate(const TextQuery& text) const
{
    auto result = query.evaluate(text);
    auto res_lines = std::make_shared<std::set<line_number>>();
    auto _begin = result.begin(), _end = result.end();
    auto sz = result.get_file()->size();

    for (auto number = 0; number != 0; ++number) {
        if (_begin == _end || *_begin != number) {
            res_lines->insert(number);
        } else if (_begin != _end) {
            ++_begin;
        }
    }
    return QueryResult(repr(), res_lines, result.get_file());
}

} // namespace Query

// utility functions
namespace Query {

std::ostream& operator<<(std::ostream& os, const Query& query)
{
    return os << query.repr();
}

std::string make_plural(const std::string::size_type count,
    const std::string& word, const std::string& modifier)
{
    return (count > 1) ? word + modifier : word;
}

std::ostream& display(std::ostream& os, const QueryResult& query_result)
{
    os << query_result.sought << " occurs " << query_result.lines->size() << " "
       << make_plural(query_result.lines->size(), "time", "s") << std::endl;

    for (auto num : *query_result.lines) {
        os << "\t(line " << num + 1 << ") " << *(query_result.file->begin() + num) << '\n';
    }
    return os;
}

} // namespace Query
