using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Состояния юнита после атаки вражеского юнита или специального действия.
    /// </summary>
    class UnitKeeper
    {
        private readonly Unit _original;
        private readonly Unit _changed;
        private readonly Unit _created;

        private readonly Log _log;

        /// <summary>
        /// Получает исходное состояние атакуемого юнита или юнита,
        /// на которого направлено специальное действие.
        /// </summary>
        public Unit Original
        {
            get
            {
                return _original;
            }
        }

        /// <summary>
        /// Получает измененное состояние атакуемого юнита или юнита,
        /// на которого направлено специальное действие или null,
        /// если юнит не был изменен.
        /// </summary>
        public Unit Changed
        {
            get
            {
                return _changed;
            }
        }

        /// <summary>
        /// Получает созданного в ходе клонирования магом юнита
        /// или null, если новый юнит не был создан.
        /// </summary>
        public Unit Created
        {
            get
            {
                return _created;
            }
        }

        public Log Log
        {
            get
            {
                return _log;
            }
        }

        public UnitKeeper(Unit original, Unit changed, Unit created)
        {
            _original = original;
            _changed = changed;
            _created = created;

            _log = new Log();
        }
    }
}
