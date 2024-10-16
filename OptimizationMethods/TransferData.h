#pragma once
#include "config.h"


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

    void print_vec(VectorXd& vec) {
        for (int i = 0; i < vec.size(); ++i)
            cout << vec[i] << " ";
        cout << endl;
    }

    void printData() {
        cout << endl;
        
        cout << "Curr_point: "; print_vec(*curr_point);
        cout << "Prev_point: "; print_vec(*prev_point);
        cout << "Grad: "; print_vec(*curr_gradient);
        cout << "Iter: " << *curr_iter << endl;
        cout << "Curr F: " << *curr_f_val << endl;
        cout << "Prev F: " << *prev_f_val << endl;
        cout << "Alpha: " << *alpha << endl;

        cout << endl;
    }
};
