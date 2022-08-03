#include "visitor.h"
#include <script/ast/node/node.h>

void script::ast::ASTVisitor::visit_node(Node& n)
{
	n.accept(*this);
}
