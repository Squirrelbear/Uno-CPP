#include "LobbyInterface.h"

LobbyInterface::LobbyInterface(const sf::IntRect & bounds, const sf::Font & font)
	: WndInterface(bounds)
{
	_ruleSet = new RuleSet();

	std::vector<std::string> aiNames = getRandomAINameList();
	_playerList.emplace_back(new LobbyPlayer("Player", Player::PlayerType::ThisPlayer, sf::IntRect(20, 100, bounds.width / 2, 100), font));
	_playerList.emplace_back(new LobbyPlayer(aiNames.at(0), Player::PlayerType::AIPlayer, sf::IntRect(20, 100+120, bounds.width / 2, 100), font));
	_playerList.emplace_back(new LobbyPlayer(aiNames.at(0), Player::PlayerType::AIPlayer, sf::IntRect(20, 100 + 120*2, bounds.width / 2, 100), font));
	_playerList.emplace_back(new LobbyPlayer(aiNames.at(0), Player::PlayerType::AIPlayer, sf::IntRect(20, 100 + 120*3, bounds.width / 2, 100), font));

	initialiseBackground(bounds, font);
	initialiseRuleOptions(bounds, font);
}

LobbyInterface::~LobbyInterface()
{
	delete _background;
	for (auto& p : _playerList) {
		delete p;
	}
}

void LobbyInterface::draw(sf::RenderWindow & renderWindow) const
{
	_background->draw(renderWindow);

	for (const auto& button : _buttonList) {
		button.draw(renderWindow);
	}
	for (const auto& lobbyPlayer : _playerList) {
		lobbyPlayer->draw(renderWindow);
	}

	// Draw all dynamic strings for the rules.
	for (const auto&[key, ruleString] : _ruleStrings) {
		renderWindow.draw(ruleString);
	}
}

void LobbyInterface::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (!isEnabled()) return;

	for (auto& button : _buttonList) {
		if (button.isPositionInside(mousePosition)) {
			handleButtonPress(button.getActionID());
		}
	}
	for (auto& lobbyPlayer : _playerList) {
		if (lobbyPlayer->isPositionInside(mousePosition)) {
			lobbyPlayer->handleClick();
		}
	}
}

void LobbyInterface::handleMouseMove(const sf::Vector2i & mousePosition)
{
	if (!isEnabled()) return;

	for (auto& button : _buttonList) {
		button.setHovering(button.isPositionInside(mousePosition));
	}
	for (auto& lobbyPlayer : _playerList) {
		lobbyPlayer->updateHoverState(mousePosition);
	}
}

RuleSet * LobbyInterface::getRuleSet() const
{
	return _ruleSet;
}

std::vector<LobbyPlayer*> LobbyInterface::getLobbyPlayerList() const
{
	return _playerList;
}

void LobbyInterface::handleButtonPress(const int actionID)
{
	switch (actionID) {
		case 1: toggleNumberOfPlayers(); break;
		// TODO need to add start game.
		//case 2: gamePanel.startGame(playerList, ruleSet); break;
		case 3: toggleStackRule(); break;
		case 4: toggleDrawTillCanPlayRule(); break;
		case 5: toggleSevenZeroRule(); break;
		case 6: toggleJumpInRule(); break;
		case 7: toggleForcedPlayRule(); break;
		case 8: toggleNoBluffingRule(); break;
		case 9: cycleScoreLimit(); break;
		case 10: resetRulesToDefault(); break;
	}
}

void LobbyInterface::toggleNumberOfPlayers()
{
	_ruleSet->setTwoPlayers(!_ruleSet->getOnlyTwoPlayers());
	updatePlayerNumberStatus();
}

