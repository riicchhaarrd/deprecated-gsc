#include "preprocessor.h"
#include <parse/expression_parser.h>

bool parse::preprocessor::resolve_identifier(token_parser& parser, std::string ident, token_list& preprocessed_tokens,
											 definition_map& definitions)
{

	parse::token t;
	auto def = definitions.find(ident);
	if (def != definitions.end())
	{
		//check if it's a function macro
		if (def->second.is_function)
		{
			if(!parser.accept_token(t, '('))
				throw preprocessor_error("function macro, expecting (", t.to_string(), t.line_number());

			//build token list arguments of parameters
			std::vector<token_list> arguments;

			token_list tl;
			while (1)
			{
				t = parser.read_token();
				if (t.type == parse::token_type::eof)
					throw preprocessor_error("unexpected eof", t.to_string(), t.line_number());
				int tai = t.type_as_int();
				if (tai == ',' || tai == ')')
				{
					arguments.push_back(tl);
					tl.clear();
					if (tai == ',')
						continue;
					else
						break;
				}
				tl.push_back(t);
			}

			parse_opts popts;
			popts.newlines = true;
			token_parser def_parser(def->second.body, popts);
			while (1)
			{
				t = def_parser.read_token();
				if (t.type == parse::token_type::eof)
					break;
				/*
				//scrap this
				if (t.type_as_int() == '$')
				{
					ExpressionParser expression_parser(def_parser,
					[&def,&arguments](const token& t, float& f) -> bool
					{ 
						if (t.type != token_type::identifier)
							return false;
						// if so, check if it matches any of the parameters
						auto parm = def->second.parameters.find(t.to_string());
						if (parm == def->second.parameters.end())
						{
							throw preprocessor_error("no such parameter", t.to_string(), t.line_number());
						}
						if (parm->second >= arguments.size())
							throw preprocessor_error("argument out of bounds for macro function", t.to_string(),
														t.line_number());
						float total = 0.f;
						for (auto& it : arguments[parm->second])
						{
							total += it.to_float();
						}
						return total;
					});
					preprocessed_tokens.push_back(parse::token(std::to_string(expression_parser.parse()), parse::token_type::number));
				} else
				*/
				if (t.type_as_int() == '#')
				{
					if (!def_parser.accept_token(t, parse::token_type::identifier))
						throw preprocessor_error(common::format("expected identifier got {}", t.type_as_string()), t.to_string(), t.line_number());

					// if so, check if it matches any of the parameters
					auto parm = def->second.parameters.find(t.to_string());
					if (parm == def->second.parameters.end())
					{
						throw preprocessor_error("no such parameter", t.to_string(), t.line_number());
					}
					// we found the parameter, so stringify it
					if (parm->second >= arguments.size())
						throw preprocessor_error("argument out of bounds for macro function", t.to_string(), t.line_number());
						
					std::string concatenation;
					for (auto& it : arguments[parm->second])
					{
						concatenation += it.to_string();
					}
					preprocessed_tokens.push_back(parse::token(concatenation, parse::token_type::string));
				}
				else if (t.type == parse::token_type::string) // is the current token in our block/body a ident?
				{
					std::string concatenation = t.to_string();
					while (def_parser.accept_token(t, parse::token_type::pound_pound)) // concatenate
					{
						if (!def_parser.accept_token(t, parse::token_type::identifier))
							throw preprocessor_error("expected identifier", t.to_string(), t.line_number());
						auto parm = def->second.parameters.find(t.to_string());
						if (parm == def->second.parameters.end())
						{
							throw preprocessor_error("no such parameter", t.to_string(), t.line_number());
						}
						if (parm->second >= arguments.size())
							throw preprocessor_error("argument out of bounds for concatenation", t.to_string(),
													 t.line_number());
						auto& tl = arguments[parm->second];
						if (tl.size() > 1)
						{
							throw preprocessor_error("expected 1 argument for concatenation", t.to_string(),
													 t.line_number());
						}
						auto& tl1 = tl[0];
						concatenation += tl1.to_string();
					}
					/*while (1)
					{
						auto tk = def_parser.read_token();
						printf("concat=%s,type=%d '%s'\n", concatenation.c_str(), tk.type_as_int(),
							   tk.to_string().c_str());
						getchar();
					}*/
					preprocessed_tokens.push_back(parse::token(concatenation, parse::token_type::string));
				}
				else if (t.type_as_int() == (int)parse::token_type::identifier)
				{
					// if so, check if it matches any of the parameters
					auto parm = def->second.parameters.find(t.to_string());
					if (parm != def->second.parameters.end())
					{
						// we found the parameter, so replace it..
						if (parm->second >= arguments.size())
							throw preprocessor_error("argument out of bounds for macro function", t.to_string(), t.line_number());
						auto& tl = arguments[parm->second];
						for (auto& tl_it : tl)
						{
							if (tl_it.type_as_int() == (int)parse::token_type::identifier)
							{
								if (!resolve_identifier(def_parser, tl_it.to_string(), preprocessed_tokens,
														definitions))
									preprocessed_tokens.push_back(tl_it);
							}
							else
							{
								preprocessed_tokens.push_back(tl_it);
							}
						}
					}
					else
					{
						if (!resolve_identifier(def_parser, t.to_string(), preprocessed_tokens, definitions))
							preprocessed_tokens.push_back(t);
					}
				}
				else
				{
					preprocessed_tokens.push_back(t);
				}
			}
		}
		else
		{
			// printf("found %s\n", t.to_string().c_str());
			parse_opts popts;
			popts.newlines = true;
			token_parser def_parser(def->second.body, popts);
			while (1)
			{
				t = def_parser.read_token();
				if (t.type == parse::token_type::eof)
					break;
				if (t.type == parse::token_type::identifier)
				{
					if (!resolve_identifier(def_parser, t.to_string(), preprocessed_tokens, definitions))
						preprocessed_tokens.push_back(t);
				}
				else
				{
					preprocessed_tokens.push_back(t);
				}
			}
		}
		return true;
	}
	return false;
}
