#include "PostGameInterface.h"
#include "DrawableLine.h"

PostGameInterface::PostGameInterface(const sf::IntRect & bounds, const sf::Font & font, const std::vector<Player*>& playerList, RuleSet * ruleSet)
	: WndInterface(bounds), _playerList(playerList)
{
	initialiseInterface(font, ruleSet);
	_resultState = WndResultState::NothingState;
}

PostGameInterface::~PostGameInterface()
{
	delete _staticElements;
}

void PostGameInterface::draw(sf::RenderWindow & renderWindow) const
{
	_staticElements->draw(renderWindow);
	for (const auto& button : _buttonList) {
		button.draw(renderWindow);
	}
}

void PostGameInterface::handleMousePress(const sf::Vector2i & mousePosition, bool isLeft)
{
	if (!isEnabled()) return;
	for(const auto& button : _buttonList) {
		if (button.isPositionInside(mousePosition))
			handleButtonPress(button.getActionID());
	}
}

void PostGameInterface::handleMouseMove(const sf::Vector2i & mousePosition)
{
	if (!isEnabled()) return;
	for (auto& button : _buttonList) {
		button.setHovering(button.isPositionInside(mousePosition));
	}
}

WndResultState PostGameInterface::getResultState() const
{
	return _resultState;
}

void PostGameInterface::handleButtonPress(const int actionID)
{
	switch(actionID) {
		case 1: _resultState = WndResultState::Menu; break;
		case 3: 
			for (auto player : _playerList) { player->resetScore(); } // flow through to next case
		case 2: _resultState = WndResultState::Finished; break; 
	}
}

