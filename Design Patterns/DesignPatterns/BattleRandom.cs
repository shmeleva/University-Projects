using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    static class BattleRandom
    {
        private readonly static Random _battleRandom = new Random();

        public static Double NextProbability()
        {
            return _battleRandom.Next(101) / 100;
        }

        public static Double NextDouble()
        {
            return _battleRandom.NextDouble();
        }

        public static Boolean NextBoolean()
        {
            return _battleRandom.NextDouble() < 0.5;
        }

        public static Int32 Next(Int32 maxValue)
        {
            return _battleRandom.Next(maxValue);
        }
    }
}