void LobbyInterface::updatePlayerNumberStatus()
{
	_playerList.at(2)->setEnabled(!_ruleSet->getOnlyTwoPlayers());
	_playerList.at(3)->setEnabled(!_ruleSet->getOnlyTwoPlayers());

	_ruleStrings["twoPlayerPrefix"].setString(_ruleSet->getOnlyTwoPlayers() ? "Two Players:" : "Four Players:");
	_ruleStrings["twoPlayerRule"].setString(_ruleSet->getOnlyTwoPlayers() ? "Reverse is Skip" : "Normal Rules for Reverse");
}

void LobbyInterface::toggleStackRule()
{
	_ruleSet->setCanStackCards(!_ruleSet->canStackCards());
	updateStackRuleLabel();
}

void LobbyInterface::updateStackRuleLabel()
{
	_ruleStrings["stackRule"].setString(_ruleSet->canStackCards() ? "Stacking +2/+4: On" : "Stacking +2/+4: Off");
}

void LobbyInterface::toggleDrawTillCanPlayRule()
{
	_ruleSet->setDrawnTillCanPlay(!_ruleSet->shouldDrawnTillCanPlay());
	updateDrawTillCanPlayRuleLabel();
}

void LobbyInterface::updateDrawTillCanPlayRuleLabel()
{
	_ruleStrings["drawTillCanPlayRule"].setString(_ruleSet->shouldDrawnTillCanPlay() ? "Draw Till Can Play: On" : "Draw Till Can Play: Off");
}

void LobbyInterface::toggleSevenZeroRule()
{
	_ruleSet->setSevenZeroRule(!_ruleSet->getSevenZeroRule());
	updateSevenZeroRuleLabel();
}

void LobbyInterface::updateSevenZeroRuleLabel()
{
	_ruleStrings["sevenZeroRule"].setString(_ruleSet->getSevenZeroRule() ? "Seven-0: On (7=Swap, 0=Pass All)" : "Seven-0: Off");
}

void LobbyInterface::toggleForcedPlayRule()
{
	_ruleSet->setForcedPlayRule(!_ruleSet->getForcedPlayRule());
	updateForcedPlayRuleLabel();
}

void LobbyInterface::updateForcedPlayRuleLabel()
{
	_ruleStrings["forcedPlayRule"].setString(_ruleSet->getForcedPlayRule() ? "Forced Play: On" : "Forced Play: Off");
}

void LobbyInterface::toggleJumpInRule()
{
	_ruleSet->setAllowJumpInRule(!_ruleSet->allowJumpInRule());
	updateJumpInRuleLabel();
}

void LobbyInterface::updateJumpInRuleLabel()
{
	_ruleStrings["jumpInRule"].setString(_ruleSet->allowJumpInRule() ? "Jump In: On" : "Jump In: Off");
}

void LobbyInterface::toggleNoBluffingRule()
{
	_ruleSet->setNoBuffingRule(!_ruleSet->getNoBluffingRule());
	updateNoBluffingRuleLabel();
}

void LobbyInterface::updateNoBluffingRuleLabel()
{
	_ruleStrings["noBluffingRule"].setString(_ruleSet->getNoBluffingRule() ? "No Bluffing: On" : "No Bluffing: Off");
}

void LobbyInterface::cycleScoreLimit()
{
	switch (_ruleSet->getScoreLimitType()) {
		case RuleSet::ScoreLimitType::OneRound: _ruleSet->setScoreLimitType(RuleSet::ScoreLimitType::Score200); break;
		case RuleSet::ScoreLimitType::Score200: _ruleSet->setScoreLimitType(RuleSet::ScoreLimitType::Score300); break;
		case RuleSet::ScoreLimitType::Score300: _ruleSet->setScoreLimitType(RuleSet::ScoreLimitType::Score500); break;
		case RuleSet::ScoreLimitType::Score500: _ruleSet->setScoreLimitType(RuleSet::ScoreLimitType::Unlimited); break;
		case RuleSet::ScoreLimitType::Unlimited: _ruleSet->setScoreLimitType(RuleSet::ScoreLimitType::OneRound); break;
	}
	updateScoreLimitLabel();
}

