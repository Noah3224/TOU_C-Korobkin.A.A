#include <iostream>
#include <vector>
void printField(const std::vector<char> &field)
{
    std::cout << " " << field[0] << " | " << field[1] << " | " << field[2] << " \n";
    std::cout << "---+---+---\n";
    std::cout << " " << field[3] << " | " << field[4] << " | " << field[5] << " \n";
    std::cout << "---+---+---\n";
    std::cout << " " << field[6] << " | " << field[7] << " | " << field[8] << " \n";
}

bool checkWin(const std::vector<char> &field, char player)
{
    const int winCombos[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Строки
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8}, // Столбцы
        {0, 4, 8},
        {2, 4, 6} // Диагонали
    };

    for (const auto &combo : winCombos)
    {
        if (field[combo[0]] == player && field[combo[1]] == player && field[combo[2]] == player)
        {
            return true;
        }
    }
    return false;
}

int main()
{
    std::vector<char> field = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char currentPlayer = 'X';
    int moves = 0;
    bool gameOver = false;

    while (!gameOver)
    {
        printField(field);
        int move;
        std::cout << "Игрок " << currentPlayer << ", выберите клетку (1-9): ";
        std::cin >> move;

        if (std::cin.fail() || move < 1 || move > 9 || field[move - 1] == 'X' || field[move - 1] == 'O')
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Некорректный ввод! Попробуйте снова.\n";
            continue;
        }

        field[move - 1] = currentPlayer;
        moves++;

        if (checkWin(field, currentPlayer))
        {
            printField(field);
            std::cout << "Игрок " << currentPlayer << " победил!\n";
            gameOver = true;
        }
        else if (moves == 9)
        {
            printField(field);
            std::cout << "Ничья!\n";
            gameOver = true;
        }
        else
        {
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }

    return 0;
}