void PostGameInterface::initialiseInterface(const sf::Font & font, RuleSet * ruleSet)
{
	_staticElements = new DrawableObjectGroup();
	DrawableShape* background = new DrawableShape(new sf::RectangleShape(sf::Vector2f(_bounds.width / 2 - 2, 500 - 2)), sf::Color(205, 138, 78, 128), sf::Vector2f(_bounds.width / 4 + 1, 80 + 1));
	background->setBorder(1, sf::Color::Black);
	_staticElements->addChild(background);

	// UNO! Text with shadow
	DrawableText* unoShadow = new DrawableText(sf::Vector2f(0, 0), "UNO!", font, 40, sf::Color::Black, sf::Text::Bold);
	unoShadow->setOffset(sf::Vector2f(_bounds.width / 2 - 40, 10));
	_staticElements->addChild(unoShadow);
	DrawableText* uLetter = new DrawableText(sf::Vector2f(0, 0), "U", font, 40, Card::getColourByID(0), sf::Text::Bold);
	uLetter->setOffset(sf::Vector2f(_bounds.width / 2 - 40 + 2, 8));
	_staticElements->addChild(uLetter);
	DrawableText* nLetter = new DrawableText(sf::Vector2f(0, 0), "N", font, 40, Card::getColourByID(1), sf::Text::Bold);
	nLetter->setOffset(sf::Vector2f(_bounds.width / 2 - 40 + 2 + 30, 8));
	_staticElements->addChild(nLetter);
	DrawableText* oLetter = new DrawableText(sf::Vector2f(0, 0), "O", font, 40, Card::getColourByID(2), sf::Text::Bold);
	oLetter->setOffset(sf::Vector2f(_bounds.width / 2 - 40 + 2 + 60, 8));
	_staticElements->addChild(oLetter);
	DrawableText* exclamationMark = new DrawableText(sf::Vector2f(0, 0), "!", font, 40, Card::getColourByID(3), sf::Text::Bold);
	exclamationMark->setOffset(sf::Vector2f(_bounds.width / 2 - 40 + 2 + 90, 8));
	_staticElements->addChild(exclamationMark);

	DrawableText* titleText = new DrawableText(sf::Vector2f(0, 0), "Post-Game Summary", font, 40, sf::Color::Black, sf::Text::Bold);
	titleText->setOffset(sf::Vector2f(_bounds.width / 2 - titleText->getTextWidth() / 2, 80));
	_staticElements->addChild(titleText);

	DrawableLine* topLine = new DrawableLine({ sf::Vector2f(_bounds.width / 4 + 10, 200), sf::Vector2f(_bounds.width * 3 / 4 - 10, 200) });
	_staticElements->addChild(topLine);

	DrawableText* roundScoreTitle = new DrawableText(sf::Vector2f(0, 0), "Round Score", font, 20, sf::Color::Black, sf::Text::Bold);
	roundScoreTitle->setOffset(sf::Vector2f(_bounds.width / 2-20, 160));
	_staticElements->addChild(roundScoreTitle);
	DrawableText* totalScoreTitle = new DrawableText(sf::Vector2f(0, 0), "Total Score", font, 20, sf::Color::Black, sf::Text::Bold);
	totalScoreTitle->setOffset(sf::Vector2f(_bounds.width / 2 + 165, 160));
	_staticElements->addChild(totalScoreTitle);

	Player* winner = _playerList.at(0);
	for (int i = 0; i < _playerList.size(); i++) {
		if (_playerList.at(i)->getWon()) {
			winner = _playerList.at(i);
		}

		DrawableLine* dividerLine = new DrawableLine({ sf::Vector2f(_bounds.width / 4 + 10, 260 + i * 60), sf::Vector2f(_bounds.width * 3 / 4 - 10, 260 + i * 60) });
		_staticElements->addChild(dividerLine);
		std::string playerString = (_playerList.at(i)->getPlayerType() == Player::PlayerType::ThisPlayer ? "You: " : "AI: ") + _playerList.at(i)->getPlayerName();
		DrawableText* playerStringText = new DrawableText(sf::Vector2f(0, 0), playerString, font, 20, sf::Color::Black, sf::Text::Bold);
		playerStringText->setOffset(sf::Vector2f(_bounds.width / 4 + 50, 220 + i * 60));
		_staticElements->addChild(playerStringText);
		DrawableText* currentRoundScore = new DrawableText(sf::Vector2f(0, 0), std::to_string(_playerList.at(i)->getCurrentRoundScore()), font, 20, sf::Color::Black, sf::Text::Bold);
		currentRoundScore->setOffset(sf::Vector2f(_bounds.width / 2, 220 + i * 60));
		_staticElements->addChild(currentRoundScore);
		DrawableText* totalScore = new DrawableText(sf::Vector2f(0, 0), std::to_string(_playerList.at(i)->getTotalScore()), font, 20, sf::Color::Black, sf::Text::Bold);
		totalScore->setOffset(sf::Vector2f(_bounds.width / 2 + 180, 220 + i * 60));
		_staticElements->addChild(totalScore);
	}

	DrawableLine* line1 = new DrawableLine({ sf::Vector2f(_bounds.width / 4 + 10, 200), sf::Vector2f(_bounds.width / 4 + 10, 200 + _playerList.size() * 60) });
	_staticElements->addChild(line1);
	DrawableLine* line2 = new DrawableLine({ sf::Vector2f(_bounds.width / 2 - 40, 150), sf::Vector2f(_bounds.width / 2 - 40, 200 + _playerList.size() * 60) });
	_staticElements->addChild(line2);
	DrawableLine* line3 = new DrawableLine({ sf::Vector2f(_bounds.width / 2 + 130, 150), sf::Vector2f(_bounds.width / 2 + 130, 200 + _playerList.size() * 60) });
	_staticElements->addChild(line3);
	DrawableLine* line4 = new DrawableLine({ sf::Vector2f(_bounds.width * 3 / 4 - 10, 150), sf::Vector2f(_bounds.width * 3 / 4 - 10, 200 + _playerList.size() * 60) });
	_staticElements->addChild(line4);
	DrawableLine* line5 = new DrawableLine({ sf::Vector2f(_bounds.width / 2 - 40, 150), sf::Vector2f(_bounds.width * 3 / 4 - 10, 150) });
	_staticElements->addChild(line5);

	std::string scoreLimitStr;
	bool scoreLimitReached;
	switch (ruleSet->getScoreLimitType()) {
		case RuleSet::ScoreLimitType::OneRound: 
			scoreLimitReached = true;
			scoreLimitStr = "Score Limit: One Round"; 
			break;
		case RuleSet::ScoreLimitType::Score200: 
			scoreLimitReached = winner->getTotalScore() >= 200;
			scoreLimitStr = "Score Limit: 200 Points"; 
			break;
		case RuleSet::ScoreLimitType::Score300: 
			scoreLimitReached = winner->getTotalScore() >= 300;
			scoreLimitStr = "Score Limit: 300 Points"; 
			break;
		case RuleSet::ScoreLimitType::Score500:
			scoreLimitReached = winner->getTotalScore() >= 500;
			scoreLimitStr = "Score Limit: 500 Points"; 
			break;
		case RuleSet::ScoreLimitType::Unlimited: 
			scoreLimitReached = false;
			scoreLimitStr = "Score Limit: Unlimited"; break;
	}

	DrawableText* winnerTitle = new DrawableText(sf::Vector2f(0, 0), "Round Winner: ", font, 20, sf::Color::Black, sf::Text::Bold);
	winnerTitle->setOffset(sf::Vector2f(_bounds.width / 4 + 25, 470));
	_staticElements->addChild(winnerTitle);
	DrawableText* winnerLabel = new DrawableText(sf::Vector2f(0, 0), winner->getPlayerName(), font, 20, sf::Color::Black, sf::Text::Bold);
	winnerLabel->setOffset(sf::Vector2f(_bounds.width / 4 + 175, 470));
	_staticElements->addChild(winnerLabel);
	DrawableText* scoreLimitText = new DrawableText(sf::Vector2f(0, 0), scoreLimitStr, font, 20, sf::Color::Black, sf::Text::Bold);
	scoreLimitText->setOffset(sf::Vector2f(_bounds.width / 2 + 20, 470));
	_staticElements->addChild(scoreLimitText);

	if (scoreLimitReached) {
		DrawableText* scoreLimitReachedText = new DrawableText(sf::Vector2f(0, 0), "Score limit reached!", font, 20, sf::Color::Black, sf::Text::Bold);
		scoreLimitReachedText->setOffset(sf::Vector2f(_bounds.width / 2 + 40, 510));
		_staticElements->addChild(scoreLimitReachedText);

		_buttonList.emplace_back(Button(sf::IntRect(_bounds.width / 2 - 125 - 250 - 20, 620, 250, 40), "Return to Lobby", 1, font));
		_buttonList.emplace_back(Button(sf::IntRect(_bounds.width / 2 - 125, 620, 250, 40), "Continue Next Round", 2, font));
		_buttonList.emplace_back(Button(sf::IntRect(_bounds.width / 2 + 125 + 20, 620, 270, 40), "New Game Same Settings", 3, font));
	}
	else {
		_buttonList.emplace_back(Button(sf::IntRect(_bounds.width / 2 - 250 - 10, 620, 250, 40), "Return to Lobby", 1, font));
		_buttonList.emplace_back(Button(sf::IntRect(_bounds.width / 2 + 10, 620, 270, 40), "New Game Same Settings", 3, font));
	}

	_staticElements->setPositionWithOffset(sf::Vector2f(0, 0));
}
