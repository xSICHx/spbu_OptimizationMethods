#pragma once
#include "config.h"
#include <fstream>
#include <iostream>


/**
 * @class TransferData
 * @brief A class to manage and transfer optimization data points, gradients, and other related information.
 *
 * This class encapsulates various data points and parameters used in optimization methods, providing
 * getter and setter methods for each data member. It also includes utility functions to print the data.
 *
 */
class TransferData
{
private:
    std::shared_ptr<VectorXd> curr_point = nullptr;
    std::shared_ptr<VectorXd> prev_point = nullptr;
    std::shared_ptr<VectorXd> curr_gradient = nullptr;
    std::shared_ptr<size_t> curr_iter = nullptr;
    std::shared_ptr<double> curr_f_val = nullptr;
    std::shared_ptr<double> prev_f_val = nullptr;
    std::shared_ptr<double> alpha = nullptr;
    shared_ptr<bool> flag_in_bound = nullptr; // if overstepped = false

public:
    // getters
    shared_ptr<VectorXd> getCurrPoint() const {
        return curr_point;
    }

    shared_ptr<VectorXd> getPrevPoint() const {
        return prev_point;
    }

    shared_ptr<VectorXd> getCurrGradient() const {
        return curr_gradient;
    }

    shared_ptr<size_t> getCurrIter() const {
        return curr_iter;
    }

    shared_ptr<double> getCurrFVal() const {
        return curr_f_val;
    }

    shared_ptr<double> getPrevFVal() const {
        return prev_f_val;
    }

    shared_ptr<double> getAlpha() const {
        return alpha;
    }
    shared_ptr<bool> getFlagInBound() const {
        return flag_in_bound;
    }

    // setters
    void setCurrPoint(const VectorXd& point) {
        curr_point = std::make_shared<VectorXd>(point);
    }

    void setPrevPoint(const VectorXd& point) {
        prev_point = std::make_shared<VectorXd>(point);
    }

    void setCurrGradient(const VectorXd& gradient) {
        curr_gradient = std::make_shared<VectorXd>(gradient);
    }

    void setCurrIter(const size_t& iter) {
        curr_iter = std::make_shared<size_t>(iter);
    }

    void setCurrFVal(const double& val) {
        curr_f_val = std::make_shared<double>(val);
    }

    void setPrevFVal(const double& val) {
        prev_f_val = std::make_shared<double>(val);
    }

    void setAlpha(const double& alpha_val) {
        alpha = std::make_shared<double>(alpha_val);
    }
    void setFlagInBound(const bool& flag) {
        flag_in_bound = make_shared<bool>(flag);
    }

    void print_vec(VectorXd& vec) {
        for (int i = 0; i < vec.size(); ++i)
            cout << vec[i] << " ";
        cout << endl;
    }

    void printData() {
        cout << endl;
        
        cout << "Curr_point: "; print_vec(*curr_point);
        cout << "Prev_point: "; print_vec(*prev_point);
        if (curr_gradient != nullptr) {
            cout << "Grad: "; print_vec(*curr_gradient);
        }
        cout << "Iter: " << *curr_iter << endl;
        cout << "Curr F: " << *curr_f_val << endl;
        cout << "Prev F: " << *prev_f_val << endl;
        if (alpha != nullptr) {
            cout << "Alpha: " << *alpha << endl;
        }

        cout << endl;
    }

    void savePointToFile() {
        std::ofstream file("currOpt.txt", std::ios_base::app);
        // Проверяем, открылся ли файл
        if (file.is_open()) {
            file << (*curr_point)[0] << " " << (*curr_point)[1] << std::endl;

            // Закрываем файл
            file.close();
        }
    }

    void clearFile() {
        // Открываем файл в режиме записи, что удаляет его содержимое
        std::ofstream file("currOpt.txt", std::ios_base::trunc); // std::ios_base::trunc очищает файл

        // Проверяем, открылся ли файл
        if (file.is_open()) {
            file.close();
        }
    }


};
