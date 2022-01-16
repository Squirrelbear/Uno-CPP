#pragma once

#include "TurnAction.h"

/**
 * Prints out the tree from the specified node downward splitting at any TurnDecisionAction.
 *
 * @param currentNode The node to print at this level of the iteration.
 * @param indentLevel Indicates how far to indent text for the output and for numbering.
 */
void debugRecursiveNodeOutput(TurnAction* currentNode, int indentLevel);
