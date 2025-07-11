#ifndef CALCULATOR_SERVICE_H
#define CALCULATOR_SERVICE_H

class CalculatorService {
public:
    virtual double add(double a, double b) = 0;
    virtual double subtract(double a, double b) = 0;
     virtual double multiply(double a, double b) = 0;
    virtual double divide(double a, double b) = 0;
    virtual ~CalculatorService() {}
};

#endif // CALCULATOR_SERVICE_H
