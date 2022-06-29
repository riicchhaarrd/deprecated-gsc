#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <exception>
#include "token_parser.h"
#include "lexer.h"

namespace parse
{
	using source_map = std::unordered_map<std::string, parse::source>;
	using definition_map = std::unordered_map<std::string, token_list>;

	struct preprocessor_error : std::exception
	{
		std::string file;
		const char *msg;
		int linenumber;
		preprocessor_error(const char* message, const std::string& _file, int _linenumber)
			: file(_file), linenumber(_linenumber), msg(message)
		{
		}

		const char* what() const noexcept override
		{
			return msg;
		}
	};
	class preprocessor
	{
	  public:
		// recursively resolve
		void resolve_identifier(parse::token& t, token_list& preprocessed_tokens, definition_map& definitions)
		{
			auto fnd = definitions.find(t.to_string());
			if (fnd != definitions.end())
			{
				// printf("found %s\n", t.to_string().c_str());
				for (auto& it : fnd->second)
				{
					// preprocessed_tokens.push_back(it);
					resolve_identifier(it, preprocessed_tokens, definitions);
				}
			}
			else
				preprocessed_tokens.push_back(t);
		}

		bool preprocess(const std::string& path_base, const std::string& path, token_list& preprocessed_tokens,
						source_map& sources, definition_map& definitions, int depth = 0)
		{
			std::ifstream in(path);
			if (!in.is_open())
			{
				printf("failed to open file '%s'\n", path.c_str());
				return false;
			}
			std::stringstream ss;
			ss << in.rdbuf();

			sources.insert(std::make_pair(path, parse::source(path, ss.str())));
			parse::lexer_opts opts;
			opts.tokenize_newlines = true;
			parse::lexer lexer(&sources[path]);

			token_list tokens;

			try
			{
				tokens = lexer.tokenize();
			}
			catch (parse::lexer_error& err)
			{
				throw preprocessor_error("failed to tokenize file", path, err.line1);
			}
			parse_opts popts;
			popts.newlines = true;
			token_parser parser(tokens, popts);

			while (1)
			{
				auto t = parser.read_token();
				if (t.type == parse::token_type::eof)
					break;
				switch (t.type_as_int())
				{
				case (int)parse::token_type::identifier:
				{
					resolve_identifier(t, preprocessed_tokens, definitions);
				}
				break;

				case '#':
				{
					std::string directive = parser.read_identifier();
					if (directive == "include")
					{
						if (parser.accept_token(t, parse::token_type::string))
						{
							// for (int i = 0; i < depth; ++i)
							// putchar('\t');
							// printf("including %s\n", t.to_string().c_str());
							token_list tmp;
							if (!preprocess(path_base, path_base + t.to_string(), tmp, sources, definitions, depth + 1))
								throw preprocessor_error("failed to preprocess file", t.to_string(), t.line_number());
							preprocessed_tokens.insert(preprocessed_tokens.end(), tmp.begin(), tmp.end());
						}
						else
							throw preprocessor_error("invalid include directive", t.to_string(), t.line_number());
					}
					else if (directive == "define")
					{
						if (!parser.accept_token(t, parse::token_type::identifier))
							throw preprocessor_error("expected identifier", path, t.line_number());
						// parse rest of line
						token_list deftokens;
						parser.read_tokens_till(deftokens, '\n');
						/*std::string definition;
						for (auto& it : deftokens)
							definition += it.to_string();
						*/
						// printf("definition %s -> %s\n", t.to_string().c_str(), definition.c_str());
						definitions[t.to_string()] = deftokens;
					}
				}
				break;

				default:
					preprocessed_tokens.push_back(t);
					break;
				}
				// printf("token: type=%d, %d, %d [%s]\n", t.type, t.pos, t.sz, t.to_string().c_str());
			}

			return true;
		}
	};
};