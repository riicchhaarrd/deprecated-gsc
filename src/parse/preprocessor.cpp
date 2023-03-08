#include "preprocessor.h"

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

			while (1)
			{
				token_list tl;
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

			for (auto& it : def->second.body)
			{
				// is the current token in our block/body a ident?
				if (it.type_as_int() == (int)parse::token_type::identifier)
				{
					// if so, check if it matches any of the parameters
					auto parm = def->second.parameters.find(it.to_string());
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
								if (!resolve_identifier(parser, tl_it.to_string(), preprocessed_tokens, definitions))
									preprocessed_tokens.push_back(tl_it);
							}
							else
							{
								preprocessed_tokens.push_back(tl_it);
							}
						}
					}
				}
			}
		}
		else
		{
			// printf("found %s\n", t.to_string().c_str());
			for (auto& it : def->second.body)
			{
				// preprocessed_tokens.push_back(it);
				if (!resolve_identifier(parser, it.to_string(), preprocessed_tokens, definitions))
					preprocessed_tokens.push_back(it);
			}
		}
		return true;
	}
	return false;
}
