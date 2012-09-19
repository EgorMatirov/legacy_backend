#include <stdexcept>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <string>
#include <sstream>

#include <cassert>

enum error_code
{
    OK=0,
    WA=2,
    PE=4,
    UN=100
};

error_code check_LN(std::istream &in)
{
    char c;
    while (in.get(c))
    {
        if (c!='\n' && c!='\r')
            return WA;
    }
    return OK;
}

error_code check_equal(std::istream &out, std::istream &cout)
{
    // read all symbols and compare
    char o, c;
    do
    {
        while (out.get(o) && o=='\r');
        while (cout.get(c) && c=='\r');
        if (out && cout && o!=c)
            return WA;
    }
    while (out && cout);
    if (out.eof() && cout.eof())
        return OK;
    else
    {
        if (out.eof() && cout)
        {
            cout.putback(c);
            return check_LN(cout);
        }
        else if (out && cout.eof())
        {
            out.putback(o);
            return check_LN(out);
        }
        else
            return WA;
    }
}

error_code test_equal(const std::string &out, const std::string &cout)
{
    std::stringstream out_(out), cout_(cout);
    return check_equal(out_, cout_);
}

bool run_tests()
{
    using namespace std;
    // check_LN
    stringstream ss("\n\n");
    assert(check_LN(ss)==OK);
    ss.clear();
    ss.str("");
    assert(check_LN(ss)==OK);
    // check_equal
    assert(test_equal("", "")==OK);
    assert(test_equal("\n\n", "\n")==OK);
    assert(test_equal("n\n", "n")==OK);
    assert(test_equal("n", "n\n\n")==OK);
    assert(test_equal("n\nn", "n\nn\n")==OK);
    assert(test_equal("123", "1234")==WA);
    assert(test_equal("123\n", "1234")==WA);
    assert(test_equal("123\n", "123")==OK);
    // \r
    assert(test_equal("\r\n", "\r")==OK);
    assert(test_equal("123", "\r123\r\n\n")==OK);
    assert(test_equal("\r\r123\n\n\n\n\r\n", "123\n\n")==OK);
    assert(test_equal("123\n", "123\r\n")==OK);
    assert(test_equal("1\r2\r3", "123\n")==OK);
    assert(test_equal("123\r\n321", "123\n321")==OK);
    return true;
}

int main(int argc, char **argv)
{
    assert(run_tests());
    try
    {
        if (argc!=3+1)
            throw std::invalid_argument("wrong argument count");
        // input file [it is useless, so we don't create it]
        /*std::ifstream in(argv[1]);
        if (!in.good())
            throw std::ios_base::failure("bad file: \""+std::string(argv[1])+"\"");
        in.exceptions(std::ifstream::failbit | std::ifstream::badbit);*/
        // output file
        std::ifstream out(argv[2]);
        if (!out.good())
            throw std::ios_base::failure("bad file: \""+std::string(argv[2])+"\"");
        //out.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        // correct output file
        std::ifstream cout(argv[3]);
        if (!cout.good())
            throw std::ios_base::failure("bad file: \""+std::string(argv[3])+"\"");
        //cout.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        return check_equal(out, cout);
    }
    catch (std::ios_base::failure &e)
    {
        std::cerr<<"File error: ["<<typeid(e).name()<<"] "<<e.what()<<std::endl;
        return UN;
    }
    catch (std::exception &e)
    {
        std::cerr<<"Error: ["<<typeid(e).name()<<"] "<<e.what()<<std::endl;
        std::cerr<<"Usage: checkdef <input file> <output file generated by solution> <correct output file>"<<std::endl<<std::endl;
        std::cerr<<"Return codes:"<<std::endl;
        std::cerr<<"\t"<<OK<<" - OK"<<std::endl
            <<"\t"<<WA<<" - Wrong Answer"<<std::endl
            <<"\t"<<PE<<" - Presentation Error"<<std::endl
            <<"\t"<<UN<<" - Invalid parameters or check error"<<std::endl;
        return UN;
    }
}

