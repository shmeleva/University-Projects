using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    enum ActionTarget
    {
        Ally,
        Enemy,
        Any
    }
    interface ISpecialAction
    {
        /// <summary>
        /// Радиус действия Special Action.
        /// Возвращает целое число в диапазоне от 0 до Int32.MaxValue.
        /// </summary>
        Int32 Range { get; }

        /// <summary>
        /// Процентная вероятность выполнения Special Action.
        /// Возвращает целое число в диапазоне от 0 до 100.
        /// </summary>
        Int32 Probability { get; }

        /// <summary>
        /// Сила Special Action.
        /// Возвращает 0, если сила не задана.
        /// </summary>
        Int32 Power { get; }

        /// <summary>
        /// Получает тип юнита, на которого должно быть направлено
        /// специальное действие.
        /// </summary>
        ActionTarget Target { get; }

        /// <summary>
        /// Проверяет совместимость типа параметра со специальным действием.
        /// </summary>
        /// <param name="target"></param>
        /// <returns>Значение true, если специальное действие может быть использовано
        /// в отношении параметра, и false в обратном случае.</returns>
        Boolean IsCompatibleType(Unit target);

        /// <summary>
        /// Специальное действие.
        /// </summary>
        /// <param name="target">Юнит, на которого направлено специальное действие.</param>
        /// <returns>Объект, инкапсулируюший состояния юнита.</returns>
        UnitKeeper SpecialAction(Unit target);
    }
}
