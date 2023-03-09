#include "stdafx.h"
const std::string kFileName = "data.txt";

struct Player {
	std::string name;
	std::string team;
	unsigned int goals_scored{};
	unsigned int num_yellow_cards{};
	unsigned int num_red_cards{};
};

struct SMenuOptions {
	int index;
	std::string text;
};

const SMenuOptions MenuOptionList[]{
	{1, "Display all players"},
	{2, "Search for player"},
	{3, "Add new player"},
	{0, "Exit program"}
};

void ClearInputBuffer();
void MenuHeader();
void MenuOptions();
int HandleMenuOption();
void DisplayScoreboard();
void SearchPlayerName();
std::vector<Player> GetPlayers();
void EmplacePlayer(const std::vector<Player>&);
Player CreateReplacementPlayer(const std::string& name);
void AddPlayer();
void isValidPositiveNumber(int& input);

int main()
{
	MenuHeader();
	MenuOptions();
}

void ClearInputBuffer()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void isValidPositiveNumber(int& input)
{
	while (!(std::cin >> input) || input < 0)
	{
		std::cin.clear();
		ClearInputBuffer();
	}
}

void MenuHeader()
{
	std::cout << "-------------------------------------" << '\n';
	std::cout << "\tFootball Tracker" << '\n';
	std::cout << "-------------------------------------" << '\n';
}

void MenuOptions()
{

	for (auto& option : MenuOptionList)
	{
		std::cout << option.index << ". " << option.text << '\n';
	}
	HandleMenuOption();
}

int HandleMenuOption()
{
	std::cout << "Choose an option: ";
	int option = -1;
	while (!(std::cin >> option) || (option < 0 || option > 3))
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Invalid input, please select an option from the menu." << '\n';
	}
	system("cls");
	switch (option)
	{
	case 1:
		DisplayScoreboard();

		break;
	case 2:
		SearchPlayerName();
		break;
	case 3:
		AddPlayer();
		break;
	case 0:
		std::cout << "Exiting program..." << '\n';
		return 0;
		break;
	default:
		std::cout << "Invalid selection. Please enter a number listed from the menu" << '\n';
		break;
		HandleMenuOption();
	}
}

void DisplayScoreboard()
{
	std::ifstream data{ kFileName };
	if (!data.is_open())
	{
		std::cout << "Error opening file " << kFileName << '\n';
		MenuOptions();
	}
	std::string line;
	while (std::getline(data, line))
	{
		std::stringstream lineStream(line);
		std::string cell;
		while (std::getline(lineStream, cell, ','))
		{
			std::cout << std::setw(20) << std::left << cell << '\t';
		}
		std::cout << '\n';
	}
	data.close();
	MenuOptions();
}

void SearchPlayerName()
{
	std::vector<Player> players = GetPlayers();
	std::string searchName;
	std::cout << "Enter a name to search for: ";
	ClearInputBuffer();
	std::getline(std::cin, searchName);
	for (auto& player : players) {
		if (player.name == searchName)
		{
			Player replacementPlayer = CreateReplacementPlayer(searchName);
			player = replacementPlayer;
			break;
		}
	}
	EmplacePlayer(players);
	MenuOptions();
}

Player CreateReplacementPlayer(const std::string& name) {
	Player replacementPlayer;

	std::cout << "Enter new team for " << name << ": ";
	std::getline(std::cin, replacementPlayer.team);
	std::cout << "Enter new goals scored for " << name << ": ";
	std::cin >> replacementPlayer.goals_scored;
	std::cout << "Enter new number of yellow cards for " << name << ": ";
	std::cin >> replacementPlayer.num_yellow_cards;
	std::cout << "Enter new number of red cards for " << name << ": ";
	std::cin >> replacementPlayer.num_red_cards;

	replacementPlayer.name = name;

	return replacementPlayer;
}

std::vector<Player> GetPlayers()
{
	std::vector<Player> players;
	std::ifstream file(kFileName);

	if (file) {
		std::string line;
		std::getline(file, line); // Skip the header row

		while (std::getline(file, line)) {
			std::stringstream ss(line);
			std::string field;
			Player player;

			std::getline(ss, player.name, ',');
			std::getline(ss, player.team, ',');
			std::getline(ss, field, ',');
			player.goals_scored = std::stoi(field);
			std::getline(ss, field, ',');
			player.num_yellow_cards = std::stoi(field);
			std::getline(ss, field);
			player.num_red_cards = std::stoi(field);

			players.push_back(player);
		}
	}
	return players;
}

void EmplacePlayer(const std::vector<Player>& players)
{
	std::ofstream file(kFileName);

	if (file) {
		file << "Name,Team,Goals Scored,Number of Yellow Cards,Number of Red Cards\n";

		for (const auto& player : players) {
			file << player.name << ","
				<< player.team << ","
				<< player.goals_scored << ","
				<< player.num_yellow_cards << ","
				<< player.num_red_cards << "\n";
		}
	}
	else
	{
		std::cout << "Failed";
		system("pause");
	}
	file.close();
}

/* int SearchForPlayer()
{
	std::cout << "Searching for player..." << '\n';
	std::ifstream data{ kDataFile };
	if (!data.is_open())
	{
		std::cout << "Failed to open file " << kDataFile << '\n';
		return 1;
	}

	std::vector<std::array<std::string, 5>> records;

	std::string line;
	while (std::getline(data, line)) {
		std::stringstream lineStream(line);
		std::array<std::string, 5> record{};
		std::string cell;
		std::size_t i{};
		while (std::getline(lineStream, cell, ',')) {
			record[i] = cell;
			i++;
		}
		records.push_back(record);


	}

	data.close();
	MenuOptions();
}
*/

void AddPlayer()
{
	std::vector<Player> players = GetPlayers();
	Player newPlayer;
	std::cout << "Enter player's name: ";
	ClearInputBuffer();
	std::getline(std::cin, newPlayer.name);

	std::cout << "Enter player's team: ";
	std::getline(std::cin, newPlayer.team);

	std::cout << "Enter number of goals scored: ";
	std::cin >> newPlayer.goals_scored;
	std::cout << "Enter number of yellow cards: ";
	std::cin >> newPlayer.num_yellow_cards;

	std::cout << "Enter number of red cards: ";
	std::cin >> newPlayer.num_red_cards;

	for (auto it = players.begin(); it != players.end(); it++) {
		if (it->name == newPlayer.name && it->team == newPlayer.team) {
			std::cout << "Player already exists. Do you want to replace them? (y/n): ";
			char answer;
			std::cin >> answer;
			answer = tolower(answer);
			if (answer == 'y') {
				players.erase(it);
			}
			else {
				MenuOptions();
			}
			break;
		}
	}
	players.push_back(newPlayer);

	EmplacePlayer(players);

	MenuOptions();
}