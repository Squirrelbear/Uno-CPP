#include "TurnActionSequenceUtility.h"

#include "TurnDecisionAction.h"
#include <iostream>

void debugRecursiveNodeOutput(TurnAction * currentNode, int indentLevel)
{
	if (currentNode == nullptr) return;
	for (int i = 0; i < indentLevel; i++) {
		std::cout << "\t";
	}

	TurnDecisionAction* currentSplitNode = dynamic_cast<TurnDecisionAction*>(currentNode);
	if (currentSplitNode != nullptr) {
		std::cout << "? " << (indentLevel + 1) << ". " << currentSplitNode->getFlagName()
			<< " Timeout: " << currentSplitNode->getTimeOut() << " " << currentSplitNode->getActionDebugText() << std::endl;
			debugRecursiveNodeOutput(currentSplitNode->getNextPointer(), indentLevel + 1);
			if (currentSplitNode->getNextPointer() != currentSplitNode->getOtherNextPointer()) {
				debugRecursiveNodeOutput(currentSplitNode->getOtherNextPointer(), indentLevel + 1);
			}
	}
	else {
		std::cout << "- " << (indentLevel + 1) << ". " << currentNode->getActionDebugText() << std::endl;
		debugRecursiveNodeOutput(currentNode->getNextPointer(), indentLevel + 1);
	}
}
