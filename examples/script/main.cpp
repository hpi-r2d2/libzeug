#include <iostream>
#include <scriptzeug/ScriptEnvironment.h>
#include <scriptzeug/Scriptable.h>


using namespace scriptzeug;


void testFunction()
{
    std::cout << "Test function\n";
}


class MyInterface : public Scriptable
{
public:
    MyInterface()
    {
        addFunction("test", &testFunction);
        addFunction("helloWorld",    this, &MyInterface::helloWorld);
        addFunction("bottlseOfBeer", this, &MyInterface::bottlesOfBeer);
    }

    ~MyInterface()
    {
    }

    void helloWorld()
    {
        std::cout << "Hello World\n";
    }

    void bottlesOfBeer(int count)
    {
        std::cout << count << " bottles of beer.\n";
    }
};


int main(int argc, char const *argv[])
{
    MyInterface obj;

    ScriptEnvironment scripting;
    scripting.registerObject("testobj", &obj);
//  scripting.evaluate("testobj.test(1, 2, '3', 23.42, 'asd');");
    scripting.evaluate("testobj.helloWorld();");

    return 0;
}
