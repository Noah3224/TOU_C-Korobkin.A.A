#include <iostream>
#include <vector>
#include <random>

void generateStory()
{
    std::vector<std::string> heroes = {"смелый рыцарь", "хитрый вор", "волшебник", "отважный пират", "дерзкий исследователь"};
    std::vector<std::string> places = {"в далёком королевстве", "на заброшенной фабрике", "в густом лесу", "на просторах космоса", "у подножия гор"};
    std::vector<std::string> actions = {"победил дракона", "обнаружил сокровища", "выиграл битву", "устроил бал", "раскрыл древнюю тайну"};
    std::vector<std::string> details = {"с волшебным мечом", "на летающем ковре", "под звуки волшебной музыки", "с удивительной силой", "в сопровождении магического существа"};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 4);

    std::string story = heroes[distrib(gen)] + " " + places[distrib(gen)] + ", " + actions[distrib(gen)] + " " + details[distrib(gen)] + ".";

    std::cout << "Сгенерированная история:\n" << story << "\n";
}

int main()
{
    char choice;
    do
    {
        generateStory();
        std::cout << "Хотите сгенерировать новую историю? (Y/N): ";
        std::cin >> choice;
    } while (choice == 'Y' || choice == 'y');

    return 0;
}
