#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <parse/preprocessor.h>
#include <core/filesystem/api.h>

#include <script/ast/node/literal.h>

namespace script
{
	struct ASTException : std::exception
	{
		std::string message_;
		template <typename... Ts> ASTException(std::string_view fmt, Ts&&... ts)
		{
			message_ = std::vformat(fmt, std::make_format_args(std::forward<Ts>(ts)...));
		}
		const char* what() const noexcept override
		{
			return message_.c_str();
		}
	};

	class ASTGenerator
	{
	  public:
		ASTGenerator()
		{
		}
		~ASTGenerator()
		{
		}

		bool accept(int token_type)
		{
		}

		bool generate(filesystem_api &fs, const std::string base_path, const std::string path)
		{
			try
			{
				parse::source_map sources;
				parse::token_list tokens;
				parse::definition_map definitions;
				parse::preprocessor proc;
				parse::lexer_opts opts;
				opts.backslash_comments = true;
				if (proc.preprocess(fs, base_path, path, tokens, sources, definitions, opts))
				{
				}
			}
			catch (std::exception& e)
			{
				throw ASTException("something went wrong generating syntax tree {}", e.what());
			}
		}
	};
}; // namespace script