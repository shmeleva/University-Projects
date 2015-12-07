using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Рыцарь.
    /// </summary>
    class KnightUnit : HeavyUnit, IClonableUnit
    {
        private List<Type> availableAmmunition;

        public override String Name
        {
            get
            {
                return "Knight";
            }
        }

        public override Int32 Attack
        {
            get
            {
                return 100;
            }
        }
        public override Int32 Defence
        {
            get
            {
                return 30;
            }
        }
        public override Int32 Cost
        {
            get
            {
                return 500;
            }
        }

        /// <summary>
        /// Получает список доступной амуниции.
        /// </summary>
        public virtual List<Type> AvailableAmmunition
        {
            get
            {
                return availableAmmunition;
            }
        }

        /// <summary>
        /// Конструктор.
        /// </summary>
        /// <param name="healthPoints">Значение здоровья создаваемого юнита.</param>
        public KnightUnit(Int32 health = 100)
            : base(health)
        {
            availableAmmunition = new List<Type>(5);

            foreach (Type t in Assembly.GetExecutingAssembly().GetTypes())
                if (t.BaseType == typeof(KnightUnitBaseDecorator))
                    availableAmmunition.Add(t);
        }

        /// <summary>
        /// Конструктор копии.
        /// </summary>
        /// <param name="prototype">Прототип создаваемого объекта.</param>
        public KnightUnit(KnightUnit prototype)
            : base(prototype.Health)
        {
            availableAmmunition = new List<Type>(prototype.AvailableAmmunition);
        }

        /// <summary>
        ///  Вооружение.
        /// </summary>
        /// <returns>Объект, инкапсулирующий состояние рыцаря до получаения новой амуниции и после.</returns>
        public virtual UnitKeeper Arm()
        {
            // Отсутствие доступной амуниции.
            if (AvailableAmmunition.Count == 0)
                return new UnitKeeper(this, this, null);

            // Случайный выбор амуниции из списка доступной.
            Int32 index = BattleRandom.Next(AvailableAmmunition.Count);
            Type type = AvailableAmmunition[index];

            // Удаление выбранной амуниции из списка доступной.
            AvailableAmmunition.Remove(type);
            
            // Создание копии текущего юнита, экипированного выбранной амуницией.
            KnightUnit changed = type.GetConstructor(new[] { typeof(KnightUnit) }).Invoke(new object[] { this.CloneModifiedUnit(0) }) as KnightUnit;
            
            return new UnitKeeper(this, changed, null);
        }

        public override Unit CloneModifiedUnit(Int32 healthPoints)
        {
            // Копия с сохраненным здоровьем.
            if (healthPoints == 0)
                return new KnightUnit(this);

            // Сохраняем здоровье юнита и травмируем (лечим) его.
            Int32 thisHealth = this.Health;
            this.Health += healthPoints;

            // Создаем поврежденную (исцеленную) копию юнита, используя конструктор копии.
            KnightUnit clone = new KnightUnit(this);

            // Восстанавливаем здоровье прототипа.
            this.Health = thisHealth;

            return clone;
        }

        #region IClonableUnit

        public virtual UnitKeeper MagicallyClone()
        {
            KnightUnit clone = new KnightUnit(this);
            return new UnitKeeper(this, null, clone);
        }

        #endregion

        public override string ToString()
        {
            return String.Format("K({0})", Health);
        }
    }
}
