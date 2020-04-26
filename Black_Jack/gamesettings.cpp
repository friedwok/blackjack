//
// Created by Павел Петров on 11/04/2020.
//
#include "gamesettings.h"
#include <fstream>
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include "commonhandler.h"
#include <utility>

GameSettings::GameSettings()
{
    std::ifstream config("config.txt");
    std::string str;
    std::vector<std::string> splited_str;
    //checking the configuration file, if the bit is 1, then this parameter is present
    std::pair<int, int> check_conf[3];
    for(int i = 0; i < 3; i++) {
        check_conf[i] = std::make_pair(1, 0);
    }

    if(!config) {
        std::cerr << "Config file not opened" << std::endl;
        exit(1);
    } else {
        while(std::getline(config, str)) {
            boost::split(splited_str, str, boost::is_any_of("="));
            if (splited_str[0] == "players_money_amount" and CommonHandler::is_number(splited_str[1])) {
                players_money_amount = std::stoi(splited_str[1]);
                check_conf[0].second = 1;
            }

            if (splited_str[0] == "minimum_bet" and CommonHandler::is_number(splited_str[1])) {
                min_bet = std::stoi(splited_str[1]);
                check_conf[1].second = 1;
            }

            if (splited_str[0] == "bet_range" and CommonHandler::is_number(splited_str[1])) {
                bet_range = std::stoi(splited_str[1]);
                check_conf[2].second = 1;
            }
        }
    }

    for(int i = 0; i < 3; i++) {
        if(!check_conf[i].second) {
            std::cerr << "Config file error, standard settings used:" << std::endl;
            players_money_amount = 100;
            min_bet = 5;
            bet_range = 5;
            break;
        }
    }
}

int GameSettings::GetPlayersMoney()
{
    return players_money_amount;
}

int GameSettings::GetMinimumBet()
{
    return min_bet;
}

int GameSettings::GetBetRange()
{
    return bet_range;
}