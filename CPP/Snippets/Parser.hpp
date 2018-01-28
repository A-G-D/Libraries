/*******************************************************************************
*
*	parser.hpp
*
*	Parser
*
*
*	Author: AGD
*
*
*	API:
*
*		class Parser
*
*			Constructors:
*
*				Parser()
*					- Default Constructor
*				Parser(const Parser &parser)
*					- Copy Constructor
*
*			Fields:
*
*				bool stretch_spaces
*				bool stretch_tabs
*				bool stretch_linefeeds
*
*			Methods:
*
*				void set_syntax(const std::string &str, char delimiter)
*					- Sets the syntax to be recognized by the Parser
*				List< List<std::string> > parse(const std::string &str, unsigned int match_limit = 0)
*					- Parses the input string and returns the arguments of the substrings that matched the
*					  syntax as a List of Lists of strings
*					- See example below for a clearer explanation
*
*				Parser operator+(const Parser &parser)
*				Parser& operator+=(const Parser &parser)
*
*				Parser& operator=(const Parser &parser)
*					- Copy assignment operator
*
*
*	Example:
*
*		Parser parser; parser.set_syntax("int ", ' ');
*		parser += Parser().set_syntax("= ", '\n');
*
*		std::string text("
*						  int x = 1
*						  int y = 2
*						  int z = 3
*						 ");
*		List< List<std::string> > list(parser.parse(text));
*
*		std::cout<<list.first().data().first().data(); 			//displays "x"
*		std::cout<<list.first().data().last().data();  			//displays "1"
*
*		std::cout<<list.first().next().data().first().data(); 	//displays "y"
*		std::cout<<list.first().next().data().last().data();  	//displays "2"
*
*		std::cout<<list.last().data().first().data(); 			//displays "z"
*		std::cout<<list.last().data().last().data();  			//displays "3"
*
*
*******************************************************************************/
#ifndef __PARSER_HPP__
#define __PARSER_HPP__


#include <string>
#include <sequence.hpp>
#include <linkedlist.hpp>


class Parser {


	List< Sequence<char> > sequence;
	List<char> delimiter;

public:

	bool stretch_spaces;
	bool stretch_tabs;
	bool stretch_linefeeds;

	void set_syntax(const std::string &str, char input_delimiter) {
		sequence.clear();
		delimiter.clear();
		delimiter.enqueue(input_delimiter);
		Sequence<char> seq(true);
		for(int i = 0; i < str.size(); ++i)
			seq.enqueue(str[i], (stretch_spaces && str[i] == ' ') || (stretch_tabs && str[i] == '\t') || (stretch_linefeeds && str[i] == '\n'));
		sequence.enqueue(seq);
	}

	List< List<std::string> > parse(const std::string &str, unsigned int limit = 0) {
		bool matched;
		bool prev_match = true;
		unsigned int match_limit = limit;
		List< List<std::string> > input;
		List<std::string> input_temp;
		List<char>::Node delim;
		std::string temp;
		int position = 0;
		while(true) {
			delim = delimiter.first();
			for(List< Sequence<char> >::Node node = sequence.first(); node != sequence.head(); ++node) {
				temp = "";
				matched = false;
				while(position < str.size()) {
					if(!node.data().solved())
						node.data().decode(str[position]);
					else if(str[position] == delim.data()) {
						matched = true;
						input_temp.enqueue(temp);
						break;
					}else
						temp += str[position];
					++position;
				}
				prev_match = (prev_match && matched);
				node.data().reset();
				++delim;
			}
			if(prev_match) {
				input.enqueue(List<std::string>());
				for(List<std::string>::Node node = input_temp.first(); node != input_temp.head(); ++node)
					input.last().data().enqueue(node.data());
				if(limit > 0 && --match_limit == 0)
					break;
			}
			input_temp.clear();
			if(position == str.size())
				break;
		}
		return input;
	}

	Parser& operator=(const Parser &other) {
		sequence = other.sequence;
		delimiter = other.delimiter;
		stretch_spaces = other.stretch_spaces;
		stretch_tabs = other.stretch_tabs;
		stretch_linefeeds = other.stretch_linefeeds;
		return *this;
	}

	Parser& operator+=(const Parser &other) {
		for(List< Sequence<char> >::Node node = other.sequence.first(); node != other.sequence.head(); ++node)
			sequence.enqueue(node.data());
		for(List<char>::Node node = other.delimiter.first(); node != other.delimiter.head(); ++node)
			delimiter.enqueue(node.data());
		return *this;
	}

	Parser operator+(const Parser &other) {
		Parser temp(*this);
		return temp += other;
	}

	Parser() {}
	Parser(const Parser &other)
	: sequence(other.sequence), delimiter(other.delimiter), stretch_spaces(other.stretch_spaces),
	  stretch_tabs(other.stretch_tabs), stretch_linefeeds(other.stretch_linefeeds) {
	}

	~Parser() {}


};
#endif
