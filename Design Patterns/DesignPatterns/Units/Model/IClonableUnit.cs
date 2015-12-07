using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    interface IClonableUnit
    {
        /// <summary>
        /// Вызывается магом для клонирования юнита.
        /// </summary>
        /// <returns></returns>
        UnitKeeper MagicallyClone();
    }
}
