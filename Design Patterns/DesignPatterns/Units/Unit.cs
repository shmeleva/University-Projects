using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    abstract class Unit
    {
        protected Int32 _health;

        public abstract String Type     { get; }
        public abstract String Name     { get; }

        public abstract Int32 Attack    { get; }
        public abstract Int32 Defence   { get; }
        public abstract Int32 Cost      { get; }

        protected Unit(int health = 100)
        {
            Health = health;
        }

        /// <summary>
        /// Получает значение здоровья юнита и 0 для мертвого юнита.
        /// </summary>
        public virtual Int32 Health
        {
            get
            {
                return _health;
            }
            protected set
            {
                _health = value > 0 ? value : 0;
            }
        }

        public UnitKeeper AttackTarget(Unit target)
        {
            Int32 damage = Unit.CalculateDamage(this.Attack, target.Defence);

            UnitKeeper targetKeeper = target.FightBack(damage);

            targetKeeper.Log.ObjectName = target.Name;
            targetKeeper.Log.Action = targetKeeper.Changed.Health == 0 ? "killed" : "attacked";
            targetKeeper.Log.SubjectName = this.Name.ToLower();

            return targetKeeper;
        }

        /// <summary>
        /// Отражение нападения. 
        /// </summary>
        /// <param name="damage">Наносимый урон.</param>
        /// <returns>Объект, инкапсулирующий состояние юнита до и после атаки.</returns>
        public virtual UnitKeeper FightBack(Int32 damage)
        {
            Unit damagedClone = this.CloneModifiedUnit(-damage);
            return new UnitKeeper(this, damagedClone, null);
        }

        /// <summary>
        /// Создание копии текущего юнита с измененным здоровьем.
        /// </summary>
        /// <param name="healthPoints">Отрицательное значение для травмирования юнита, положительное - для лечения.</param>
        /// <returns>Новый юнит.</returns>
        public virtual Unit CloneModifiedUnit(Int32 healthPoints)
        {
            // Копия с сохраненным здоровьем.
            if (healthPoints == 0)
                return (Unit)this.MemberwiseClone();

            // Сохраняем здоровье юнита и травмируем (лечим) его.
            Int32 thisHealth = this.Health;
            this.Health += healthPoints;

            // Создаем поврежденную (исцеленную) копию юнита.
            Unit clone = (Unit)this.MemberwiseClone();

            // Восстанавливаем здоровье прототипа.
            this.Health = thisHealth;

            return clone;
        }

        /// <summary>
        /// Вычисление урона, наносимого нападающим жертве.
        /// </summary>
        /// <param name="attacker">Сила атаки нападающего.</param>
        /// <param name="target">Сила защиты.</param>
        /// <returns>Урон.</returns>
        protected static Int32 CalculateDamage(Int32 attack, Int32 defence)
        {
            Int32 damage = (Int32)((attack * BattleRandom.Next(101) / 100) - (defence * BattleRandom.Next(101) / 100));

            // Проверка того, что урон является неотрицательным значением.
            return damage > 0 ? damage : 0;
        }
    }
}