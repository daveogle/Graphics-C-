using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace erlangTests
{
    class Program
    {

    static double pi(double C, double N)
    {
        if (C == 999)
            return N;

        if (C % 2 == 0)
        {
             return pi((C + 1), (N + (1 / (1 + 2 * C))));
        }
        else
        {
            return pi((C + 1), (N - (1 / (1 + 2 * C))));
        }
    }

    static void Main(string[] args)
        {
            double myPi = pi(0.0, 0.0);
            Console.WriteLine(4 * myPi);
            Console.Read();
        }
    }
}
