#include "action.hh"

MyActionInitialization::MyActionInitialization(/* args */)
{}

MyActionInitialization::~MyActionInitialization()
{}

void MyActionInitialization::Build() const 
{
    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);
}