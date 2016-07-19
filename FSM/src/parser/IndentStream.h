#ifndef POCO_CMP_INDENTSTREAM_H_
#define POCO_CMP_INDENTSTREAM_H_

#include <iostream>
#include <streambuf>
#include <locale>
#include <cstdio>

using std::streambuf;
using std::fill_n;
using std::ostream;
using std::ostreambuf_iterator;


class IndentBuffer : public streambuf {
  public:

	IndentBuffer(streambuf* sbuf) : _sbuf(sbuf), _tab(0), _need(true) {}

	int tab() const {
		return _tab;
	}
	void tab() {
		_tab += 1;
	}
	void back() {
		if (_tab >= 0) _tab -= 1;
	}

  protected:
	virtual int_type overflow(int_type c) {

		if (traits_type::eq_int_type(c, traits_type::eof()))
			return _sbuf->sputc(c);

		if (_need) {
			fill_n(std::ostreambuf_iterator<char>(_sbuf), _tab, '\t');
			_need = false;
		}

		if (traits_type::eq_int_type(_sbuf->sputc(c), traits_type::eof()))
			return traits_type::eof();

		if (traits_type::eq_int_type(c, traits_type::to_char_type('\n')))
			_need = true;

		return traits_type::not_eof(c);
	}

	streambuf* _sbuf;
	int _tab;
	bool _need;
};

std::ostream& tab(std::ostream& stream);
std::ostream& back(std::ostream& stream);

class IndentStream : public std::ostream {
  public:
	IndentStream(std::ostream &os) : ib(os.rdbuf()), std::ostream(&ib) {};

	friend std::ostream& tab(std::ostream& stream);

	friend std::ostream& back(std::ostream& stream);

  private:
	IndentBuffer ib;
};
#endif