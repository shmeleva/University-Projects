using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DesignPatterns
{
    /// <summary>
    /// Лучник.
    /// Стреляет в случайного юнита вражеского стека,
    /// стоящего не далее, чем в 5 клетках от лучника.
    /// </summary>
    class ArcherUnit : LightUnit, ICanBeHealed, IClonableUnit, ISpecialAction
    {
        public override String Name
        {
            get
            {
                return "Archer";
            }
        }

        public override Int32 Attack
        {
            get
            {
                return 50;
            }
        }    
        public override Int32 Defence
        {
            get
            {
                return 20;
            }
        }
        public override Int32 Cost
        {
            get
            {
                return 200;
            }
        }

        public override int Health
        {
            get
            {
                return base.Health;
            }
            protected set
            {
                _health = value > 0 ? (value < MaxHealth ? value : MaxHealth) : 0;
            }
        }

        public ArcherUnit(Int32 health = 100)
        {
            Health = MaxHealth = health;
        }

        #region ICanBeHealed

        public Int32 MaxHealth
        {
            get;
            protected set;
        }

        public UnitKeeper Heal(Int32 healthPoints)
        {
            Unit clone = this.CloneModifiedUnit(healthPoints);
            return new UnitKeeper(this, clone, null);
        }

        #endregion

        #region IClonableUnit

        public UnitKeeper MagicallyClone()
        {
            ArcherUnit clone = (ArcherUnit)this.MemberwiseClone();
            return new UnitKeeper(this, null, clone);
        }

        #endregion

        #region ISpecialAction

        public Int32 Range { get { return 5; } }
        public Int32 Probability { get { return 80; } }
        public Int32 Power { get { return 80; } }

        public ActionTarget Target { get { return ActionTarget.Enemy; } }

        public Boolean IsCompatibleType(Unit target)
        {
            return true;
        }

        /// <summary>
        /// Удар по вражескому юниту.
        /// </summary>
        /// <param name="target">Вражеский юнит.</param>
        /// <returns>Объект, инкапсулирующий состояние вражеского юнита до и после атаки.</returns>
        public UnitKeeper SpecialAction(Unit target)
        {
            // Реализуем как обычный бой, но с силой атаки равной силе специального действия.
            Int32 damage = Unit.CalculateDamage(this.Power, target.Defence);

            UnitKeeper targetKeeper = target.FightBack(damage);

            targetKeeper.Log.ObjectName = target.Name;
            targetKeeper.Log.Action = targetKeeper.Changed.Health == 0 ? "killed" : "shot";
            targetKeeper.Log.SubjectName = this.Name.ToLower();

            return targetKeeper;
        }

        #endregion

        public override string ToString()
        {
            return String.Format("A({0})", Health);
        }
    }
}
