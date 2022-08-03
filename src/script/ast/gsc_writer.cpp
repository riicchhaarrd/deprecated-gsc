#include "gsc_writer.h"

void script::GSCWriter::visit(ast::SwitchCase& n)
{
	if (n.test)
	{
		os >> "case ";
		n.test->accept(*this);
		os << ":" << std::endl;
	}
	else
	{
		os >> "default:" << std::endl;
	}
	os.indent(1);
	for (auto& stmt : n.consequent)
		stmt->accept(*this);
	os.indent(-1);
	os >> "break;" << std::endl;
	os << "" << std::endl;
}

void script::GSCWriter::visit(ast::Directive& n)
{
	os << "#";
	os << n.directive;
	if (n.directive == "include")
	{
		os << " ";
		os << n.value;
	}
	else
	{
		os << "(\"";
		os << n.value;
		os << "\")";
	}
	os << ";" << std::endl;
}
void script::GSCWriter::visit(ast::ContinueStatement& n)
{
	os >> "continue;";
}

void script::GSCWriter::visit(ast::SwitchStatement& n)
{
	os >> "switch(";
	n.discriminant->accept(*this);
	os << ")" << std::endl;
	os >> "{";
	os << "" << std::endl;
	os.indent(1);
	for (auto& c : n.cases)
		c->accept(*this);
	os.indent(-1);
	os >> "}";
	os << "" << std::endl;
}
