using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    interface IUnitFactory
    {
        /// <summary>
        /// Получает стоимость самого дешевого юнита из тех, что могут быть созданы фабрикой.
        /// </summary>
        Int32 LowestCost { get; }

        /// <summary>
        /// Возвращает нового юнита или, в случае невозможности создания юнита, null.
        /// </summary>
        /// <param name="cost">Максимальная стоимость создаваемого юнита.</param>
        /// <returns></returns>
        Unit CreateUnit(Int32 cost);
    }
}