void LobbyInterface::updateScoreLimitLabel()
{
	switch (_ruleSet->getScoreLimitType()) {
		case RuleSet::ScoreLimitType::OneRound: _ruleStrings["scoreLimit"].setString("Score Limit: One Round"); break;
		case RuleSet::ScoreLimitType::Score200: _ruleStrings["scoreLimit"].setString("Score Limit: 200 Points"); break;
		case RuleSet::ScoreLimitType::Score300: _ruleStrings["scoreLimit"].setString("Score Limit: 300 Points"); break;
		case RuleSet::ScoreLimitType::Score500: _ruleStrings["scoreLimit"].setString("Score Limit: 500 Points"); break;
		case RuleSet::ScoreLimitType::Unlimited: _ruleStrings["scoreLimit"].setString("Score Limit: Unlimited"); break;
	}
}

void LobbyInterface::resetRulesToDefault()
{
	_ruleSet->setToDefaults();
	updateAllRuleLabels();
}

void LobbyInterface::updateAllRuleLabels()
{
	updateStackRuleLabel();
	updateDrawTillCanPlayRuleLabel();
	updatePlayerNumberStatus();
	updateSevenZeroRuleLabel();
	updateJumpInRuleLabel();
	updateForcedPlayRuleLabel();
	updateNoBluffingRuleLabel();
	updateScoreLimitLabel();
}

std::vector<std::string> LobbyInterface::getRandomAINameList()
{
	std::vector<std::string> names;
	names.emplace_back("NOT_TODO");
	return names;
}

void LobbyInterface::initialiseRuleOptions(const sf::IntRect& bounds, const sf::Font& font)
{
	_ruleStrings["twoPlayerPrefix"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["twoPlayerPrefix"].setPosition(sf::Vector2f(bounds.width / 2 + 140, 270-20));
	_ruleStrings["twoPlayerRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["twoPlayerRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 270 - 20));
	_ruleStrings["stackRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["stackRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 175 - 20));
	_ruleStrings["drawTillCanPlayRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["drawTillCanPlayRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 225 - 20));
	_ruleStrings["sevenZeroRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["sevenZeroRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 315 - 20));
	_ruleStrings["forcedPlayRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["forcedPlayRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 415 - 20));
	_ruleStrings["jumpInRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["jumpInRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 365 - 20));
	_ruleStrings["noBluffingRule"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["noBluffingRule"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 465 - 20));
	_ruleStrings["scoreLimit"] = sf::Text("NOTSET", font, 20);
	_ruleStrings["scoreLimit"].setPosition(sf::Vector2f(bounds.width / 2 + 300, 515 - 20));

	for (auto&[key, textElement] : _ruleStrings) {
		textElement.setStyle(sf::Text::Bold);
		textElement.setFillColor(sf::Color::Black);
	}

	updateAllRuleLabels();
	_buttonList.emplace_back(Button(sf::IntRect(bounds.width / 4 - 150, bounds.height - 100, 300, 60), "Toggle Number of Players", 1, font));
	_buttonList.emplace_back(Button(sf::IntRect(bounds.width * 3 / 4 - 150, bounds.height - 100, 300, 60), "Start Game", 2, font));
	_buttonList.emplace_back(Button(sf::IntRect(bounds.width / 2 + 120, 145, 150, 40), "Toggle Rule", 3, font));
	_buttonList.emplace_back(Button(sf::IntRect(bounds.width / 2 + 120, 195, 150, 40), "Toggle Rule", 4, font));
	_buttonList.emplace_back(Button(sf::IntRect(bounds.width / 2 + 120, 285, 150, 40), "Toggle Rule", 5, font));
	_buttonList.emplace_back(Button(sf::IntRect(bounds.width / 2 + 120, 335, 150, 40), "Toggle Rule", 6, font));
	_buttonList.emplace_back(Button(sf::IntRect(bounds.width / 2 + 120, 385, 150, 40), "Toggle Rule", 7, font));
	_buttonList.emplace_back(Button(sf::IntRect(bounds.width / 2 + 120, 435, 150, 40), "Toggle Rule", 8, font));
	_buttonList.emplace_back(Button(sf::IntRect(bounds.width / 2 + 120, 485, 150, 40), "Cycle Limit", 9, font));
	_buttonList.emplace_back(Button(sf::IntRect(bounds.width * 3 / 4 - 100, 535, 200, 40), "Reset To Default", 10, font));
}

