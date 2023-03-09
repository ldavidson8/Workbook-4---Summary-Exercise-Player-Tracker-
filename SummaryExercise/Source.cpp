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
	{3, "Search for team"},
	{4, "Add new player"},
	{0, "Exit program"}
};


// Utility functions
void ClearInputBuffer();
void getPositiveIntegerInput(uint32_t& input);
std::string toLower(std::string str);

// Primary functions
void MenuHeader();
void MenuOptions();
int HandleMenuOption();
void DisplayScoreboard();
void SearchPlayerName();
void SearchForTeam(const std::string& teamName);
std::vector<Player> GetPlayers();
Player CreateReplacementPlayer(const std::string& name);
void EmplacePlayer(const std::vector<Player>& players);
void AddPlayer();

int main()
{
	while (true) {
		MenuHeader();
		MenuOptions();
		int result = HandleMenuOption();
		if (result == 0)
			break;
	}
	return 0;
}

void ClearInputBuffer()
{
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void getPositiveIntegerInput(uint32_t& input)
{
	int signedInput;
	while (!(std::cin >> signedInput) || signedInput < 0)
	{
		ClearInputBuffer();
		std::cout << "Please enter a number higher or equal to 0" << '\n';
	}
	input = static_cast<uint32_t>(signedInput);
}

std::string toLower(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
		return std::tolower(c);
		});
	return str;
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
}

int HandleMenuOption()
{
	std::cout << "Choose an option: ";
	int option = -1;
	while (!(std::cin >> option) || (option < 0 || option > std::size(MenuOptionList)))
	{
		ClearInputBuffer();
		std::cout << "Invalid input, please select an option from the menu." << '\n';
	}
	system("cls");
	switch (option)
	{
	case 1:
	{
		DisplayScoreboard();
		break;
	}
	case 2:
	{
		SearchPlayerName();
		break;
	}
	case 3: 
	{
		std::string teamName;
		std::cout << "Enter team name to search: ";
		std::cin >> teamName;
		SearchForTeam(teamName);
		break;
	}
	case 4:
	{
		AddPlayer();
		break;
	}
	case 0: 
	{
		std::cout << "Exiting program..." << '\n';
		return 0;
		break;
	}
	default:
	{
		std::cout << "Invalid selection. Please enter a number listed from the menu" << '\n';
		break;
	}
	}
}

void DisplayScoreboard()
{
	std::vector<Player> players = GetPlayers();
	std::sort(players.begin(), players.end(), [](const Player& a, const Player& b) {
		return a.name < b.name;
		});

	std::cout << std::setw(20) << std::left << "Name"
		<< std::setw(20) << std::left << "Team"
		<< std::setw(20) << std::left << "Goals Scored"
		<< std::setw(20) << std::left << "Yellow Cards"
		<< std::setw(20) << std::left << "Red Cards" << '\n';

	for (const auto& player : players) {
		std::cout << std::setw(20) << std::left << player.name
			<< std::setw(20) << std::left << player.team
			<< std::setw(20) << std::left << player.goals_scored
			<< std::setw(20) << std::left << player.num_yellow_cards
			<< std::setw(20) << std::left << player.num_red_cards << '\n';
	}
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
}

void SearchForTeam(const std::string& teamName) {
	std::vector<Player> players = GetPlayers();
	bool foundTeam = false;

	std::string teamNameLower = toLower(teamName);

	for (const auto& player : players) {
		std::string playerTeamLower = toLower(player.team);
		if (playerTeamLower == teamNameLower) {
			if (!foundTeam) {
				std::cout << "Players under " << teamName << " team:\n";
				std::cout << std::setw(20) << std::left << "Name" 
					<< std::setw(20) << std::left << "Team" 
					<< std::setw(20) << std::left << "Goals Scored" 
					<< std::setw(20) << std::left << "Yellow Cards" 
					<< std::setw(20) << std::left << "Red Cards\n";
				foundTeam = true;
			}

			std::cout << std::setw(20) << std::left << player.name
				<< std::setw(20) << std::left << player.team
				<< std::setw(20) << std::left << player.goals_scored
				<< std::setw(20) << std::left << player.num_yellow_cards
				<< std::setw(20) << std::left << player.num_red_cards << '\n';
		}
	}

	if (!foundTeam) {
		std::cout << "No players found under " << teamName << " team\n";
	}
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

Player CreateReplacementPlayer(const std::string& name) {
	Player replacementPlayer;

	replacementPlayer.name = name;

	std::cout << "Enter new team for " << name << ": ";
	std::getline(std::cin, replacementPlayer.team);

	std::cout << "Enter new goals scored for " << name << ": ";
	getPositiveIntegerInput(replacementPlayer.goals_scored);

	std::cout << "Enter new number of yellow cards for " << name << ": ";
	getPositiveIntegerInput(replacementPlayer.num_yellow_cards);

	std::cout << "Enter new number of red cards for " << name << ": ";
	getPositiveIntegerInput(replacementPlayer.num_red_cards);

	return replacementPlayer;
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
	getPositiveIntegerInput(newPlayer.goals_scored);

	std::cout << "Enter number of yellow cards: ";
	getPositiveIntegerInput(newPlayer.num_yellow_cards);

	std::cout << "Enter number of red cards: ";
	getPositiveIntegerInput(newPlayer.num_red_cards);

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
}