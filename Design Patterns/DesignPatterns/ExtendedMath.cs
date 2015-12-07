using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    static class ExtendedMath
    {
        /// <summary>
        /// Наименьшее общее кратное для массива значений.
        /// </summary>
        /// <param name="numbers">Непустой массив значений.</param>
        /// <returns>Наименьшее общее кратное.</returns>
        public static Int32 AggregatedLcm(Int32[] numbers)
        {
            return numbers.Aggregate(Lcm);
        }

        /// <summary>
        /// Наибольший общий делитель для массива значений.
        /// </summary>
        /// <param name="numbers">Непустой массив значений.</param>
        /// <returns>Наибольший общий делитель.</returns>
        public static Int32 AggregatedGsd(Int32[] numbers)
        {
            return numbers.Aggregate(Gcd);
        }

        /// <summary>
        /// Наименьшее общее кратное для двух значений.
        /// </summary>
        /// <param name="a">Первое значение.</param>
        /// <param name="b">Второе значение.</param>
        /// <returns>Наименьшее общее кратное.</returns>
        public static Int32 Lcm(Int32 a, Int32 b)
        {
            return Math.Abs(a * b) / Gcd(a, b);
        }

        /// <summary>
        /// Наибольший общий делитель для двух значений.
        /// </summary>
        /// <param name="a">Первое значение.</param>
        /// <param name="b">Второе значение.</param>
        /// <returns>Наибольший общий делитель.</returns>
        public static Int32 Gcd(Int32 a, Int32 b)
        {
            return b == 0 ? a : Gcd(b, a % b);
        }
    }
}