void LobbyInterface::initialiseBackground(const sf::IntRect & bounds, const sf::Font & font)
{
	_background = new DrawableObjectGroup();
	DrawableShape* leftRegion = new DrawableShape(new sf::RectangleShape(sf::Vector2f(bounds.width / 2 + 20 - 2, 500 - 2)), sf::Color(205, 138, 78, 128), sf::Vector2f(10+1, 80 +1));
	leftRegion->setBorder(1, sf::Color::Black);
	_background->addChild(leftRegion);
	DrawableShape* rightRegion = new DrawableShape(new sf::RectangleShape(sf::Vector2f(bounds.width / 2 - 60 - 2, 500 - 2)), sf::Color(205, 138, 78, 128), sf::Vector2f(bounds.width / 2 + 40 + 1, 80 + 1));
	rightRegion->setBorder(1, sf::Color::Black);
	_background->addChild(rightRegion);

	DrawableText* rulesTitle = new DrawableText(sf::Vector2f(0, 0), "Rules", font, 30, sf::Color::Black, sf::Text::Bold);
	rulesTitle->setOffset(sf::Vector2f(bounds.width / 2 + 280, 120 - 30));
	_background->addChild(rulesTitle);

	// UNO! Text with shadow
	DrawableText* unoShadow = new DrawableText(sf::Vector2f(0, 0), "UNO!", font, 40, sf::Color::Black, sf::Text::Bold);
	unoShadow->setOffset(sf::Vector2f(bounds.width / 2 - 40, 50 - 40));
	_background->addChild(unoShadow);
	DrawableText* uLetter = new DrawableText(sf::Vector2f(0, 0), "U", font, 40, Card::getColourByID(0), sf::Text::Bold);
	uLetter->setOffset(sf::Vector2f(bounds.width / 2 - 40 + 2, 48 - 40));
	_background->addChild(uLetter);
	DrawableText* nLetter = new DrawableText(sf::Vector2f(0, 0), "N", font, 40, Card::getColourByID(1), sf::Text::Bold);
	nLetter->setOffset(sf::Vector2f(bounds.width / 2 - 40 + 2 + 30, 48 - 40));
	_background->addChild(nLetter);
	DrawableText* oLetter = new DrawableText(sf::Vector2f(0, 0), "O", font, 40, Card::getColourByID(2), sf::Text::Bold);
	oLetter->setOffset(sf::Vector2f(bounds.width / 2 - 40 + 2 + 60, 48 - 40));
	_background->addChild(oLetter);
	DrawableText* exclamationMark = new DrawableText(sf::Vector2f(0, 0), "!", font, 40, Card::getColourByID(3), sf::Text::Bold);
	exclamationMark->setOffset(sf::Vector2f(bounds.width / 2 - 40 + 2 + 90, 48 - 40));
	_background->addChild(exclamationMark);

	// Credits
	DrawableText* credits = new DrawableText(sf::Vector2f(0, 0), "Developed by Peter Mitchell (2022)", font, 10, sf::Color::Black, sf::Text::Bold);
	credits->setOffset(sf::Vector2f(bounds.width / 2 - 85, 65 - 10));
	_background->addChild(credits);

	// Force everything to position itself correctly if it was not.
	_background->setPositionWithOffset(sf::Vector2f(bounds.left, bounds.top));
}
