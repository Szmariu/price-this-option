// Parameters.h

#ifndef PARAMETERS_H
#define PARAMETERS_H

//This is a base class.
//All its methods are abstract, so this is why we refer to it as an abstract class.
class ParametersInner {
public:
    ParametersInner(){}
    virtual ParametersInner* clone() const=0;
    virtual double Integral(double time1, double time2) const=0;
    virtual double IntegralSquare(double time1, double time2) const=0;

private:

};


//This is a BRIDGING CLASS!
//This class handles the interaction without the outside world,
//and the memory handling. Its only data member is a pointer to an abstract class (ParametersInner).
class Parameters {

public:
    Parameters(const ParametersInner& innerObject);
    Parameters(const Parameters& original);
    Parameters& operator=(const Parameters& original);
    virtual ~Parameters();

    inline double Integral(double time1, double time2) const;
    inline double IntegralSquare(double time1, double time2) const;

    double Mean(double time1, double time2) const;
    double RootMeanSquare(double time1, double time2) const;

private:
    ParametersInner* InnerObjectPtr;

};


inline double Parameters::Integral(double time1, double time2) const {
    return InnerObjectPtr->Integral(time1,time2);
}


inline double Parameters::IntegralSquare(double time1, double time2) const {
    return InnerObjectPtr->IntegralSquare(time1,time2);
}


// This is a subclass of the ParametersInner base class.
// Tt is designed for handling option parameters, that are constant in time.
class ParametersConstant : public ParametersInner {

public:
    ParametersConstant(double constant);

    virtual ParametersInner* clone() const;
    virtual double Integral(double time1, double time2) const;
    virtual double IntegralSquare(double time1, double time2) const;


private:
    double Constant;
    double ConstantSquare;

};

#endif
