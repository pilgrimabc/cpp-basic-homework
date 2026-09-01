#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>

int check_value (int max_value = 100) {
    int attempts_count = 0; // num of attempts

    std::srand(std::time(nullptr)); // using current time as a seed
    int random_number = std::rand() % 100; // generate random number

    std::cout << "Enter your guess" << std::endl;

    do {
        int number = 0;
        std::cin >> number;
        attempts_count++;

        if (number < random_number) {
            std::cout << "less than guessed number" << std::endl;
        }
        else if (number > random_number) {
            std::cout << "grater than guessed number" << std::endl;
        }
        else {
            std::cout << "you win! attempts = " << attempts_count << std::endl;
            break;
        }
    } while (true);

    return attempts_count;
}

int write_high_scores(std::string& user_name, int& attempts, const std::string& filename) {

    std::ofstream out_file{filename, std::ios_base::app};

    if (!out_file.is_open()) {
			std::cout << "Failed to open file for write: " << filename << "!" << std::endl;
			return -1;
	}

    out_file << user_name << ' ';
    out_file << attempts;
    out_file << std::endl; 
    
    return 0;
}

int read_high_scores(const std::string& filename) {

    std::ifstream in_file{filename};
    if (!in_file.is_open()) {
			std::cout << "Failed to open file for read: " << filename << "!" << std::endl;
			return -1;
	}

    std::cout << "High scores table:" << std::endl;

    std::string user; 
    int high_score = 0;

    while (true) {
        // Read the username first
		in_file >> user;
		// Read the high score next
		in_file >> high_score;
		// Ignore the end of line symbol
		in_file.ignore();

		if (in_file.fail()) {
			break;
		}

		// Print the information to the screen
		std::cout << user << '\t' << high_score << std::endl;
    }

    return 0;
}

int main(int argc, char** argv) {

    const std::string high_scores_filename = "high_scores.txt";

    if (argc >=2) {
        std::string arg1_value = argv[1];
        
        if (arg1_value == "-max") {
            
            if (argc < 3) {
                std::cout << "Wrong usage! The argument '-max' requires some value!" << std::endl;
                return -1;
            }
            int max_value = std::stoi(argv[2]);

            std::cout << "Max value is " << max_value << std::endl;

            // Enter user_name 
            std::cout << "Hi! Enter your name, please." << std::endl;
            std::string user_name;
            std::cin >> user_name;

            int attempts = check_value(max_value);

            //std::cout << "bla-bla" << std::endl;
            //std::cout << "total attempts = " << attempts << std::endl;

            //write result
            write_high_scores(user_name, attempts, high_scores_filename);
            read_high_scores(high_scores_filename);

        }
        else if (arg1_value == "-table") {
            read_high_scores(high_scores_filename);
        }
    }
    else {
        // Enter user_name 
        std::cout << "Hi! Enter your name, please." << std::endl;
        std::string user_name;
        std::cin >> user_name;

        int attempts = check_value();

        //std::cout << "bla-bla" << std::endl;
        //std::cout << "total attempts = " << attempts << std::endl;

        //write result
        write_high_scores(user_name, attempts, high_scores_filename);
        read_high_scores(high_scores_filename);

    }

    return 0;
}