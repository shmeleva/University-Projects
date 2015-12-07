using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    interface ICanBeHealed
    {
        /// <summary>
        /// Получает максимально возможное здоровье юнита.
        /// </summary>
        Int32 MaxHealth { get; }

        /// <summary>
        /// Вызывается лекарем для лечения юнита.
        /// Увеличивает здоровье юнита на указанное число очков.
        /// </summary>
        /// <param name="healthPoints">Health Points.</param>
        UnitKeeper Heal(Int32 healthPoints);
    }
}
